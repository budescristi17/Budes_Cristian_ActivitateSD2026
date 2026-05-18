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

typedef struct NodSimplu NodSimplu;
struct NodSimplu {
	Comanda info;
	NodSimplu* next;
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

void adaugaComandaInLista(NodSimplu** cap, Comanda comandaNoua) {
	NodSimplu* nodNou = (NodSimplu*)malloc(sizeof(NodSimplu));
	nodNou->info = comandaNoua;
	nodNou->next = NULL;
	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		NodSimplu* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
}

int numaraComenziClientStatus(NodSimplu* cap, const char* numeClient, char status) {
	int contor = 0;
	while (cap) {
		if (strcmp(cap->info.numeClient, numeClient) == 0 && cap->info.statusComanda == status) {
			contor++;
		}
		cap = cap->next;
	}
	return contor;
}

void afisareLista(NodSimplu* cap) {
	while (cap) {
		afisareComanda(cap->info);
		cap = cap->next;
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

void dezalocareLista(NodSimplu** cap) {
	while (*cap) {
		NodSimplu* temp = *cap;
		*cap = (*cap)->next;
		dezalocareComanda(&temp->info);
		free(temp);
	}
}

int main() {
	NodSimplu* lista = NULL;
	adaugaComandaInLista(&lista, initializareComanda(1, "10.04.2026", "15.04.2026", "Ion Popescu", 'C'));
	adaugaComandaInLista(&lista, initializareComanda(2, "11.04.2026", "17.04.2026", "Ana Ionescu", 'L'));
	adaugaComandaInLista(&lista, initializareComanda(3, "12.04.2026", "18.04.2026", "Ion Popescu", 'C'));

	afisareLista(lista);
	printf("Comenzi finalizate Ion Popescu: %d\n", numaraComenziClientStatus(lista, "Ion Popescu", 'C'));

	dezalocareLista(&lista);
	return 0;
}
