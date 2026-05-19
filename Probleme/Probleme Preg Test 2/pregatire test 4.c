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
	Nod* stanga;
	Nod* dreapta;
	int inaltime;
};

typedef struct NodLista NodLista;
struct NodLista {
	Masina info;
	NodLista* next;
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

void dezalocareMasina(Masina* m) {
	if (m->model) {
		free(m->model);
		m->model = NULL;
	}
	if (m->numeSofer) {
		free(m->numeSofer);
		m->numeSofer = NULL;
	}
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int maxim(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int inaltimeNod(Nod* nod) {
	if (nod) {
		return nod->inaltime;
	}
	return 0;
}

int calculeazaFactor(Nod* nod) {
	if (nod) {
		return inaltimeNod(nod->stanga) - inaltimeNod(nod->dreapta);
	}
	return 0;
}

Nod* creareNod(Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->stanga = NULL;
	nou->dreapta = NULL;
	nou->inaltime = 1;
	return nou;
}

Nod* rotireDreapta(Nod* y) {
	Nod* x = y->stanga;
	Nod* t2 = x->dreapta;
	x->dreapta = y;
	y->stanga = t2;
	y->inaltime = maxim(inaltimeNod(y->stanga), inaltimeNod(y->dreapta)) + 1;
	x->inaltime = maxim(inaltimeNod(x->stanga), inaltimeNod(x->dreapta)) + 1;
	return x;
}

Nod* rotireStanga(Nod* x) {
	Nod* y = x->dreapta;
	Nod* t2 = y->stanga;
	y->stanga = x;
	x->dreapta = t2;
	x->inaltime = maxim(inaltimeNod(x->stanga), inaltimeNod(x->dreapta)) + 1;
	y->inaltime = maxim(inaltimeNod(y->stanga), inaltimeNod(y->dreapta)) + 1;
	return y;
}

Nod* echilibreazaNod(Nod* radacina) {
	if (radacina == NULL) {
		return radacina;
	}
	radacina->inaltime = 1 + maxim(inaltimeNod(radacina->stanga), inaltimeNod(radacina->dreapta));
	int factor = calculeazaFactor(radacina);
	if (factor > 1 && calculeazaFactor(radacina->stanga) >= 0) {
		return rotireDreapta(radacina);
	}
	if (factor > 1 && calculeazaFactor(radacina->stanga) < 0) {
		radacina->stanga = rotireStanga(radacina->stanga);
		return rotireDreapta(radacina);
	}
	if (factor < -1 && calculeazaFactor(radacina->dreapta) <= 0) {
		return rotireStanga(radacina);
	}
	if (factor < -1 && calculeazaFactor(radacina->dreapta) > 0) {
		radacina->dreapta = rotireDreapta(radacina->dreapta);
		return rotireStanga(radacina);
	}
	return radacina;
}

Nod* adaugaMasinaInAVL(Nod* radacina, Masina masinaNoua) {
	if (radacina == NULL) {
		return creareNod(masinaNoua);
	}
	if (masinaNoua.id < radacina->info.id) {
		radacina->stanga = adaugaMasinaInAVL(radacina->stanga, masinaNoua);
	}
	else if (masinaNoua.id > radacina->info.id) {
		radacina->dreapta = adaugaMasinaInAVL(radacina->dreapta, masinaNoua);
	}
	else {
		dezalocareMasina(&masinaNoua);
		return radacina;
	}
	return echilibreazaNod(radacina);
}

Nod* cautaMinim(Nod* radacina) {
	if (radacina) {
		while (radacina->stanga) {
			radacina = radacina->stanga;
		}
	}
	return radacina;
}

Nod* stergeMasinaDinAVL(Nod* radacina, int id) {
	if (radacina == NULL) {
		return radacina;
	}
	if (id < radacina->info.id) {
		radacina->stanga = stergeMasinaDinAVL(radacina->stanga, id);
	}
	else if (id > radacina->info.id) {
		radacina->dreapta = stergeMasinaDinAVL(radacina->dreapta, id);
	}
	else {
		if (radacina->stanga == NULL || radacina->dreapta == NULL) {
			Nod* temp = radacina->stanga ? radacina->stanga : radacina->dreapta;
			if (temp == NULL) {
				dezalocareMasina(&radacina->info);
				free(radacina);
				return NULL;
			}
			else {
				Nod* nodDeSters = radacina;
				radacina = temp;
				dezalocareMasina(&nodDeSters->info);
				free(nodDeSters);
			}
		}
		else {
			Nod* succesor = cautaMinim(radacina->dreapta);
			Masina copie = copiazaMasina(succesor->info);
			int idSuccesor = succesor->info.id;
			dezalocareMasina(&radacina->info);
			radacina->info = copie;
			radacina->dreapta = stergeMasinaDinAVL(radacina->dreapta, idSuccesor);
		}
	}
	return echilibreazaNod(radacina);
}

Masina cautaMasinaDupaId(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			return copiazaMasina(radacina->info);
		}
		if (radacina->info.id > id) {
			return cautaMasinaDupaId(radacina->stanga, id);
		}
		return cautaMasinaDupaId(radacina->dreapta, id);
	}
	Masina m;
	m.id = -1;
	m.nrUsi = 0;
	m.pret = 0;
	m.model = NULL;
	m.numeSofer = NULL;
	m.serie = '-';
	return m;
}

int verificaAVL(Nod* radacina) {
	if (radacina) {
		int factor = calculeazaFactor(radacina);
		if (factor < -1 || factor > 1) {
			return 0;
		}
		return verificaAVL(radacina->stanga) && verificaAVL(radacina->dreapta);
	}
	return 1;
}

int numaraMasiniPestePret(Nod* radacina, float pretMinim) {
	if (radacina) {
		int rezultat = numaraMasiniPestePret(radacina->stanga, pretMinim) + numaraMasiniPestePret(radacina->dreapta, pretMinim);
		if (radacina->info.pret > pretMinim) {
			rezultat++;
		}
		return rezultat;
	}
	return 0;
}

float calculeazaPretTotalSerie(Nod* radacina, unsigned char serie) {
	if (radacina) {
		float suma = calculeazaPretTotalSerie(radacina->stanga, serie) + calculeazaPretTotalSerie(radacina->dreapta, serie);
		if (radacina->info.serie == serie) {
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;
}

void adaugaMasinaInLista(NodLista** cap, Masina masina) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info = masina;
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		NodLista* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}

void copiazaMasiniDupaSerie(Nod* radacina, unsigned char serie, NodLista** lista) {
	if (radacina) {
		copiazaMasiniDupaSerie(radacina->stanga, serie, lista);
		if (radacina->info.serie == serie) {
			adaugaMasinaInLista(lista, copiazaMasina(radacina->info));
		}
		copiazaMasiniDupaSerie(radacina->dreapta, serie, lista);
	}
}

void afisareLista(NodLista* lista) {
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void dezalocareLista(NodLista** lista) {
	while (*lista) {
		NodLista* temp = *lista;
		*lista = (*lista)->next;
		dezalocareMasina(&temp->info);
		free(temp);
	}
}

void afisareInordine(Nod* radacina) {
	if (radacina) {
		afisareInordine(radacina->stanga);
		afisareMasina(radacina->info);
		afisareInordine(radacina->dreapta);
	}
}

void afisarePreordineCuFactor(Nod* radacina) {
	if (radacina) {
		printf("Id: %d, factor: %d, inaltime: %d\n", radacina->info.id, calculeazaFactor(radacina), radacina->inaltime);
		afisarePreordineCuFactor(radacina->stanga);
		afisarePreordineCuFactor(radacina->dreapta);
	}
}

void dezalocareAVL(Nod** radacina) {
	if (*radacina) {
		dezalocareAVL(&(*radacina)->stanga);
		dezalocareAVL(&(*radacina)->dreapta);
		dezalocareMasina(&(*radacina)->info);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* radacina = NULL;
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(10, 4, 12000, "Skoda", "Vlad", 'A'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(20, 2, 18000, "Mini", "Ioana", 'B'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(30, 5, 26000, "Dacia", "Matei", 'A'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(40, 3, 9000, "Hyundai", "Elena", 'C'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(50, 4, 21000, "Audi", "Ana", 'A'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(25, 4, 17000, "Seat", "Cristian", 'B'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(5, 2, 8000, "Ford", "Daria", 'C'));
	radacina = adaugaMasinaInAVL(radacina, initializareMasina(35, 5, 24000, "BMW", "Vlad", 'A'));

	printf("AVL varianta 2:\n");
	afisareInordine(radacina);
	printf("Factori de echilibru:\n");
	afisarePreordineCuFactor(radacina);
	printf("Este AVL: %d\n", verificaAVL(radacina));
	printf("Masini peste 17000: %d\n", numaraMasiniPestePret(radacina, 17000));
	printf("Pret total seria A: %.2f\n", calculeazaPretTotalSerie(radacina, 'A'));

	Masina cautata = cautaMasinaDupaId(radacina, 35);
	if (cautata.id != -1) {
		printf("Masina cautata:\n");
		afisareMasina(cautata);
		dezalocareMasina(&cautata);
	}

	NodLista* lista = NULL;
	copiazaMasiniDupaSerie(radacina, 'A', &lista);
	printf("Lista seria A:\n");
	afisareLista(lista);

	radacina = stergeMasinaDinAVL(radacina, 10);
	radacina = stergeMasinaDinAVL(radacina, 50);
	printf("Dupa stergeri:\n");
	afisareInordine(radacina);
	printf("Este AVL dupa stergeri: %d\n", verificaAVL(radacina));

	dezalocareLista(&lista);
	dezalocareAVL(&radacina);
	return 0;
}
