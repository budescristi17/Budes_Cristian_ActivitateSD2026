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

void modificaDenumire(struct Produs* p, const char* denumireNoua) {
	free(p->denumire);
	p->denumire = (char*)malloc(strlen(denumireNoua) + 1);
	strcpy(p->denumire, denumireNoua);
}

void actualizeazaStoc(struct Produs* produse, int nrProduse, int id, int stocNou) {
	for (int i = 0; i < nrProduse; i++) {
		if (produse[i].id == id) {
			produse[i].stoc = stocNou;
		}
	}
}

void aplicaReducereCategorie(struct Produs* produse, int nrProduse, char categorie, float procent) {
	for (int i = 0; i < nrProduse; i++) {
		if (produse[i].categorie == categorie) {
			produse[i].pret = produse[i].pret - produse[i].pret * procent / 100;
		}
	}
}

void afisareVector(struct Produs* produse, int nrProduse) {
	for (int i = 0; i < nrProduse; i++) {
		afisare(produse[i]);
	}
}

void dezalocare(struct Produs* produse, int nrProduse) {
	for (int i = 0; i < nrProduse; i++) {
		free(produse[i].denumire);
		produse[i].denumire = NULL;
	}
}

int main() {
	int nrProduse = 3;
	struct Produs* produse = (struct Produs*)malloc(sizeof(struct Produs) * nrProduse);
	produse[0] = initializare(10, 7, "monitor", 900, 'E');
	produse[1] = initializare(11, 15, "cablu", 30, 'A');
	produse[2] = initializare(12, 6, "router", 250, 'E');

	actualizeazaStoc(produse, nrProduse, 12, 20);
	modificaDenumire(&produse[1], "cablu hdmi");
	aplicaReducereCategorie(produse, nrProduse, 'E', 10);
	afisareVector(produse, nrProduse);

	dezalocare(produse, nrProduse);
	free(produse);
	return 0;
}
