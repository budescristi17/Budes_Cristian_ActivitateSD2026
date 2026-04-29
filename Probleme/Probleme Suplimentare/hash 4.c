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

typedef struct HashTable HashTable;
struct HashTable {
	int dim;
	Nod** vector;
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

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return (id * 5) % dimensiune;
}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	if (*lista == NULL) {
		*lista = nodNou;
	}
	else {
		Nod* p = *lista;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
}

void inserareMasinaInTabela(HashTable ht, Masina masina) {
	int pozitie = calculeazaHash(masina.id, ht.dim);
	adaugaMasinaInLista(&ht.vector[pozitie], masina);
}

Nod* copiazaMasiniDupaSerie(HashTable ht, unsigned char serie) {
	Nod* rezultat = NULL;
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vector[i];
		while (p) {
			if (p->info.serie == serie) {
				adaugaMasinaInLista(&rezultat, copiazaMasina(p->info));
			}
			p = p->next;
		}
	}
	return rezultat;
}

void afisareListaMasini(Nod* lista) {
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void dezalocareLista(Nod** lista) {
	while (*lista) {
		Nod* temp = *lista;
		*lista = (*lista)->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
}

void dezalocareTabela(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dezalocareLista(&ht->vector[i]);
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

int main() {
	HashTable ht = initializareHashTable(4);
	inserareMasinaInTabela(ht, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	inserareMasinaInTabela(ht, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	inserareMasinaInTabela(ht, initializareMasina(5, 5, 18000, "Dacia", "Maria", 'A'));
	inserareMasinaInTabela(ht, initializareMasina(8, 4, 16000, "Ford", "Vlad", 'C'));

	Nod* lista = copiazaMasiniDupaSerie(ht, 'A');
	afisareListaMasini(lista);

	dezalocareLista(&lista);
	dezalocareTabela(&ht);
	return 0;
}
