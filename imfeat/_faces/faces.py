import cv2
import numpy as np
import os
import imfeat
from . import __path__


def _detect_faces(img, cascade):
    min_size = (20, 20)
    image_scale = 2
    haar_scale = 1.2
    min_neighbors = 2
    haar_flags = 0
    small_width = img.shape[1] / image_scale
    small_height = img.shape[0] / image_scale
    small_img = cv2.resize(img, (small_width, small_height))
    cv2.equalizeHist(small_img, small_img)
    faces = cascade.detectMultiScale(small_img, scaleFactor=haar_scale, minNeighbors=min_neighbors, flags=haar_flags, minSize=min_size)
    return [((x * image_scale, y * image_scale,
              w * image_scale, h * image_scale), n)
            for (x, y, w, h), n in faces]


class Faces(imfeat.BaseFeature):

    def __init__(self):
        super(Faces, self).__init__({'type': 'numpy', 'dtype': 'uint8', 'mode': 'gray'})
        path = 'haarcascade_frontalface_default.xml'
        if os.path.exists(path):
            self._cascade = cv2.CascadeClassifier(path)
        else:
            path = (__path__[0] + '/data/haarcascade_frontalface_default.xml')
            if os.path.exists(path):
                self._cascade = cv2.CascadeClassifier(path)
            else:
                raise ValueError("Can't find .xml file!")

    def __call__(self, image):
        image = self.convert(image)
        faces = _detect_faces(image, self._cascade)
        if faces:
            mean_pos = np.mean(np.asfarray([x[0] for x in faces]), 0)
            mean_pos[0] /= image.size[0]
            mean_pos[1] /= image.size[1]
            return np.asfarray([len(faces)] + mean_pos.tolist())
        return np.zeros(5)
