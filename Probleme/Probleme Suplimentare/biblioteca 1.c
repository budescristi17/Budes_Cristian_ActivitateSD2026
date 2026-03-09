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
	printf("id: %d, nr carti: %d, nume: %s, suprafata: %.2f, sector: %c\n", b.id, b.nrCarti, b.nume, b.suprafata, b.sector);
}

void afisareVector(struct Biblioteca* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

void copiazaBiblioteciDinSector(struct Biblioteca* vector, int nrElemente, char sector, struct Biblioteca** rezultat, int* nrRezultat) {
	*nrRezultat = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].sector == sector) {
			(*nrRezultat)++;
		}
	}

	*rezultat = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (*nrRezultat));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].sector == sector) {
			(*rezultat)[k] = copiazaBiblioteca(vector[i]);
			k++;
		}
	}
}

float calculeazaSuprafataMedie(struct Biblioteca* vector, int nrElemente) {
	float suma = 0;
	for (int i = 0; i < nrElemente; i++) {
		suma += vector[i].suprafata;
	}
	if (nrElemente == 0) {
		return 0;
	}
	return suma / nrElemente;
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
	int nrBiblioteci = 5;
	struct Biblioteca* biblioteci = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrBiblioteci);
	biblioteci[0] = initializare(1, 3000, "Centrala", 450, '1');
	biblioteci[1] = initializare(2, 1500, "ASE", 180, '2');
	biblioteci[2] = initializare(3, 2200, "Nord", 260, '1');
	biblioteci[3] = initializare(4, 900, "Sud", 120, '3');
	biblioteci[4] = initializare(5, 3100, "Est", 300, '1');

	struct Biblioteca* selectie = NULL;
	int nrSelectie = 0;
	copiazaBiblioteciDinSector(biblioteci, nrBiblioteci, '1', &selectie, &nrSelectie);
	afisareVector(selectie, nrSelectie);
	printf("Suprafata medie selectie: %.2f\n", calculeazaSuprafataMedie(selectie, nrSelectie));

	dezalocare(&selectie, &nrSelectie);
	dezalocare(&biblioteci, &nrBiblioteci);
	return 0;
}
