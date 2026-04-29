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

void inserareMasinaInTabela(HashTable ht, Masina masina) {
	int pozitie = calculeazaHash(masina.id, ht.dim);
	adaugaMasinaInLista(&ht.vector[pozitie], masina);
}

float calculeazaPretMediuCluster(HashTable ht, int pozitie) {
	float suma = 0;
	int contor = 0;
	if (pozitie >= 0 && pozitie < ht.dim) {
		Nod* p = ht.vector[pozitie];
		while (p) {
			suma += p->info.pret;
			contor++;
			p = p->next;
		}
	}
	if (contor == 0) {
		return 0;
	}
	return suma / contor;
}

void afiseazaPreturiMedii(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d: %.2f\n", i, calculeazaPretMediuCluster(ht, i));
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
	HashTable ht = initializareHashTable(4);
	inserareMasinaInTabela(ht, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	inserareMasinaInTabela(ht, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	inserareMasinaInTabela(ht, initializareMasina(5, 5, 18000, "Dacia", "Maria", 'A'));
	inserareMasinaInTabela(ht, initializareMasina(8, 4, 16000, "Ford", "Vlad", 'C'));

	afiseazaPreturiMedii(ht);

	dezalocareTabela(&ht);
	return 0;
}
