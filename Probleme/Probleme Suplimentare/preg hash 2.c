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

typedef struct Nod Nod;
struct Nod {
	Comanda info;
	Nod* next;
};

typedef struct HashTable HashTable;
struct HashTable {
	int dim;
	Nod** vector;
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

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(unsigned int idComanda, int dimensiune) {
	return (idComanda * 7) % dimensiune;
}

void adaugaComandaInLista(Nod** lista, Comanda comandaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = comandaNoua;
	nodNou->next = *lista;
	*lista = nodNou;
}

void inserareComandaInTabela(HashTable ht, Comanda comanda) {
	int pozitie = calculeazaHash(comanda.idComanda, ht.dim);
	adaugaComandaInLista(&ht.vector[pozitie], comanda);
}

void dezalocareComanda(Comanda* c) {
	free(c->dataLansare);
	free(c->dataLivrareProgramata);
	free(c->numeClient);
	c->dataLansare = NULL;
	c->dataLivrareProgramata = NULL;
	c->numeClient = NULL;
}

void stergeComenziDupaStatus(HashTable ht, char status) {
	for (int i = 0; i < ht.dim; i++) {
		Nod** p = &ht.vector[i];
		while (*p) {
			if ((*p)->info.statusComanda == status) {
				Nod* temp = *p;
				*p = (*p)->next;
				dezalocareComanda(&temp->info);
				free(temp);
			}
			else {
				p = &(*p)->next;
			}
		}
	}
}

void afisareTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d\n", i);
		Nod* p = ht.vector[i];
		while (p) {
			afisareComanda(p->info);
			p = p->next;
		}
	}
}

void dezalocareTabela(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		while (ht->vector[i]) {
			Nod* temp = ht->vector[i];
			ht->vector[i] = ht->vector[i]->next;
			dezalocareComanda(&temp->info);
			free(temp);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

int main() {
	HashTable ht = initializareHashTable(5);
	inserareComandaInTabela(ht, initializareComanda(1, "10.04.2026", "15.04.2026", "Ion Popescu", 'C'));
	inserareComandaInTabela(ht, initializareComanda(2, "11.04.2026", "17.04.2026", "Ana Ionescu", 'L'));
	inserareComandaInTabela(ht, initializareComanda(3, "12.04.2026", "18.04.2026", "Maria Stan", 'C'));

	stergeComenziDupaStatus(ht, 'C');
	afisareTabela(ht);

	dezalocareTabela(&ht);
	return 0;
}
