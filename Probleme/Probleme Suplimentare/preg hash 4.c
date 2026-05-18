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

typedef struct NodSimplu NodSimplu;
struct NodSimplu {
	Comanda info;
	NodSimplu* next;
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

Comanda copiazaComanda(Comanda c) {
	return initializareComanda(c.idComanda, c.dataLansare, c.dataLivrareProgramata, c.numeClient, c.statusComanda);
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

void adaugaComandaInListaHash(Nod** lista, Comanda comandaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = comandaNoua;
	nodNou->next = *lista;
	*lista = nodNou;
}

void adaugaComandaInListaSimpla(NodSimplu** lista, Comanda comandaNoua) {
	NodSimplu* nodNou = (NodSimplu*)malloc(sizeof(NodSimplu));
	nodNou->info = comandaNoua;
	nodNou->next = NULL;
	if (*lista == NULL) {
		*lista = nodNou;
	}
	else {
		NodSimplu* p = *lista;
		while (p->next) {
			p = p->next;
		}
		p->next = nodNou;
	}
}

void inserareComandaInTabela(HashTable ht, Comanda comanda) {
	int pozitie = calculeazaHash(comanda.idComanda, ht.dim);
	adaugaComandaInListaHash(&ht.vector[pozitie], comanda);
}

NodSimplu* copiazaComenziDupaStatus(HashTable ht, char status) {
	NodSimplu* rezultat = NULL;
	for (int i = 0; i < ht.dim; i++) {
		Nod* p = ht.vector[i];
		while (p) {
			if (p->info.statusComanda == status) {
				adaugaComandaInListaSimpla(&rezultat, copiazaComanda(p->info));
			}
			p = p->next;
		}
	}
	return rezultat;
}

void afisareListaSimpla(NodSimplu* lista) {
	while (lista) {
		afisareComanda(lista->info);
		lista = lista->next;
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

void dezalocareListaSimpla(NodSimplu** lista) {
	while (*lista) {
		NodSimplu* temp = *lista;
		*lista = (*lista)->next;
		dezalocareComanda(&temp->info);
		free(temp);
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

	NodSimplu* lista = copiazaComenziDupaStatus(ht, 'C');
	afisareListaSimpla(lista);

	dezalocareListaSimpla(&lista);
	dezalocareTabela(&ht);
	return 0;
}
