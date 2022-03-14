//Novetschi Vlad 312CA
#include "header.h"

void allocimg(img *x)
{
	x->m = malloc(x->dimy * sizeof(pixel *));
	if (!x->m) //verificam alocarea
		printf("Eroare Alocare\n"),
		exit(1);
	for (int i = 0; i < x->dimy; ++i) {
		x->m[i] = malloc(x->dimx * sizeof(pixel));
		if (!x->m[i])
			printf("Eroare Alocare\n"),
			exit(1);
	}
}

void stergereimg(img x)
{
	if (x.tip != 0) { //testam daca avem o imagine alocata
		for (int i = 0; i < x.dimy; ++i)
			free(x.m[i]);
	free(x.m);
	x.tip = 0;
	}
}

int main(void)
{
	img x;
	selectie s;
	x.tip = 0;
	int exit = 0;

	while (!exit) {
		char linie[256];
		fgets(linie, 256, stdin);
		char comanda[100], argument[100] = {0};
		//parsam linia introdusa in sirurile comanda si argument
		int check = sscanf(linie, " %s %s", comanda, argument);

		if (!strcmp(comanda, "EXIT") || !check) {
			stergereimg(x); //dealocam memoria
			if (!x.tip)
				printf("No image loaded\n");
			exit = 1; //oprim programul

		} else if (!strcmp(comanda, "LOAD")) {
			stergereimg(x);				//stergem imaginea precedenta
			if (citireimg(&x, argument))//citim imaginea
				selectall(&s, x);		//o selectam in intregime

		} else if (!strcmp(comanda, "SELECT")) {
			if (strcmp(argument, "ALL") == 0) {
				if (selectall(&s, x))
					printf("Selected ALL\n");
			} else {
				selectd(&s, x, linie);
			}

		} else if (!strcmp(comanda, "GRAYSCALE")) {
			graysc(x, s);

		} else if (!strcmp(comanda, "SEPIA")) {
			sepia(x, s);

		} else if (!strcmp(comanda, "CROP")) {
			if (crop(&x, s)) //daca selectia a fost efectuata cu succes
				selectall(&s, x); //selectam imaginea

		} else if (!strcmp(comanda, "ROTATE")) {
			if (rotate(&x, s, argument)) {//rotim imaginea
				//daca a fost rotita intreaga imagine, o selectam
				if (s.x2 - s.x1 != s.y2 - s.y1)
					selectall(&s, x);
			}

		} else if (!strcmp(comanda, "SAVE")) {
			char ascii[10];
			if (sscanf(linie, " %s %s %s", comanda, argument, ascii) == 3)
				scriere(x, argument, 1); //scriere ascii
			else
				scriere(x, argument, 0); //scriere binara

		} else {
			printf("Invalid command\n");
		}
	}
	return 0;
}
