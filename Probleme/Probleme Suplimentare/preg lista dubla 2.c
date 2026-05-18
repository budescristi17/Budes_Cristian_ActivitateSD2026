#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraComanda {
	unsigned int idComanda;
	char* dataLansare;
	char* dataLivrareProgramata;
	char* numeClient;
	char statusComanda;
};
typedef struct StructuraComanda Comanda;

typedef struct NodDublu NodDublu;
struct NodDublu {
	Comanda info;
	NodDublu* next;
	NodDublu* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	NodDublu* prim;
	NodDublu* ultim;
};

Comanda initializareComanda(unsigned int idComanda, const char* dataLansare, const char* dataLivrareProgramata, const char* numeClient, char statusComanda) {
	Comanda c;
	c.idComanda = idComanda;
	c.dataLansare = (char*)malloc(strlen(dataLansare) + 1);
	strcpy(c.dataLansare, dataLansare);
	c.dataLivrareProgramata = (char*)malloc(strlen(dataLivrareProgramata) + 1);
	strcpy(c.dataLivrareProgramata, dataLivrareProgramata);
	c.numeClient = (char*)malloc(strlen(numeClient) + 1);
	strcpy(c.numeClient, numeClient);
	c.statusComanda = statusComanda;
	return c;
}

void afisareComanda(Comanda c) {
	printf("Id comanda: %u\n", c.idComanda);
	printf("Data lansare: %s\n", c.dataLansare);
	printf("Data livrare programata: %s\n", c.dataLivrareProgramata);
	printf("Nume client: %s\n", c.numeClient);
	printf("Status comanda: %c\n\n", c.statusComanda);
}

void adaugaComandaInListaDubla(ListaDubla* lista, Comanda comandaNoua) {
	NodDublu* nodNou = (NodDublu*)malloc(sizeof(NodDublu));
	nodNou->info = comandaNoua;
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

void mutaPrimaComandaCuStatusLaFinal(ListaDubla* lista, char status) {
	NodDublu* p = lista->prim;
	while (p && p->info.statusComanda != status) {
		p = p->next;
	}
	if (p == NULL || p == lista->ultim) {
		return;
	}
	if (p->prev) {
		p->prev->next = p->next;
	}
	else {
		lista->prim = p->next;
	}
	if (p->next) {
		p->next->prev = p->prev;
	}
	p->next = NULL;
	p->prev = lista->ultim;
	lista->ultim->next = p;
	lista->ultim = p;
}

void afisareLista(ListaDubla lista) {
	NodDublu* p = lista.prim;
	while (p) {
		afisareComanda(p->info);
		p = p->next;
	}
}

void dezalocareComanda(Comanda* c) {
	free(c->dataLansare);
	free(c->dataLivrareProgramata);
	free(c->numeClient);
	c->dataLansare = NULL;
	c->dataLivrareProgramata = NULL;
	c->numeClient = NULL;
}

void dezalocareListaDubla(ListaDubla* lista) {
	while (lista->prim) {
		NodDublu* temp = lista->prim;
		lista->prim = lista->prim->next;
		dezalocareComanda(&temp->info);
		free(temp);
	}
	lista->ultim = NULL;
}

int main() {
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	adaugaComandaInListaDubla(&lista, initializareComanda(1, "10.04.2026", "15.04.2026", "Ion Popescu", 'C'));
	adaugaComandaInListaDubla(&lista, initializareComanda(2, "11.04.2026", "17.04.2026", "Ana Ionescu", 'L'));
	adaugaComandaInListaDubla(&lista, initializareComanda(3, "12.04.2026", "18.04.2026", "Maria Stan", 'C'));

	mutaPrimaComandaCuStatusLaFinal(&lista, 'C');
	afisareLista(lista);

	dezalocareListaDubla(&lista);
	return 0;
}
