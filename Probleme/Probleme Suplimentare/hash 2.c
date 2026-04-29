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
	nodNou->next = *lista;
	*lista = nodNou;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.id, hash.dim);
	adaugaMasinaInLista(&hash.vector[pozitie], masina);
}

void stergeMasinaDinTabela(HashTable hash, int id) {
	int pozitie = calculeazaHash(id, hash.dim);
	Nod** p = &hash.vector[pozitie];
	while (*p) {
		if ((*p)->info.id == id) {
			Nod* temp = *p;
			*p = (*p)->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
			return;
		}
		p = &(*p)->next;
	}
}

void afisareTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d\n", i);
		Nod* p = ht.vector[i];
		while (p) {
			afisareMasina(p->info);
			p = p->next;
		}
	}
}

void dezalocareTabela(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		while (ht->vector[i]) {
			Nod* temp = ht->vector[i];
			ht->vector[i] = ht->vector[i]->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

int main() {
	HashTable ht = initializareHashTable(3);
	inserareMasinaInTabela(ht, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	inserareMasinaInTabela(ht, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	inserareMasinaInTabela(ht, initializareMasina(3, 5, 18000, "Dacia", "Maria", 'C'));

	stergeMasinaDinTabela(ht, 2);
	afisareTabela(ht);

	dezalocareTabela(&ht);
	return 0;
}
