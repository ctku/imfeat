
There is an example in the file "main.c" where you see how to use this file to extract the straights.

It is necessary indicate where are the libraries of OpenCV. This place depends of the OpenCV install

#gcc -c -I "/opt/local/include/opencv" -L"/opt/local/lib" extractSegments.c
#gcc -I "/opt/local/include/opencv" -L"/opt/local/lib" -lhighgui -lcv -lcvaux -lcxcore extractSegments.o main.c -o 
main

gcc -c -I"usr/include/opencv" -L"usr/lib" extractSegments.c
gcc -I"/usr/include/opencv" -L"/usr/lib" -l opencv_highgui -opencv -opencv_aux -opencv_cxcore extractSegments.o 
main.c -o main



//example of correct use
./main photo1.jpg 200


