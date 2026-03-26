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

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void ordoneazaDupaNumarUsi(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini - 1; i++) {
		for (int j = i + 1; j < nrMasini; j++) {
			if (masini[i].nrUsi > masini[j].nrUsi) {
				Masina aux = masini[i];
				masini[i] = masini[j];
				masini[j] = aux;
			}
		}
	}
}

float calculeazaPretMediuSerie(Masina* masini, int nrMasini, unsigned char serie) {
	float suma = 0;
	int contor = 0;
	for (int i = 0; i < nrMasini; i++) {
		if (masini[i].serie == serie) {
			suma += masini[i].pret;
			contor++;
		}
	}
	if (contor == 0) {
		return 0;
	}
	return suma / contor;
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
	int nrMasini = 4;
	Masina* masini = (Masina*)malloc(sizeof(Masina) * nrMasini);
	masini[0] = initializareMasina(1, 4, 12000, "Skoda", "Vlad", 'A');
	masini[1] = initializareMasina(2, 2, 18000, "Mini", "Ioana", 'B');
	masini[2] = initializareMasina(3, 5, 26000, "Dacia", "Matei", 'A');
	masini[3] = initializareMasina(4, 3, 9000, "Hyundai", "Elena", 'C');

	ordoneazaDupaNumarUsi(masini, nrMasini);
	afisareVectorMasini(masini, nrMasini);
	printf("Pret mediu seria A: %.2f\n", calculeazaPretMediuSerie(masini, nrMasini, 'A'));

	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}
