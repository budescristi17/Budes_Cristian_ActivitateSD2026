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

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if (*radacina) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->stanga, masinaNoua);
		}
		else if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&(*radacina)->dreapta, masinaNoua);
		}
		else {
			dezalocareMasina(&masinaNoua);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->stanga = NULL;
		nou->dreapta = NULL;
		*radacina = nou;
	}
}

void afisareInordine(Nod* radacina) {
	if (radacina) {
		afisareInordine(radacina->stanga);
		afisareMasina(radacina->info);
		afisareInordine(radacina->dreapta);
	}
}

void afisarePreordine(Nod* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisarePreordine(radacina->stanga);
		afisarePreordine(radacina->dreapta);
	}
}

void afisarePostordine(Nod* radacina) {
	if (radacina) {
		afisarePostordine(radacina->stanga);
		afisarePostordine(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

int maxim(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int determinaInaltime(Nod* radacina) {
	if (radacina) {
		return 1 + maxim(determinaInaltime(radacina->stanga), determinaInaltime(radacina->dreapta));
	}
	return 0;
}

int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
	}
	return 0;
}

int determinaNumarFrunze(Nod* radacina) {
	if (radacina) {
		if (radacina->stanga == NULL && radacina->dreapta == NULL) {
			return 1;
		}
		return determinaNumarFrunze(radacina->stanga) + determinaNumarFrunze(radacina->dreapta);
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret + calculeazaPretTotal(radacina->stanga) + calculeazaPretTotal(radacina->dreapta);
	}
	return 0;
}

float calculeazaPretMediu(Nod* radacina) {
	int nrNoduri = determinaNumarNoduri(radacina);
	if (nrNoduri == 0) {
		return 0;
	}
	return calculeazaPretTotal(radacina) / nrNoduri;
}

int numaraMasiniDinSerie(Nod* radacina, unsigned char serie) {
	if (radacina) {
		int rezultat = numaraMasiniDinSerie(radacina->stanga, serie) + numaraMasiniDinSerie(radacina->dreapta, serie);
		if (radacina->info.serie == serie) {
			rezultat++;
		}
		return rezultat;
	}
	return 0;
}

float calculeazaPretTotalSofer(Nod* radacina, const char* numeSofer) {
	if (radacina) {
		float suma = calculeazaPretTotalSofer(radacina->stanga, numeSofer) + calculeazaPretTotalSofer(radacina->dreapta, numeSofer);
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			suma += radacina->info.pret;
		}
		return suma;
	}
	return 0;
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

int modificaPretDupaId(Nod* radacina, int id, float pretNou) {
	if (radacina) {
		if (radacina->info.id == id) {
			radacina->info.pret = pretNou;
			return 1;
		}
		if (radacina->info.id > id) {
			return modificaPretDupaId(radacina->stanga, id, pretNou);
		}
		return modificaPretDupaId(radacina->dreapta, id, pretNou);
	}
	return 0;
}

int modificaSoferDupaId(Nod* radacina, int id, const char* numeSoferNou) {
	if (radacina) {
		if (radacina->info.id == id) {
			free(radacina->info.numeSofer);
			radacina->info.numeSofer = (char*)malloc(strlen(numeSoferNou) + 1);
			strcpy(radacina->info.numeSofer, numeSoferNou);
			return 1;
		}
		if (radacina->info.id > id) {
			return modificaSoferDupaId(radacina->stanga, id, numeSoferNou);
		}
		return modificaSoferDupaId(radacina->dreapta, id, numeSoferNou);
	}
	return 0;
}

Nod* cautaMinim(Nod* radacina) {
	if (radacina) {
		while (radacina->stanga) {
			radacina = radacina->stanga;
		}
	}
	return radacina;
}

Nod* cautaMaxim(Nod* radacina) {
	if (radacina) {
		while (radacina->dreapta) {
			radacina = radacina->dreapta;
		}
	}
	return radacina;
}

void stergeMasinaDupaId(Nod** radacina, int id) {
	if (*radacina == NULL) {
		return;
	}
	if ((*radacina)->info.id > id) {
		stergeMasinaDupaId(&(*radacina)->stanga, id);
	}
	else if ((*radacina)->info.id < id) {
		stergeMasinaDupaId(&(*radacina)->dreapta, id);
	}
	else {
		Nod* temp = *radacina;
		if ((*radacina)->stanga == NULL && (*radacina)->dreapta == NULL) {
			dezalocareMasina(&temp->info);
			free(temp);
			*radacina = NULL;
		}
		else if ((*radacina)->stanga == NULL) {
			*radacina = (*radacina)->dreapta;
			dezalocareMasina(&temp->info);
			free(temp);
		}
		else if ((*radacina)->dreapta == NULL) {
			*radacina = (*radacina)->stanga;
			dezalocareMasina(&temp->info);
			free(temp);
		}
		else {
			Nod* succesor = cautaMinim((*radacina)->dreapta);
			Masina copie = copiazaMasina(succesor->info);
			int idSuccesor = succesor->info.id;
			dezalocareMasina(&(*radacina)->info);
			(*radacina)->info = copie;
			stergeMasinaDupaId(&(*radacina)->dreapta, idSuccesor);
		}
	}
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

void copiazaMasiniPestePret(Nod* radacina, float pretMinim, NodLista** lista) {
	if (radacina) {
		copiazaMasiniPestePret(radacina->stanga, pretMinim, lista);
		if (radacina->info.pret > pretMinim) {
			adaugaMasinaInLista(lista, copiazaMasina(radacina->info));
		}
		copiazaMasiniPestePret(radacina->dreapta, pretMinim, lista);
	}
}

void copiazaMasiniDupaIntervalId(Nod* radacina, int idMinim, int idMaxim, NodLista** lista) {
	if (radacina) {
		if (radacina->info.id > idMinim) {
			copiazaMasiniDupaIntervalId(radacina->stanga, idMinim, idMaxim, lista);
		}
		if (radacina->info.id >= idMinim && radacina->info.id <= idMaxim) {
			adaugaMasinaInLista(lista, copiazaMasina(radacina->info));
		}
		if (radacina->info.id < idMaxim) {
			copiazaMasiniDupaIntervalId(radacina->dreapta, idMinim, idMaxim, lista);
		}
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

void dezalocareArbore(Nod** radacina) {
	if (*radacina) {
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);
		dezalocareMasina(&(*radacina)->info);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* radacina = NULL;
	adaugaMasinaInArbore(&radacina, initializareMasina(50, 4, 12000, "Skoda", "Vlad", 'A'));
	adaugaMasinaInArbore(&radacina, initializareMasina(30, 2, 18000, "Mini", "Ioana", 'B'));
	adaugaMasinaInArbore(&radacina, initializareMasina(70, 5, 26000, "Dacia", "Matei", 'A'));
	adaugaMasinaInArbore(&radacina, initializareMasina(20, 3, 9000, "Hyundai", "Elena", 'C'));
	adaugaMasinaInArbore(&radacina, initializareMasina(40, 4, 21000, "Audi", "Ana", 'A'));
	adaugaMasinaInArbore(&radacina, initializareMasina(60, 2, 15000, "Ford", "Vlad", 'B'));
	adaugaMasinaInArbore(&radacina, initializareMasina(80, 5, 30000, "BMW", "Ioana", 'C'));
	adaugaMasinaInArbore(&radacina, initializareMasina(35, 4, 17000, "Seat", "Cristian", 'A'));
	adaugaMasinaInArbore(&radacina, initializareMasina(45, 4, 14000, "Renault", "Daria", 'B'));

	printf("Afisare inordine:\n");
	afisareInordine(radacina);
	printf("Numar noduri: %d\n", determinaNumarNoduri(radacina));
	printf("Numar frunze: %d\n", determinaNumarFrunze(radacina));
	printf("Inaltime: %d\n", determinaInaltime(radacina));
	printf("Pret mediu: %.2f\n", calculeazaPretMediu(radacina));
	printf("Masini seria A: %d\n", numaraMasiniDinSerie(radacina, 'A'));
	printf("Pret total Vlad: %.2f\n", calculeazaPretTotalSofer(radacina, "Vlad"));

	Masina cautata = cautaMasinaDupaId(radacina, 40);
	if (cautata.id != -1) {
		printf("Masina cautata:\n");
		afisareMasina(cautata);
		dezalocareMasina(&cautata);
	}

	modificaPretDupaId(radacina, 60, 19500);
	modificaSoferDupaId(radacina, 35, "Andrei Pop");

	NodLista* listaPret = NULL;
	copiazaMasiniPestePret(radacina, 18000, &listaPret);
	printf("Lista masini peste pret:\n");
	afisareLista(listaPret);

	NodLista* listaInterval = NULL;
	copiazaMasiniDupaIntervalId(radacina, 30, 60, &listaInterval);
	printf("Lista masini din interval:\n");
	afisareLista(listaInterval);

	stergeMasinaDupaId(&radacina, 30);
	stergeMasinaDupaId(&radacina, 70);
	printf("Dupa stergeri:\n");
	afisareInordine(radacina);

	dezalocareLista(&listaPret);
	dezalocareLista(&listaInterval);
	dezalocareArbore(&radacina);
	return 0;
}
