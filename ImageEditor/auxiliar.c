//Novetschi Vlad 312CA
#include "header.h"

void citirecom(FILE *f, char *tip, void *adr1, void *adr2)
{
	char tmp[256];
	//ignoram liniile care incep cu #
	while (fgets(tmp, 256, f)[0] == '#')
		;
	//parsam linie in functie de tipul cerut
	if (sscanf(tmp, tip, adr1, adr2) == 0)
		printf("Eroare Citire\n");
}

int min(int x, int y)
{
	if (x < y)
		return x;
	return y;
}

int rotateinvalid(selectie s, img x)
{
	//testam daca selectia e patrata
	if (s.x2 - s.x1 == s.y2 - s.y1)
		return 0;
	//testam daca intreaga imagine e selectata
	if (s.x2 == x.dimx && s.y2 == x.dimy)
		return 0;
	//pentru selectii ce nu pot fi rotite returnam 1
	return 1;
}

void swap(int *x, int *y)
{
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

void rotire(img *x, selectie s)
{
	//creem o noua imagine de dimensiunea selectiei
	img tmp;
	tmp.tip = x->tip;
	tmp.vmax = x->vmax;
	tmp.dimx = s.y2 - s.y1;
	tmp.dimy = s.x2 - s.x1;
	allocimg(&tmp);

	//salvam pixelii rotiti
	for (int i = s.x1; i < s.x2; ++i)
		for (int j = s.y1; j < s.y2; ++j)
			tmp.m[i - s.x1][j - s.y1] = (x->m)[s.y2 - j - 1 + s.y1][i];

	//daca rotim intreaga imagine
	if (tmp.dimx == x->dimy && tmp.dimy == x->dimx) {
		stergereimg(*x);//stergem imaginea precedenta
		*x = tmp;		//o inlocuim cu cea rotita

	} else { //daca selectia e patrata
		for (int i = s.y1; i < s.y2; ++i)
			for (int j = s.x1; j < s.x2; ++j)
				//inlocuim pixelii selectiei cu cei rotiti
				(x->m)[i][j] = tmp.m[i - s.y1][j - s.x1];
		stergereimg(tmp);	//eliberam spatiul
	}
}

int sinvalida(selectie s, img x)
{
	//testam daca coordonatele selectiei nu sunt in interiorul imaginii
	if (s.x2 > x.dimx || s.y2 > x.dimy)
		return 1;
	if (s.x1 < 0 || s.y1 < 0)
		return 1;

	//testam daca aria selectiei nu este 0
	if (s.x2 - s.x1 == 0 || s.y2 - s.y1 == 0)
		return 1;

	//returnam 0 daca selectia e valida
	return 0;
}
