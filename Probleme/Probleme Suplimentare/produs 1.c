#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produs {
	int id;
	int stoc;
	char* denumire;
	float pret;
	char categorie;
};

struct Produs initializare(int id, int stoc, const char* denumire, float pret, char categorie) {
	struct Produs p;
	p.id = id;
	p.stoc = stoc;
	p.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(p.denumire, denumire);
	p.pret = pret;
	p.categorie = categorie;
	return p;
}

void afisare(struct Produs p) {
	printf("ID: %d\n", p.id);
	printf("stoc: %d\n", p.stoc);
	printf("denumire: %s\n", p.denumire);
	printf("pret: %.2f\n", p.pret);
	printf("categorie: %c\n\n", p.categorie);
}

float calculTotal(struct Produs p) {
	return p.stoc * p.pret;
}

void afisareProdusePesteValoare(struct Produs* produse, int nrProduse, float prag) {
	for (int i = 0; i < nrProduse; i++) {
		if (calculTotal(produse[i]) > prag) {
			afisare(produse[i]);
		}
	}
}

float calculValoareCategorie(struct Produs* produse, int nrProduse, char categorie) {
	float suma = 0;
	for (int i = 0; i < nrProduse; i++) {
		if (produse[i].categorie == categorie) {
			suma += calculTotal(produse[i]);
		}
	}
	return suma;
}

void dezalocare(struct Produs* produse, int nrProduse) {
	for (int i = 0; i < nrProduse; i++) {
		free(produse[i].denumire);
		produse[i].denumire = NULL;
	}
}

int main() {
	int nrProduse = 4;
	struct Produs* produse = (struct Produs*)malloc(sizeof(struct Produs) * nrProduse);
	produse[0] = initializare(1, 10, "telefon", 2000, 'A');
	produse[1] = initializare(2, 40, "mouse", 80, 'B');
	produse[2] = initializare(3, 5, "laptop", 4500, 'A');
	produse[3] = initializare(4, 12, "tastatura", 250, 'B');

	printf("Produse cu valoarea totala peste prag:\n\n");
	afisareProdusePesteValoare(produse, nrProduse, 5000);

	printf("Valoarea produselor din categoria A: %.2f\n", calculValoareCategorie(produse, nrProduse, 'A'));

	dezalocare(produse, nrProduse);
	free(produse);
	return 0;
}
