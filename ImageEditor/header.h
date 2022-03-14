//Novetschi Vlad 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pixel {
	unsigned char r, g, b;
} pixel;

typedef struct imagine {
	int tip;	//tipul imaginii
	int vmax;	//valoarea maxima
	int dimx;	//latimea
	int dimy;	//inaltimea
	pixel **m;
} img;

typedef struct selectie {
	int x1, y1, x2, y2;
} selectie;

void allocimg(img *x);		//ALOCARE
void stergereimg(img x);	//ELIBERARE SPATIU

//citeste datele imaginii, ignorand liniile care incep cu #
void citirecom(FILE *f, char *tip, void *adr1, void *adr2);
int citireimg(img *x, char *numef);			//LOAD

int scriere(img x, char *numef, int ascii);	//SAVE

int sinvalida(selectie s, img x);//returneaza 1 pentru selectii invalide
int selectd(selectie *t, img x, char *linie);	//SELECT
int selectall(selectie *s, img x);				//SELECT ALL

int graysc(img x, selectie s);	//GRAYSCALE
int sepia(img x, selectie s);	//SEPIA

int rotateinvalid(selectie s, img x);//verifica daca selectia poate fi rotita
int rotate(img *x, selectie s, char *arg);//efectueaza rotiri succesive
void rotire(img *x, selectie s);//efectueaza rotirea la dreapta cu 90 de grade

int crop(img *x, selectie s);

int min(int x, int y);
void swap(int *x, int *y);
