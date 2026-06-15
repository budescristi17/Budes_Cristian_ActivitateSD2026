#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrusi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	printf("id ul masinii: %d\n", masina.id);
	printf("nr ul de usi: %d\n", masina.nrusi);
	printf("pret: %.2f\n", masina.pret);
	printf("model: %s\n", masina.model);
	printf("nume sofer: %s\n", masina.numeSofer);
	printf("serie: %c\n", masina.serie);
	printf("\n");
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	Masina* temp = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));

	for (int i = 0; i < *nrMasini; i++) {
		temp[i] = (*masini)[i];
	}

	temp[*nrMasini] = masinaNoua;

	free(*masini);
	*masini = temp;

	(*nrMasini)++;
}

int citireMasinaFisier(FILE* file, Masina* m) {
	char buffer[100];
	char delimitatori[] = ",\n";

	if (fgets(buffer, sizeof(buffer), file) == NULL) {
		return 0;
	}

	char* token = strtok(buffer, delimitatori);
	if (token == NULL) return 0;
	m->id = atoi(token);

	token = strtok(NULL, delimitatori);
	if (token == NULL) return 0;
	m->nrusi = atoi(token);

	token = strtok(NULL, delimitatori);
	if (token == NULL) return 0;
	m->pret = (float)atof(token);

	token = strtok(NULL, delimitatori);
	if (token == NULL) return 0;
	m->model = (char*)malloc(strlen(token) + 1);
	strcpy(m->model, token);

	token = strtok(NULL, delimitatori);
	if (token == NULL) return 0;
	m->numeSofer = (char*)malloc(strlen(token) + 1);
	strcpy(m->numeSofer, token);

	token = strtok(NULL, delimitatori);
	if (token == NULL) return 0;
	m->serie = token[0];

	return 1;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	FILE* file = fopen(numeFisier, "r");

	if (file == NULL) {
		printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
		return NULL;
	}

	Masina* masini = NULL;
	*nrMasiniCitite = 0;

	Masina m;
	while (citireMasinaFisier(file, &m)) {
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}

	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}

	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main() {
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);

	if (masini != NULL) {
		afisareVectorMasini(masini, nrMasini);
		dezalocareVectorMasini(&masini, &nrMasini);
	}

	return 0;
}