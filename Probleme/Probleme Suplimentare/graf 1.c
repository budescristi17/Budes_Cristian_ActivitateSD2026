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

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Masina info;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodS* next;
	NodP* info;
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

void inserareInListaP(NodP** graf, Masina masinaNoua) {
	NodP* nou = (NodP*)malloc(sizeof(NodP));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*graf) {
		NodP* aux = *graf;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*graf = nou;
	}
}

void inserareInListaS(NodS** listaS, NodP* vecin) {
	NodS* nou = (NodS*)malloc(sizeof(NodS));
	nou->info = vecin;
	nou->next = NULL;
	if (*listaS) {
		NodS* aux = *listaS;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
	}
	else {
		*listaS = nou;
	}
}

NodP* cautaNodDupaID(NodP* graf, int id) {
	while (graf) {
		if (graf->info.id == id) {
			return graf;
		}
		graf = graf->next;
	}
	return NULL;
}

void inserareMuchie(NodP* graf, int idStart, int idStop) {
	NodP* start = cautaNodDupaID(graf, idStart);
	NodP* stop = cautaNodDupaID(graf, idStop);
	if (start && stop) {
		inserareInListaS(&start->vecini, stop);
		inserareInListaS(&stop->vecini, start);
	}
}

int numaraVecini(NodP* graf, int id) {
	NodP* nod = cautaNodDupaID(graf, id);
	int contor = 0;
	if (nod) {
		NodS* vecin = nod->vecini;
		while (vecin) {
			contor++;
			vecin = vecin->next;
		}
	}
	return contor;
}

void dezalocareGraf(NodP** graf) {
	while (*graf) {
		NodP* temp = *graf;
		*graf = (*graf)->next;
		while (temp->vecini) {
			NodS* vecin = temp->vecini;
			temp->vecini = temp->vecini->next;
			free(vecin);
		}
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
}

int main() {
	NodP* graf = NULL;
	inserareInListaP(&graf, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	inserareInListaP(&graf, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	inserareInListaP(&graf, initializareMasina(3, 5, 18000, "Dacia", "Maria", 'C'));

	inserareMuchie(graf, 1, 2);
	inserareMuchie(graf, 1, 3);

	printf("Numar vecini pentru id 1: %d\n", numaraVecini(graf, 1));

	dezalocareGraf(&graf);
	return 0;
}
