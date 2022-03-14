//Novetschi Vlad 312CA
#include "header.h"

int citireimg(img *x, char *numef)
{
	FILE *f = fopen(numef, "r");
	if (!f) {
		x->tip = 0;
		printf("Failed to load %s\n", numef);
		return 0;
	}
	char tmp;
	citirecom(f, "%c%d", &tmp, &x->tip);		//citim tipul
	citirecom(f, "%d%d", &x->dimx, &x->dimy);	//citim dimensiunile
	allocimg(x);	//alocam imaginea
	citirecom(f, "%d%c", &x->vmax, &tmp);		//citim valoarea maxima
	for (int i = 0; i < x->dimy; ++i) {
		for (int j = 0; j < x->dimx; ++j) {
			if (x->tip <= 3) {	//pentru tipul ascii
				int r, g, b;
				fscanf(f, "%d", &r);	//citim valoarea pentru pixeli rosii
				if (x->tip == 3)	//daca imaginea e color citim si celelalte
					fscanf(f, "%d %d", &g, &b);
				x->m[i][j].r = r;
				x->m[i][j].g = g;
				x->m[i][j].b = b;
			} else {	//pentru binar
				fread(&x->m[i][j].r, 1, 1, f);
				if (x->tip == 6)
					fread(&x->m[i][j].g, 1, 1, f),
					fread(&x->m[i][j].b, 1, 1, f);
			}
		}
	}
	fclose(f);
	printf("Loaded %s\n", numef);
	return 1;
}

int scriere(img x, char *numef, int ascii)
{
	if (x.tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	FILE *f = fopen(numef, "w");
	if (!f) {
		printf("Eroare scriere\n");
		return 0;
	}
	//determinam tipul imaginii salvate
	if (x.tip > 3 && ascii)
		x.tip -= 3;
	if (x.tip <= 3 && !ascii)
		x.tip += 3;

	fprintf(f, "P%d\n", x.tip);
	fprintf(f, "%d %d\n", x.dimx, x.dimy);
	fprintf(f, "%d\n", x.vmax);

	for (int i = 0; i < x.dimy; ++i) {
		for (int j = 0; j < x.dimx; ++j) {
			if (ascii) {//ascii = 1 pentru imaginile ce trebuie salvate ascii
				fprintf(f, "%d ", x.m[i][j].r);
				if (x.tip == 3)
					fprintf(f, "%d %d ", x.m[i][j].g, x.m[i][j].b);

			} else {//ascii = 0 pentru imaginile ce trebuie salvate binar
				fwrite(&x.m[i][j].r, 1, 1, f);
				if (x.tip == 6)
					fwrite(&x.m[i][j].g, 1, 1, f),
					fwrite(&x.m[i][j].b, 1, 1, f);
			}
		}
		if (ascii)//pentru imaginile ascii printram \n dupa fiecare linie
			fprintf(f, "\n");
	}
	fclose(f);
	printf("Saved %s\n", numef);
	return 1;
}

int graysc(img x, selectie s)
{
	if (x.tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	if (x.tip % 3 != 0) {
		printf("Grayscale filter not available\n");
		return 0;
	}
	for (int i = s.y1; i < s.y2; ++i) {
		for (int j = s.x1; j < s.x2; ++j) {
			//calculam noile valori ale pixelilor
			int medie = (x.m[i][j].r + x.m[i][j].g + x.m[i][j].b + 1.5) / 3;
			x.m[i][j].r = medie;
			x.m[i][j].g = medie;
			x.m[i][j].b = medie;
		}
	}
	printf("Grayscale filter applied\n");
	return 1;
}

int sepia(img x, selectie s)
{
	if (x.tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	if (x.tip % 3 != 0) {
		printf("Sepia filter not available\n");
		return 0;
	}
	for (int i = s.y1; i < s.y2; ++i) {
		for (int j = s.x1; j < s.x2; ++j) {
			pixel t = x.m[i][j];
			pixel nou;
			//calculam noile valori ale pixelilor
			nou.r = min(0.393 * t.r + 0.769 * t.g + 0.189 * t.b + 0.5, x.vmax);
			nou.g = min(0.349 * t.r + 0.686 * t.g + 0.168 * t.b + 0.5, x.vmax);
			nou.b = min(0.272 * t.r + 0.534 * t.g + 0.131 * t.b + 0.5, x.vmax);
			x.m[i][j] = nou;
		}
	}
	printf("Sepia filter applied\n");
	return 1;
}

int rotate(img *x, selectie s, char *arg)
{
	int unghi, n;
	//verificam daca a fost specificat unghiul
	if (sscanf(arg, "%d", &unghi) == 0)
		printf("Invalid command\n");
	if (x->tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	if (unghi % 90 != 0) {	//testam daca unghiul e divizibil cu 90
		printf("Unsupported rotation angle\n");
		return 0;
	}
	if (rotateinvalid(s, *x)) {	//testam daca selectia e valida pentru rotate
		printf("The selection must be square\n");
		return 0;
	}

	//calculam numarul de rotiri la dreapta necesare
	n = unghi / 90;
	if (n < 0)
		n += 4;

	//efectuam rotirile la dreapta
	for (int i = 0; i < n; i++) {
		rotire(x, s);
		//selectam intreaga imagine daca rotirea nu a fost patrata
		if (s.x2 > x->dimx || s.y2 > x->dimy)
			s.x2 = x->dimx,
			s.y2 = x->dimy;
	}
	printf("Rotated %d\n", unghi);
	return 1;
}

int crop(img *x, selectie s)
{
	img tmp;
	tmp.tip = x->tip;
	if (tmp.tip == 0) {
		printf("No image loaded\n");
		return 0;
	}

	//construim noua imagine
	tmp.vmax = x->vmax;
	tmp.dimx = s.x2 - s.x1;
	tmp.dimy = s.y2 - s.y1;
	allocimg(&tmp);

	for (int i = s.y1; i < s.y2; ++i)
		for (int j = s.x1; j < s.x2; ++j)
			tmp.m[i - s.y1][j - s.x1] = x->m[i][j];	//salvam pixelii

	stergereimg(*x);	//stergem imaginea completa
	*x = tmp;			//o inlocuim cu cea decupata
	printf("Image cropped\n");
	return 1;
}

int selectd(selectie *t, img x, char *linie)
{
	selectie s;
	char tmp[100], check;
	if (x.tip == 0) {
		printf("No image loaded\n");
		return 0;
	}

	//parsam coordonatele si verificam daca toate au fost citite
	check = sscanf(linie, " %s %d %d %d %d", tmp, &s.x1, &s.y1, &s.x2, &s.y2);
	if (check != 5) {
		printf("Invalid command\n");
		return 0;
	}

	//interschimbam coordonatele daca e necesar
	if (s.x1 > s.x2)
		swap(&s.x1, &s.x2);
	if (s.y1 > s.y2)
		swap(&s.y1, &s.y2);

	//verificam daca selectia este valida
	if (sinvalida(s, x)) {
		printf("Invalid set of coordinates\n");
		return 0;
	}
	printf("Selected %d %d %d %d\n", s.x1, s.y1, s.x2, s.y2);
	*t = s;
	return 1;
}

int selectall(selectie *s, img x)
{
	if (x.tip == 0) {
		printf("No image loaded\n");
		return 0;
	}
	//selectam intreaga imagine
	s->x1 = 0;
	s->y1 = 0;
	s->x2 = x.dimx;
	s->y2 = x.dimy;
	return 1;
}
