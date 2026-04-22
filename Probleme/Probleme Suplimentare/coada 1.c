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

typedef struct NodD NodD;
struct NodD {
	Masina info;
	NodD* next;
	NodD* prev;
};

typedef struct ListaD ListaD;
struct ListaD {
	NodD* first;
	NodD* last;
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

void enqueue(ListaD* lista, Masina masina) {
	NodD* nodNou = (NodD*)malloc(sizeof(NodD));
	nodNou->info = masina;
	nodNou->prev = NULL;
	nodNou->next = lista->first;
	if (lista->first) {
		lista->first->prev = nodNou;
	}
	else {
		lista->last = nodNou;
	}
	lista->first = nodNou;
}

Masina dequeue(ListaD* lista) {
	Masina rezultat;
	rezultat.id = -1;
	if (lista->last) {
		NodD* aux = lista->last;
		rezultat = aux->info;
		lista->last = lista->last->prev;
		if (lista->last) {
			lista->last->next = NULL;
		}
		else {
			lista->first = NULL;
		}
		free(aux);
	}
	return rezultat;
}

float calculeazaPretTotalSofer(ListaD* coada, const char* numeSofer) {
	ListaD coadaNoua;
	coadaNoua.first = NULL;
	coadaNoua.last = NULL;
	float suma = 0;
	while (coada->last) {
		Masina m = dequeue(coada);
		if (strcmp(m.numeSofer, numeSofer) == 0) {
			suma += m.pret;
		}
		enqueue(&coadaNoua, m);
	}
	while (coadaNoua.last) {
		enqueue(coada, dequeue(&coadaNoua));
	}
	return suma;
}

void dezalocareCoada(ListaD* coada) {
	while (coada->last) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
}

int main() {
	ListaD coada;
	coada.first = NULL;
	coada.last = NULL;

	enqueue(&coada, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	enqueue(&coada, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	enqueue(&coada, initializareMasina(3, 5, 18000, "Dacia", "Ion", 'A'));

	printf("Pret total masini Ion: %.2f\n", calculeazaPretTotalSofer(&coada, "Ion"));

	dezalocareCoada(&coada);
	return 0;
}
