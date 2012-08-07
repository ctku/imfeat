///////////////////////////////////////////////////////////////////////
// File:        extractSegments.c
// Description: Functions to extract and show the straight segments of an image
// Author:      Ana Belén Cambra
// Created:     Mon Jun 20 2011, 12:53:35 
//
///////////////////////////////////////////////////////////////////////

#include "extractSegments.h"
/****************************************************************************************/
void rellenarMascaras(MASCARA *m1,MASCARA *m2){
	
	m1->tamano=3;
	m1->matriz= (float *)malloc(m1->tamano*sizeof(float));
	m1->matriz[0] = 1.0;
	m1->matriz[1] = 0.0;
	m1->matriz[2] = -1.0;
	m1->sumaPositivos=1.0;
	
	m2->tamano=3;
	m2->matriz= (float *)malloc(m2->tamano*sizeof(float));
	m2->matriz[0] = 1.0;
	m2->matriz[1] = 2.0;
	m2->matriz[2] = 1.0;
	m2->sumaPositivos=4.0;
}
/****************************************************************************************/
void eliminarMascaras(MASCARA *m1,MASCARA *m2){
	free(m1->matriz);
	free(m2->matriz);
}
/****************************************************************************************/
/****************************************************************************************/
void calculoGradientes(IplImage *imagen,int filas,int columnas, IplImage *dx, IplImage *dy){
	
	MASCARA m1,m2;
	int i,j,m,tamBasura;
	float *filaAuxX,*filaAuxY;
	
	rellenarMascaras(&m1,&m2);
	tamBasura= 1;
	
	/*reservar para los valores de la fila auxiliar*/
	filaAuxX=(float *)malloc(columnas*sizeof(float));
	memset(filaAuxX, 0,columnas*sizeof(float));
	filaAuxY=(float *)malloc(columnas*sizeof(float));
	memset(filaAuxY, 0,columnas*sizeof(float));
	
	for (i=0; i < filas;i++){
		for (j=0; j < columnas;j++){			
			/*calculamos el valor de la mascara por FILAS*/
			if ((i-tamBasura<0)||(i+tamBasura>=filas)){
				/*zona de basura, rellenar con ceros esas columnas*/
				filaAuxX[j]=0.0;				
				filaAuxY[j]=0.0;
			}
			else{
				/*calculo gradientes por filas*/				
				for(m=0; m<m1.tamano; m++){				
					/*gradiente X	filaaux(j)=(m2(i))*i(i,j) */
					filaAuxX[j] += m2.matriz[m]*(float)CV_IMAGE_ELEM(imagen,uchar,(i-tamBasura + m),j);
					/*gradiente Y	filaaux(j)= (m1(i))*i(i,j) */
					filaAuxY[j] += m1.matriz[m]*(float)CV_IMAGE_ELEM(imagen,uchar,(i-tamBasura + m),j);
				}				
			}
		}		
		for (j=0; j < columnas;j++){			
			/*calculamos el valor de la mascara por COLUMNAS*/
			if ((j-tamBasura < 0)||(j+tamBasura>=columnas)){
				/*zona de basura, rellenar con ceros esas filas*/
				CV_IMAGE_ELEM(dx,float,i,j)=0.0;				
				CV_IMAGE_ELEM(dy,float,i,j)=0.0;	
				/*calculo del modulo*/
			}
			else{				
				/*calculo X gradientes por columnas*/				
				for(m=0; m<m2.tamano; m++){
					/*calculo X  por columnas - m1(j)* filaaux*/
					CV_IMAGE_ELEM(dx,float,i,j) += (filaAuxX[j-tamBasura+m])*(-m1.matriz[m]);					
					/*calculo Y  por columnas m2(j)* filaaux*/
					CV_IMAGE_ELEM(dy,float,i,j) += (m2.matriz[m]) * (filaAuxY[j-tamBasura+m]);
				}				
				/*Una vez calculados los gradientes, dividir por la suma de los valores positivos de la mascara*/
				CV_IMAGE_ELEM(dx,float,i,j) = CV_IMAGE_ELEM(dx,float,i,j)/(m1.sumaPositivos*m2.sumaPositivos);	
				CV_IMAGE_ELEM(dy,float,i,j) = CV_IMAGE_ELEM(dy,float,i,j)/(m1.sumaPositivos*m2.sumaPositivos);					
			}
		}
		/*limpiamos las variables para las siguientes iteraciones*/
		for(j=0; j < columnas; j++){
			filaAuxX[j] = 0.0;
			filaAuxY[j] = 0.0;
		}
	}
	free(filaAuxX);
	free(filaAuxY);
	eliminarMascaras(&m1,&m2);
}
/****************************************************************************************/
/****************************************************************************************/
int seleccionFilaDeConjunto(int conjunto,int *dir1,int *dir2,int *dir3,int *dir4,int *dir5,
							int *dir6,int *dir7,int *dir8){
	int d=0;
	
	switch (conjunto) {
		case 1:
			d=*dir1;
			(*dir1)=(*dir1)+1;
			break;
		case 2:
			d=*dir2;
			(*dir2)=(*dir2)+1;
			break;
		case 3:
			d=*dir3;
			(*dir3)=(*dir3)+1;
			break;
		case 4:
			d=*dir4;
			(*dir4)=(*dir4)+1;
			break;
		case 5:
			d=*dir5;
			(*dir5)=(*dir5)+1;
			break;
		case 6:
			d=*dir6;
			(*dir6)=(*dir6)+1;
			break;
		case 7:
			d=*dir7;
			(*dir7)=(*dir7)+1;
			break;
		case 8:
			d=*dir8;
			(*dir8)=(*dir8)+1;
	}
	return d;
	
}
/*****************************************************************************************/
/****************************************************************************************/
void modificarPixels(int grupo, int dir,IplImage *imagen, int valorNuevo,int **direction, int tamFila, int tamColumna ){
	
	int i;	
	for (i=0; i < dir; i++) {
		CV_IMAGE_ELEM(imagen,uchar,I_INDICE( direction[grupo][i],tamFila),
					  J_INDICE( direction[grupo][i],tamFila))=1;
	}
	
}

/****************************************************************************************/
/****************************************************************************************/
int obtenerPixels(int conjunto,IplImage *imagen,
				  int dir1,int dir2,int dir3,int dir4,int dir5,int dir6,int dir7,int dir8,
				  int **direction){
	
	//direction: matriz[k] continen los indices de la imagne contornos
	//tamañoFilas*(columna-1)+fila
	int num=0,k,dir;
	CvSize tam;
	
	tam=cvGetSize(imagen);
	switch (conjunto) {
		case 1:
			num=dir8+dir1+dir2;
			//indices de vecinos de 1 son:7,0,1
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir8;
						break;
					case 1:
						dir=dir1;
						break;
					case 2:
						dir=dir2;
						break;
				}
				if (k==0)	modificarPixels(7, dir, imagen, 1, direction, tam.width,tam.height);
				else modificarPixels(k-1, dir, imagen, 1, direction, tam.width,tam.height);
			}
			break;
		case 2:
			num=dir1+dir2+dir3;
			//indices de vecinos de 2 son:0,1,2
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir1;
						break;
					case 1:
						dir=dir2;
						break;
					case 2:
						dir=dir3;
						break;
				}
				modificarPixels(k, dir, imagen, 1, direction, tam.width,tam.height);
			}
			break;
		case 3:
			num=dir2+dir3+dir4;
			//indices de vecinos de 3 son:1,2,3
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir2;
						break;
					case 1:
						dir=dir3;
						break;
					case 2:
						dir=dir4;
						break;
				}
				modificarPixels(k+1, dir, imagen, 1, direction, tam.width,tam.height);
			}
			break;
		case 4:
			num=dir3+dir4+dir5;
			//indices de vecinos de 4 son:2,3,4
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir3;
						break;
					case 1:
						dir=dir4;
						break;
					case 2:
						dir=dir5;
						break;
				}
				modificarPixels(k+2, dir, imagen, 1, direction, tam.width,tam.height);
			}
			break;
		case 5:
			num=dir4+dir5+dir6;
			//indices de vecinos de 5 son:3,4,5
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir4;
						break;
					case 1:
						dir=dir5;
						break;
					case 2:
						dir=dir6;
						break;
				}
				modificarPixels(k+3, dir, imagen, 1, direction, tam.width,tam.height);
			}
			break;
		case 6:
			num=dir5+dir6+dir7;
			//indices de vecinos de 6 son:4,5,6
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir5;
						break;
					case 1:
						dir=dir6;
						break;
					case 2:
						dir=dir7;
						break;
				}
				modificarPixels(k+4, dir, imagen, 1, direction,tam.width,tam.height);
			}
			break;
		case 7:
			num=dir6+dir7+dir8;
			//indices de vecinos de 7 son:5,6,7
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir6;
						break;
					case 1:
						dir=dir7;
						break;
					case 2:
						dir=dir8;
						break;
				}
				modificarPixels(k+5, dir, imagen, 1, direction, tam.width,tam.height);
			}
			break;
		case 8:
			num=dir7+dir8+dir1;
			//indices de vecinos de 8 son:6,7,0
			for (k=0; k<3; k++) {
				switch (k) {
					case 0:
						dir=dir7;
						break;
					case 1:
						dir=dir8;
						break;
					case 2:
						dir=dir1;
						break;
				}
				if (k==2)	modificarPixels(0, dir, imagen, 1, direction, tam.width,tam.height);
				else modificarPixels(k+6, dir, imagen, 1, direction,tam.width,tam.height);
			}
	}
	return num;
	
}
/****************************************************************************************/
/****************************************************************************************/
void anyadirIndiceAconjunto(int** direction, int modulo, int indice, int d){
	if (((d)+1) >= MAX_INDICE) {
		printf("Superado el número de indices por conjunto (MAX_INDICE)\n");
	}else{
		direction[(int)(modulo-1)][d]=indice;
	}
}
/****************************************************************************************/
/****************************************************************************************/
void anyadirNuevaRecta(tpRectas *rectas, tpRecta nuevaRecta){
	
	if (((rectas->numRectas)+1) >= rectas->maximo) {
		printf("Superado el número de máximo de rectas(MAX_RECTAS)\n");
	}else{
		rectas->rectas[rectas->numRectas]=nuevaRecta;					
		rectas->numRectas=rectas->numRectas+1;
	}
}
/****************************************************************************************/
/****************************************************************************************/
void extractSegments (IplImage *imagen, tpRectas *rectas, int longitud_minima) {
	
	int i,j,modulo,d=0,k,n,total,etiqueta=1,columna,fila,maxX,maxY,minX,minY;
	
	
	int **direction;
	int dir1=0,dir2=0,dir3=0,dir4=0,dir5=0,dir6=0,dir7=0,dir8=0;
	
	double angulo,theta,conf,r;
	int edge1[TAMEDGE];
	
	float zmx,zmy,auxX,auxY,zmxy;
	
	tpRecta nuevaRecta;	
	
	CvPoint punto;	
	IplImage *contornos,*im_gaus,*dx,*dy,*etiquetas,*dir_im;
	CvSize tamano;
	
	//indices=(int *)malloc(sizeof(int)*50000);
	direction=(int **)malloc(9*sizeof(int*));
	for (i=0; i<9; i++) {
		direction[i]=(int *)malloc(sizeof(int)*MAX_INDICE);
	}
	
	//desenfocar la imagen aplicando filtro de gaus
	im_gaus= cvCreateImage(cvGetSize(imagen), IPL_DEPTH_8U, 1);
	cvZero(im_gaus);
	cvSmooth(imagen, im_gaus, CV_GAUSSIAN, 7, 7, 1.5, 0);
	tamano=cvGetSize(imagen);
	contornos= cvCreateImage(tamano, IPL_DEPTH_8U, 1);
	cvZero(contornos);
	//aplicar la mascara de canny
	cvCanny(im_gaus, contornos, 75,300 , 5);
	
	//como las mascra es de 5, borrar 2 filas y 2 columnas;Zona de BASURA
	for (j=0; j<tamano.width; j++) {
		for (i=0; i<2; i++) {
			CV_IMAGE_ELEM(contornos,uchar,i,j)=0;
			CV_IMAGE_ELEM(contornos,uchar,(tamano.height-1-i),j)=0;
		}
	}
	for (i=0; i<tamano.height; i++) {
		for (j=0; j<2; j++) {
			CV_IMAGE_ELEM(contornos,uchar,i,j)=0;
			CV_IMAGE_ELEM(contornos,uchar,i,tamano.width-1-j)=0;
		}
	}
	
	//variables para el calculo gradiente y orienacion
	dx = cvCreateImage(tamano, IPL_DEPTH_32F, 1);
	dy = cvCreateImage(tamano, IPL_DEPTH_32F, 1);
	cvZero(dx);
	cvZero(dy);
	calculoGradientes(im_gaus,tamano.height,tamano.width, dx, dy);
	cvReleaseImage(&im_gaus);
	
	//recorrer por filas	
	for (i=0; i<tamano.height; i++) {
		for (j=0; j<tamano.width; j++) {
			if (CV_IMAGE_ELEM(contornos,uchar,i,j)!= 0) {
				//almacenar el indice de los contornos
				//indices[numIndices]=INDICE(i,j,tamano.width);
				//numIndices++;
				
				if (( CV_IMAGE_ELEM(dx,double,i,j)==0)) {
					//CV_IMAGE_ELEM(dx,double,i,j)=0.000000001;
					angulo=atan((double)CV_IMAGE_ELEM(dy,float,i,j)/0.00001);
				}else
					//calcular la orientaciones del gradiente				
					angulo=atan((double)CV_IMAGE_ELEM(dy,float,i,j)/(double)CV_IMAGE_ELEM(dx,float,i,j));
				
				//asignar a cada orientacion del gradiente un conjuto				
				if ((angulo >= (pi/16.0))&&(angulo<(3*pi/16.0))) modulo=1;				
				else if ((angulo >= (3*pi/16.0))&&(angulo<(5*pi/16.0))) modulo=2;
				else if ((angulo >= (5*pi/16.0))&&(angulo<(7*pi/16.0))) modulo=3;
				else if(((angulo >= (7*pi/16.0))||(angulo<-(7*pi/16.0)))) modulo=4;
				
				else if ((angulo >= -(7*pi/16.0))&&(angulo<-(5*pi/16.0))) modulo=5;
				else if ((angulo >= -(5*pi/16.0))&&(angulo<-(3*pi/16.0))) modulo=6;
				else if ((angulo >= -(3*pi/16.0))&&(angulo<-(pi/16.0))) modulo=7;
				else if ((angulo >= -(pi/16.0))&&(angulo<(pi/16.0))) modulo=8;
				else modulo=8;
	
				d=seleccionFilaDeConjunto(modulo,&dir1,&dir2,&dir3,&dir4,&dir5,&dir6,&dir7,&dir8);
				anyadirIndiceAconjunto(direction, (int)modulo,INDICE(i,j,tamano.width),d);
			}//if
			//else cvSet2D(final,y,x,CV_RGB(0,0,0));
		}//for y
	}//for x
	
	cvReleaseImage(&dx);
	cvReleaseImage(&dy);
	cvReleaseImage(&contornos);
	//agrupar los contornos
	dir_im= cvCreateImage(tamano, IPL_DEPTH_8U, 1);
	etiquetas=cvCreateImage(tamano, IPL_DEPTH_16U, 1);
	
	for (k=1; k<=8; k++) {		
		
		
		cvZero(dir_im);		
		//contar los pixels de los conjuntos vecinos y poner a 1 dichos pixels de la matriz dir_im
		obtenerPixels(k,dir_im,dir1,dir2,dir3,dir4,dir5,dir6,dir7,dir8,direction);
		cvZero(etiquetas);		
		
		//etiquetar las vecinas conectadas
		//obtener la cadena de vecinas conectadas
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* contour,*cur_contour;
		CvSeqReader reader; 
		
		n=cvFindContours( dir_im, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE,cvPoint(0, 0));
		
		//reservamos espaicio para los contornos etiqueados
		int *edges=(int *)malloc((n+1)*sizeof(edge1));
		memset(edges,0,((n+1)*sizeof(edge1)));
		int *sizeEdges=(int *)malloc((n+1)*sizeof(int));
		memset(sizeEdges,0,((n+1)*sizeof(int)));
		float *mediaI=(float *)malloc((n+1)*sizeof(float));
		memset(mediaI,0,(n+1)*sizeof(float));
		float *mediaJ=(float *)malloc((n+1)*sizeof(float));
		memset(mediaJ,0,(n+1)*sizeof(float));
		
		for (i=1; i<n; i++) {
			sizeEdges[i]=0;
		}
		etiqueta=1;
		//a cada cadena se le asirna una etiqueta distinta
		for(cur_contour = contour ;cur_contour != 0;cur_contour=cur_contour->h_next) { 
			total = cur_contour->total;
			
			mediaI[etiqueta]=0.0;
			mediaJ[etiqueta]=0.0;
			cvStartReadSeq( cur_contour, &reader, 0 ); // read from beginning
			//a cada pixel de cada cadena se le asigna la misma etiqueta
			//if (total > 5){
			
			if (total > TAMEDGE) {
				printf("El contorno contiene más puntos que TAMEDGE!!!!\n");
				total=TAMEDGE;
			}
			for( i = 0; i < total; i++ ){ 
				CV_READ_SEQ_ELEM( punto, reader ) 
				CV_IMAGE_ELEM(etiquetas,unsigned int,punto.y,punto.x)=etiqueta;
				//almacenar los pixels correspondientes a cada etiqueta
				//guardar los indices pero por filas
				edges[etiqueta*TAMEDGE+sizeEdges[etiqueta]]=INDICE(punto.y,punto.x,tamano.width);
				sizeEdges[etiqueta]=sizeEdges[etiqueta]+1;
				//ir calculando la media de X e Y. coordenadas de los indices de cada etiqueta
				mediaI[etiqueta]=mediaI[etiqueta]+(float)(punto.y);
				mediaJ[etiqueta]=mediaJ[etiqueta]+(float)(punto.x);
			} 
			
			//acabar de calcular la media de cada etiqueta
			mediaI[etiqueta]=mediaI[etiqueta]/(float)sizeEdges[etiqueta];
			mediaJ[etiqueta]=mediaJ[etiqueta]/(float)sizeEdges[etiqueta];
			etiqueta++;			
		}		
		
		//recorrer cada contorno
		for (i=1; i<=n; i++) {
			zmx=0;
			zmy=0;
			zmxy=0;
			maxX=0;
			maxY=0;
			minX=tamano.width;
			minY=tamano.height;
			
			if (sizeEdges[i] >= longitud_minima) {
				//recorrer todos los indices de cada etiqueta para calcular zmx e y
				for (j=0; j<sizeEdges[i]; j++) {
					
					fila=I_INDICE(edges[i*TAMEDGE+j],tamano.width);					
					columna=J_INDICE(edges[i*TAMEDGE+j],tamano.width);
					
					maxX=MAX(maxX,columna);
					maxY=MAX(maxY,fila);
					minX=MIN(minX,columna);
					minY=MIN(minY,fila);
					
					auxX=(columna-mediaJ[i]);
					auxY=(fila-mediaI[i]);
					
					zmx=zmx+(auxX*auxX);					
					zmy=zmy+(auxY*auxY);
					zmxy=zmxy+(auxX*auxY);					
				}				
				CvMat *eVec = cvCreateMat(2,2,CV_32F);
				CvMat *eVal = cvCreateMat(2,1,CV_32F);				
				CvMat *matrizD=cvCreateMat(2, 2, CV_32F);
				
				matrizD->data.fl[0*matrizD->cols+0]=zmx;
				matrizD->data.fl[0*matrizD->cols+1]=zmxy;
				matrizD->data.fl[1*matrizD->cols+0]=zmxy;
				matrizD->data.fl[1*matrizD->cols+1]=zmy;
				
				//calcular valores y vectores propios
				//el priemr valor propio es el mayor, ordenados descendientemente
				cvEigenVV( matrizD, eVec, eVal, 0,0,0);
				theta=atan2(eVec->data.fl[0*eVec->cols+1], eVec->data.fl[0*eVec->cols+0]);
				
				if (eVal->data.fl[1] > 0.0) {
					conf=(eVal->data.fl[0]/eVal->data.fl[1]);
				}
				else {
					conf=100000;
				}
				if (conf>=400) {
					r=sqrt(((maxX-minX)*(maxX-minX))+((maxY-minY)*(maxY-minY)));
					
					nuevaRecta.inicio.x=abs((int)(mediaJ[i]-(float)(cos(theta)*r/2.0)));
					nuevaRecta.final.x=mediaJ[i]+(float)(cos(theta)*r/2.0);
					nuevaRecta.inicio.y=mediaI[i]-(float)(sin(theta)*r/2.0);
					nuevaRecta.final.y=mediaI[i]+(float)(sin(theta)*r/2.0);
					
					//calcular la pendiente de la recta
					if (abs(nuevaRecta.final.x-nuevaRecta.inicio.x)==0) {
						nuevaRecta.pendiente=(double)(nuevaRecta.final.y-nuevaRecta.inicio.y)/0.000001;
					}else{
						nuevaRecta.pendiente=(double)(nuevaRecta.final.y-nuevaRecta.inicio.y)/
						(double)(nuevaRecta.final.x-nuevaRecta.inicio.x);
					}
					//calcular los otros parametros de la recta
					nuevaRecta.theta=theta;
					nuevaRecta.p=mediaJ[i]*cos(theta)+mediaI[i]*sin(theta);
					
					//si la inclinacion de la recta supera los 45 grados es vertical
					if (fabs(nuevaRecta.pendiente)<=1){
						nuevaRecta.etiqueta=HORIZONTAL;
						
					}else{
						nuevaRecta.etiqueta=VERTICAL;
					}					
					//añadir nueva recta
					anyadirNuevaRecta(rectas,nuevaRecta);
				}
				cvReleaseMat(&eVec);
				cvReleaseMat(&matrizD);
				cvReleaseMat(&eVal);
			}//if
			
		}//for
		free(edges);
		free(sizeEdges);
		free(mediaI);
		free(mediaJ);
	}
	
	cvReleaseImage(&dir_im);
	cvReleaseImage(&etiquetas);
	//free(indices);
	free(direction);
	//return numVerticales;
}
/****************************************************************************/ 
void showSegments(tpRectas rectas, IplImage *img_color) {
/****************************************************************************/
	int i;
	CvScalar color;
	
	for (i = 0; i < rectas.numRectas; i++){
		
		color = CV_RGB( rand()&255, rand()&255, rand()&255 );
		
		cvLine( img_color,  rectas.rectas[i].inicio,rectas.rectas[i].final, color, 2, CV_AA,0);
		
		printf("->Recta %d Pto.inicio(%d,%d) Pto.final(%d,%d) tipo=%d pendiente=%f (%f,%f)\n",
			   i+1,
			   rectas.rectas[i].inicio.x,rectas.rectas[i].inicio.y,
			   rectas.rectas[i].final.x,rectas.rectas[i].final.y,
			   rectas.rectas[i].etiqueta,
			   rectas.rectas[i].pendiente,
			   rectas.rectas[i].theta,
			   rectas.rectas[i].p);
	}
}
/*****************************************************************************/


