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
	Nod* next;
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

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
}

Nod* copiazaMasinileUnuiSofer(Nod* cap, const char* numeSofer) {
	Nod* rezultat = NULL;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			adaugaMasinaInLista(&rezultat, copiazaMasina(cap->info));
		}
		cap = cap->next;
	}
	return rezultat;
}

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void dezalocareListaMasini(Nod** cap) {
	while (*cap) {
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
}

int main() {
	Nod* lista = NULL;
	adaugaMasinaInLista(&lista, initializareMasina(1, 4, 10000, "Logan", "Ion", 'A'));
	adaugaMasinaInLista(&lista, initializareMasina(2, 2, 16000, "BMW", "Ana", 'B'));
	adaugaMasinaInLista(&lista, initializareMasina(3, 4, 18000, "Seat", "Ion", 'C'));

	Nod* rezultat = copiazaMasinileUnuiSofer(lista, "Ion");
	afisareListaMasini(rezultat);

	dezalocareListaMasini(&rezultat);
	dezalocareListaMasini(&lista);
	return 0;
}
