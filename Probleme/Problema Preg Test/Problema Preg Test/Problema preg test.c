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

typedef struct NodSimplu NodSimplu;
struct NodSimplu {
	Comanda info;
	NodSimplu* next;
};

Comanda initializareComanda(
	unsigned int idComanda,
	const char* dataLansare,
	const char* dataLivrareProgramata,
	const char* numeClient,
	char statusComanda
) {
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

Comanda copiazaComanda(Comanda c) {
	return initializareComanda(
		c.idComanda,
		c.dataLansare,
		c.dataLivrareProgramata,
		c.numeClient,
		c.statusComanda
	);
}

void afisareComanda(Comanda c) {
	printf("Id comanda: %u\n", c.idComanda);
	printf("Data lansare: %s\n", c.dataLansare);
	printf("Data livrare programata: %s\n", c.dataLivrareProgramata);
	printf("Nume client: %s\n", c.numeClient);
	printf("Status comanda: %c\n\n", c.statusComanda);
}

void dezalocareComanda(Comanda* c) {
	free(c->dataLansare);
	free(c->dataLivrareProgramata);
	free(c->numeClient);

	c->dataLansare = NULL;
	c->dataLivrareProgramata = NULL;
	c->numeClient = NULL;
}

void adaugaComandaInListaDubla(ListaDubla* lista, Comanda comandaNoua) {
	NodDublu* nodNou = (NodDublu*)malloc(sizeof(NodDublu));

	nodNou->info = comandaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->ultim;

	if (lista->ultim != NULL) {
		lista->ultim->next = nodNou;
	}
	else {
		lista->prim = nodNou;
	}

	lista->ultim = nodNou;
}

void afisareListaDublaInainte(ListaDubla lista) {
	NodDublu* p = lista.prim;

	while (p != NULL) {
		afisareComanda(p->info);
		p = p->next;
	}
}

void afisareListaDublaInapoi(ListaDubla lista) {
	NodDublu* p = lista.ultim;

	while (p != NULL) {
		afisareComanda(p->info);
		p = p->prev;
	}
}

void dezalocareListaDubla(ListaDubla* lista) {
	NodDublu* p = lista->prim;

	while (p != NULL) {
		NodDublu* temp = p;
		p = p->next;

		dezalocareComanda(&temp->info);
		free(temp);
	}

	lista->prim = NULL;
	lista->ultim = NULL;
}

int numaraComenziClient(ListaDubla lista, const char* numeClient) {
	int contor = 0;
	NodDublu* p = lista.prim;

	while (p != NULL) {
		if (strcmp(p->info.numeClient, numeClient) == 0) {
			contor++;
		}

		p = p->next;
	}

	return contor;
}

void modificaDataLivrarePentruStatusC(ListaDubla* lista, const char* dataNoua) {
	NodDublu* p = lista->prim;

	while (p != NULL) {
		if (p->info.statusComanda == 'C') {
			free(p->info.dataLivrareProgramata);

			p->info.dataLivrareProgramata = (char*)malloc(strlen(dataNoua) + 1);
			strcpy(p->info.dataLivrareProgramata, dataNoua);
		}

		p = p->next;
	}
}

void adaugaComandaInListaSimpla(NodSimplu** cap, Comanda comandaNoua) {
	NodSimplu* nodNou = (NodSimplu*)malloc(sizeof(NodSimplu));

	nodNou->info = comandaNoua;
	nodNou->next = NULL;

	if (*cap == NULL) {
		*cap = nodNou;
	}
	else {
		NodSimplu* p = *cap;

		while (p->next != NULL) {
			p = p->next;
		}

		p->next = nodNou;
	}
}

NodSimplu* salveazaComenziDupaStatus(ListaDubla lista, char statusCautat) {
	NodSimplu* listaSimpla = NULL;
	NodDublu* p = lista.prim;

	while (p != NULL) {
		if (p->info.statusComanda == statusCautat) {
			Comanda copie = copiazaComanda(p->info);
			adaugaComandaInListaSimpla(&listaSimpla, copie);
		}

		p = p->next;
	}

	return listaSimpla;
}

void afisareListaSimpla(NodSimplu* cap) {
	while (cap != NULL) {
		afisareComanda(cap->info);
		cap = cap->next;
	}
}

void dezalocareListaSimpla(NodSimplu** cap) {
	while (*cap != NULL) {
		NodSimplu* temp = *cap;
		*cap = (*cap)->next;

		dezalocareComanda(&temp->info);
		free(temp);
	}
}

int main() {
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;

	adaugaComandaInListaDubla(&lista, initializareComanda(1, "10.04.2026", "15.04.2026", "Ion Popescu", 'C'));
	adaugaComandaInListaDubla(&lista, initializareComanda(2, "11.04.2026", "17.04.2026", "Ana Ionescu", 'L'));
	adaugaComandaInListaDubla(&lista, initializareComanda(3, "12.04.2026", "18.04.2026", "Ion Popescu", 'C'));
	adaugaComandaInListaDubla(&lista, initializareComanda(4, "13.04.2026", "20.04.2026", "Maria Stan", 'A'));
	adaugaComandaInListaDubla(&lista, initializareComanda(5, "14.04.2026", "22.04.2026", "Ion Popescu", 'L'));

	printf("Lista initiala - parcurgere inainte:\n\n");
	afisareListaDublaInainte(lista);

	printf("Lista initiala - parcurgere inapoi:\n\n");
	afisareListaDublaInapoi(lista);

	int nrComenzi = numaraComenziClient(lista, "Ion Popescu");
	printf("Numar comenzi pentru Ion Popescu: %d\n\n", nrComenzi);

	modificaDataLivrarePentruStatusC(&lista, "30.04.2026");

	printf("Lista dupa modificarea comenzilor cu status C - parcurgere inainte:\n\n");
	afisareListaDublaInainte(lista);

	printf("Lista dupa modificarea comenzilor cu status C - parcurgere inapoi:\n\n");
	afisareListaDublaInapoi(lista);

	NodSimplu* listaSimpla = salveazaComenziDupaStatus(lista, 'L');

	printf("Lista simpla cu comenzile care au status L:\n\n");
	afisareListaSimpla(listaSimpla);

	dezalocareListaSimpla(&listaSimpla);
	dezalocareListaDubla(&lista);

	return 0;
}
