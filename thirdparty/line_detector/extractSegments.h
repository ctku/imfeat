///////////////////////////////////////////////////////////////////////
// File:        extractSegments.h
// Description: Functions to extract and show the straight segments of an image
// Author:      Ana Belén Cambra
// Created:     Mon Jun 20 2011, 12:53:35 
//
///////////////////////////////////////////////////////////////////////


//OpenCV

#include "cv.h"
#include "highgui.h"

#include <stdio.h>


#define pi 3.141592653589793

#define INDICE(i,j,numColumnas) (i*numColumnas+j)
#define I_INDICE(indice,numColumnas) (indice/numColumnas)
#define J_INDICE(indice,numColumnas) (indice%numColumnas)

#define MAX_INDICE 50000
#define TAMEDGE 1000
#define MAX_RECTAS 100

typedef enum {
	HORIZONTAL,
	VERTICAL,
	INDEFINIDA
}tipoRecta;

typedef struct{
	CvPoint inicio;
	CvPoint final;
	tipoRecta etiqueta;
	double pendiente;
	double theta;
	double p;
}tpRecta;

typedef struct{
	tpRecta *rectas;
	int numRectas;
	int maximo;
}tpRectas;

/*definicion de la estructura de la mascara*/
typedef struct{
	/*nxn, tamano es n*/
	int tamano;
	/*valores de mascara*/
	float *matriz;
	float sumaPositivos;
	
} MASCARA;

/****************************************************************************************/
void extractSegments (IplImage *imagen, tpRectas *rectas, int longitud_minima);
/****************************************************************************************/
/****************************************************************************************/
void showSegments(tpRectas rectas, IplImage *img_color);
/****************************************************************************************/

