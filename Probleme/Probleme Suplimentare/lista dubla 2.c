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
	Nod* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	Nod* prim;
	Nod* ultim;
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

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->ultim;
	if (lista->ultim) {
		lista->ultim->next = nodNou;
	}
	else {
		lista->prim = nodNou;
	}
	lista->ultim = nodNou;
}

void mutaMasinaLaInceput(ListaDubla* lista, int id) {
	Nod* p = lista->prim;
	while (p && p->info.id != id) {
		p = p->next;
	}
	if (p == NULL || p == lista->prim) {
		return;
	}
	if (p->prev) {
		p->prev->next = p->next;
	}
	if (p->next) {
		p->next->prev = p->prev;
	}
	else {
		lista->ultim = p->prev;
	}
	p->prev = NULL;
	p->next = lista->prim;
	lista->prim->prev = p;
	lista->prim = p;
}

void afisareLista(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareMasina(p->info);
		p = p->next;
	}
}

void dezalocareLista(ListaDubla* lista) {
	while (lista->prim) {
		Nod* temp = lista->prim;
		lista->prim = lista->prim->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	lista->ultim = NULL;
}

int main() {
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	adaugaMasinaInLista(&lista, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	adaugaMasinaInLista(&lista, initializareMasina(2, 2, 19000, "BMW", "Ana", 'B'));
	adaugaMasinaInLista(&lista, initializareMasina(3, 5, 23000, "Dacia", "Maria", 'A'));

	mutaMasinaLaInceput(&lista, 3);
	afisareLista(lista);

	dezalocareLista(&lista);
	return 0;
}
