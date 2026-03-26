#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

Masina initializareMasina(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
	strcpy(m.numeSofer, numeSofer);
	m.serie = serie;
	return m;
}

Masina copiazaMasina(Masina m) {
	return initializareMasina(m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++) {
		temp[i] = (*masini)[i];
	}
	temp[*nrMasini] = masinaNoua;
	free(*masini);
	*masini = temp;
	(*nrMasini)++;
}

Masina* copiazaMasiniPestePret(Masina* masini, int nrMasini, float pretMinim, int* nrCopiate) {
	Masina* rezultat = NULL;
	*nrCopiate = 0;
	for (int i = 0; i < nrMasini; i++) {
		if (masini[i].pret > pretMinim) {
			adaugaMasinaInVector(&rezultat, nrCopiate, copiazaMasina(masini[i]));
		}
	}
	return rezultat;
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void dezalocareVectorMasini(Masina** masini, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++) {
		free((*masini)[i].model);
		free((*masini)[i].numeSofer);
	}
	free(*masini);
	*masini = NULL;
	*nrMasini = 0;
}

int main() {
	Masina* masini = NULL;
	int nrMasini = 0;
	adaugaMasinaInVector(&masini, &nrMasini, initializareMasina(1, 4, 7000, "Logan", "Ion", 'A'));
	adaugaMasinaInVector(&masini, &nrMasini, initializareMasina(2, 2, 15000, "BMW", "Ana", 'B'));
	adaugaMasinaInVector(&masini, &nrMasini, initializareMasina(3, 4, 22000, "Audi", "Maria", 'A'));

	int nrCopiate = 0;
	Masina* copiate = copiazaMasiniPestePret(masini, nrMasini, 10000, &nrCopiate);
	afisareVectorMasini(copiate, nrCopiate);

	dezalocareVectorMasini(&copiate, &nrCopiate);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}
