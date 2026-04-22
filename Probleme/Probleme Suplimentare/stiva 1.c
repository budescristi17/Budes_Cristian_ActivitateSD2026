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

void pushStack(Nod** prim, Masina masina) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masina;
	nodNou->next = *prim;
	*prim = nodNou;
}

Masina popStack(Nod** prim) {
	Masina rezultat;
	rezultat.id = -1;
	if (*prim) {
		Nod* aux = *prim;
		rezultat = aux->info;
		*prim = (*prim)->next;
		free(aux);
	}
	return rezultat;
}

Masina getMasinaCuPretMaxim(Nod** stiva) {
	Nod* stivaNoua = NULL;
	Masina maxim;
	maxim.id = -1;
	while (*stiva) {
		Masina curenta = popStack(stiva);
		if (maxim.id == -1 || curenta.pret > maxim.pret) {
			if (maxim.id != -1) {
				free(maxim.model);
				free(maxim.numeSofer);
			}
			maxim = copiazaMasina(curenta);
		}
		pushStack(&stivaNoua, curenta);
	}
	while (stivaNoua) {
		pushStack(stiva, popStack(&stivaNoua));
	}
	return maxim;
}

void dezalocareStiva(Nod** stiva) {
	while (*stiva) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

int main() {
	Nod* stiva = NULL;
	pushStack(&stiva, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	pushStack(&stiva, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	pushStack(&stiva, initializareMasina(3, 5, 18000, "Dacia", "Maria", 'A'));

	Masina maxim = getMasinaCuPretMaxim(&stiva);
	afisareMasina(maxim);
	free(maxim.model);
	free(maxim.numeSofer);

	dezalocareStiva(&stiva);
	return 0;
}
