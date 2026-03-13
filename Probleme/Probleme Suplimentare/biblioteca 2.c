#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Biblioteca {
	int id;
	int nrCarti;
	char* nume;
	float suprafata;
	char sector;
};

struct Biblioteca initializare(int id, int nrCarti, const char* nume, float suprafata, char sector) {
	struct Biblioteca b;
	b.id = id;
	b.nrCarti = nrCarti;
	b.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(b.nume, nume);
	b.suprafata = suprafata;
	b.sector = sector;
	return b;
}

struct Biblioteca copiazaBiblioteca(struct Biblioteca b) {
	return initializare(b.id, b.nrCarti, b.nume, b.suprafata, b.sector);
}

void afisare(struct Biblioteca b) {
	printf("id: %d, carti: %d, nume: %s, suprafata: %.2f, sector: %c\n", b.id, b.nrCarti, b.nume, b.suprafata, b.sector);
}

void adaugaBibliotecaInVector(struct Biblioteca** vector, int* nrElemente, struct Biblioteca bibliotecaNoua) {
	struct Biblioteca* temp = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * ((*nrElemente) + 1));
	for (int i = 0; i < *nrElemente; i++) {
		temp[i] = (*vector)[i];
	}
	temp[*nrElemente] = bibliotecaNoua;
	free(*vector);
	*vector = temp;
	(*nrElemente)++;
}

struct Biblioteca* copiazaBiblioteciCuMulteCarti(struct Biblioteca* vector, int nrElemente, int pragCarti, int* nrCopiate) {
	struct Biblioteca* copie = NULL;
	*nrCopiate = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].nrCarti >= pragCarti) {
			adaugaBibliotecaInVector(&copie, nrCopiate, copiazaBiblioteca(vector[i]));
		}
	}
	return copie;
}

void afisareVector(struct Biblioteca* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

void dezalocare(struct Biblioteca** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].nume);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

int main() {
	int nrBiblioteci = 0;
	struct Biblioteca* biblioteci = NULL;
	adaugaBibliotecaInVector(&biblioteci, &nrBiblioteci, initializare(1, 1200, "Luceafarul", 140, '2'));
	adaugaBibliotecaInVector(&biblioteci, &nrBiblioteci, initializare(2, 2500, "Universitara", 300, '1'));
	adaugaBibliotecaInVector(&biblioteci, &nrBiblioteci, initializare(3, 800, "Cartier", 90, '3'));
	adaugaBibliotecaInVector(&biblioteci, &nrBiblioteci, initializare(4, 4000, "Nationala", 700, '1'));

	int nrCopiate = 0;
	struct Biblioteca* copiate = copiazaBiblioteciCuMulteCarti(biblioteci, nrBiblioteci, 2000, &nrCopiate);
	afisareVector(copiate, nrCopiate);

	dezalocare(&copiate, &nrCopiate);
	dezalocare(&biblioteci, &nrBiblioteci);
	return 0;
}
