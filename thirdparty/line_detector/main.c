///////////////////////////////////////////////////////////////////////
// File:        main.c
// Description: Example of a correct use
// Author:      Ana Belén Cambra
// Created:     Mon Jun 20 2011, 12:53:35 
//
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "extractSegments.h"

int main (int argc, const char * argv[]) {
   
	IplImage *imagen, *imagen_GRAY, *img_color;
	tpRectas rectas;
	int num;
	
	if (argc!=3) {
		printf("Incorrect use!\n Correct use: <Program_name> <image_name> <min_pixel/straight>\n");
		exit(-1);
	
	}

	imagen=cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	
	if (imagen == 0){
		printf("ERROR to load the image: %s\n",argv[1]);
		exit(-1);
	}
	
	
	imagen_GRAY=cvCreateImage(cvGetSize(imagen),8, 1);
	cvCvtColor(imagen, imagen_GRAY, CV_RGB2GRAY);
	
	//reserve memory
	rectas.numRectas=0;
	rectas.maximo=MAX_RECTAS;
	
	rectas.rectas=(tpRecta *)malloc(rectas.maximo*sizeof(tpRecta));
	
	num=atoi(argv[2]);
	extractSegments(imagen_GRAY, &rectas, num);

//gui part is not needed
#if 0		
	img_color=cvCreateImage(cvGetSize(imagen),8, 3);
	cvCvtColor(imagen_GRAY, img_color, CV_GRAY2RGB);
	
	showSegments(rectas,img_color);
	
	//show the original image and the segments detected	
	cvNamedWindow("Input image",CV_WINDOW_AUTOSIZE);
	cvShowImage("Input image", imagen);
	
	cvNamedWindow("segments detected",CV_WINDOW_AUTOSIZE);
	cvShowImage("segments detected", img_color);
	

	cvWaitKey(0);
	cvDestroyAllWindows();
	
	//free memory
	cvReleaseImage(&imagen);
	cvReleaseImage(&img_color);
#endif
	cvReleaseImage(&imagen_GRAY);
	free(rectas.rectas);
	
    return 0;
}
