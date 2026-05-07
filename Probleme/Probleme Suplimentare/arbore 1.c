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

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* stanga;
	Nod* dreapta;
};

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

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		}
		else if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*radacina = nou;
	}
}

void afisareMasiniInterval(Nod* radacina, int idMinim, int idMaxim) {
	if (radacina) {
		if (radacina->info.id > idMinim) {
			afisareMasiniInterval(radacina->stanga, idMinim, idMaxim);
		}
		if (radacina->info.id >= idMinim && radacina->info.id <= idMaxim) {
			afisareMasina(radacina->info);
		}
		if (radacina->info.id < idMaxim) {
			afisareMasiniInterval(radacina->dreapta, idMinim, idMaxim);
		}
	}
}

void dezalocareArbore(Nod** radacina) {
	if (*radacina) {
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* radacina = NULL;
	adaugaMasinaInArbore(&radacina, initializareMasina(5, 4, 12000, "Skoda", "Vlad", 'A'));
	adaugaMasinaInArbore(&radacina, initializareMasina(2, 2, 18000, "Mini", "Ioana", 'B'));
	adaugaMasinaInArbore(&radacina, initializareMasina(8, 5, 26000, "Dacia", "Matei", 'A'));
	adaugaMasinaInArbore(&radacina, initializareMasina(6, 3, 9000, "Hyundai", "Elena", 'C'));

	afisareMasiniInterval(radacina, 3, 7);

	dezalocareArbore(&radacina);
	return 0;
}
