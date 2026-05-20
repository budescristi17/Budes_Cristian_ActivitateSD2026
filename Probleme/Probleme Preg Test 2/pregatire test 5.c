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

typedef struct Heap Heap;
struct Heap {
	int lungime;
	Masina* vector;
	int nrElemVizibile;
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

Heap initializareHeap(int lungime) {
	Heap h;
	h.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	h.lungime = lungime;
	h.nrElemVizibile = 0;
	return h;
}

void interschimbaMasini(Masina* a, Masina* b) {
	Masina aux = *a;
	*a = *b;
	*b = aux;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuStanga = 2 * pozitieNod + 1;
	int pozFiuDreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	if (pozFiuStanga < heap.nrElemVizibile && heap.vector[pozMax].pret < heap.vector[pozFiuStanga].pret) {
		pozMax = pozFiuStanga;
	}
	if (pozFiuDreapta < heap.nrElemVizibile && heap.vector[pozMax].pret < heap.vector[pozFiuDreapta].pret) {
		pozMax = pozFiuDreapta;
	}
	if (pozMax != pozitieNod) {
		interschimbaMasini(&heap.vector[pozMax], &heap.vector[pozitieNod]);
		filtreazaHeap(heap, pozMax);
	}
}

void reconstruiesteHeap(Heap heap) {
	for (int i = (heap.nrElemVizibile - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
		if (i == 0) {
			break;
		}
	}
}

void extindeHeap(Heap* heap) {
	int lungimeNoua = heap->lungime * 2;
	Masina* vectorNou = (Masina*)malloc(sizeof(Masina) * lungimeNoua);
	for (int i = 0; i < heap->nrElemVizibile; i++) {
		vectorNou[i] = heap->vector[i];
	}
	free(heap->vector);
	heap->vector = vectorNou;
	heap->lungime = lungimeNoua;
}

void inserareMasinaInHeap(Heap* heap, Masina masina) {
	if (heap->nrElemVizibile == heap->lungime) {
		extindeHeap(heap);
	}
	heap->vector[heap->nrElemVizibile] = masina;
	heap->nrElemVizibile++;
	reconstruiesteHeap(*heap);
}

Masina extrageMasina(Heap* heap) {
	Masina rezultat;
	rezultat.id = -1;
	rezultat.nrUsi = 0;
	rezultat.pret = 0;
	rezultat.model = NULL;
	rezultat.numeSofer = NULL;
	rezultat.serie = '-';

	if (heap->nrElemVizibile > 0) {
		rezultat = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemVizibile - 1];
		heap->nrElemVizibile--;
		filtreazaHeap(*heap, 0);
	}
	return rezultat;
}

int cautaPozitieDupaId(Heap heap, int id) {
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		if (heap.vector[i].id == id) {
			return i;
		}
	}
	return -1;
}

int modificaPretDupaId(Heap* heap, int id, float pretNou) {
	int pozitie = cautaPozitieDupaId(*heap, id);
	if (pozitie != -1) {
		heap->vector[pozitie].pret = pretNou;
		reconstruiesteHeap(*heap);
		return 1;
	}
	return 0;
}

Masina stergeMasinaDupaId(Heap* heap, int id) {
	Masina rezultat;
	rezultat.id = -1;
	rezultat.model = NULL;
	rezultat.numeSofer = NULL;
	int pozitie = cautaPozitieDupaId(*heap, id);
	if (pozitie != -1) {
		rezultat = heap->vector[pozitie];
		heap->vector[pozitie] = heap->vector[heap->nrElemVizibile - 1];
		heap->nrElemVizibile--;
		reconstruiesteHeap(*heap);
	}
	return rezultat;
}

float calculeazaPretMediu(Heap heap) {
	float suma = 0;
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		suma += heap.vector[i].pret;
	}
	if (heap.nrElemVizibile == 0) {
		return 0;
	}
	return suma / heap.nrElemVizibile;
}

int numaraMasiniDinSerie(Heap heap, unsigned char serie) {
	int contor = 0;
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		if (heap.vector[i].serie == serie) {
			contor++;
		}
	}
	return contor;
}

void adaugaMasinaInLista(NodLista** lista, Masina masina) {
	NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
	nou->info = masina;
	nou->next = NULL;
	if (*lista == NULL) {
		*lista = nou;
	}
	else {
		NodLista* p = *lista;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}

NodLista* copiazaMasiniPestePret(Heap heap, float pretMinim) {
	NodLista* lista = NULL;
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		if (heap.vector[i].pret > pretMinim) {
			adaugaMasinaInLista(&lista, copiazaMasina(heap.vector[i]));
		}
	}
	return lista;
}

NodLista* copiazaMasiniDupaSofer(Heap heap, const char* numeSofer) {
	NodLista* lista = NULL;
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		if (strcmp(heap.vector[i].numeSofer, numeSofer) == 0) {
			adaugaMasinaInLista(&lista, copiazaMasina(heap.vector[i]));
		}
	}
	return lista;
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

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void extragePrimeleMasini(Heap* heap, int nrExtrageri) {
	for (int i = 0; i < nrExtrageri && heap->nrElemVizibile > 0; i++) {
		Masina m = extrageMasina(heap);
		afisareMasina(m);
		dezalocareMasina(&m);
	}
}

void sorteazaVectorDupaPretDescrescator(Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente - 1; i++) {
		for (int j = i + 1; j < nrElemente; j++) {
			if (vector[i].pret < vector[j].pret) {
				interschimbaMasini(&vector[i], &vector[j]);
			}
		}
	}
}

Masina* copiazaHeapInVectorSortat(Heap heap, int* nrElemente) {
	*nrElemente = heap.nrElemVizibile;
	Masina* vector = (Masina*)malloc(sizeof(Masina) * (*nrElemente));
	for (int i = 0; i < *nrElemente; i++) {
		vector[i] = copiazaMasina(heap.vector[i]);
	}
	sorteazaVectorDupaPretDescrescator(vector, *nrElemente);
	return vector;
}

void afisareVectorMasini(Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisareMasina(vector[i]);
	}
}

void dezalocareVectorMasini(Masina** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		dezalocareMasina(&(*vector)[i]);
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrElemVizibile; i++) {
		dezalocareMasina(&heap->vector[i]);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemVizibile = 0;
}

int main() {
	Heap heap = initializareHeap(4);
	inserareMasinaInHeap(&heap, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	inserareMasinaInHeap(&heap, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	inserareMasinaInHeap(&heap, initializareMasina(3, 5, 18000, "Dacia", "Maria", 'A'));
	inserareMasinaInHeap(&heap, initializareMasina(4, 4, 16000, "Ford", "Vlad", 'C'));
	inserareMasinaInHeap(&heap, initializareMasina(5, 2, 28000, "Audi", "Ana", 'A'));
	inserareMasinaInHeap(&heap, initializareMasina(6, 5, 14000, "Skoda", "Ion", 'B'));
	inserareMasinaInHeap(&heap, initializareMasina(7, 4, 31000, "Mercedes", "Daria", 'C'));

	printf("Heap varianta 1:\n");
	afisareHeap(heap);
	printf("Pret mediu: %.2f\n", calculeazaPretMediu(heap));
	printf("Masini seria A: %d\n", numaraMasiniDinSerie(heap, 'A'));

	modificaPretDupaId(&heap, 3, 34000);
	printf("Dupa modificare pret:\n");
	afisareHeap(heap);

	Masina stearsa = stergeMasinaDupaId(&heap, 6);
	if (stearsa.id != -1) {
		printf("Masina stearsa:\n");
		afisareMasina(stearsa);
		dezalocareMasina(&stearsa);
	}

	NodLista* listaPret = copiazaMasiniPestePret(heap, 20000);
	printf("Lista peste pret:\n");
	afisareLista(listaPret);

	NodLista* listaSofer = copiazaMasiniDupaSofer(heap, "Ana");
	printf("Lista dupa sofer:\n");
	afisareLista(listaSofer);

	int nrVector = 0;
	Masina* vectorSortat = copiazaHeapInVectorSortat(heap, &nrVector);
	printf("Vector sortat:\n");
	afisareVectorMasini(vectorSortat, nrVector);

	printf("Primele extrageri:\n");
	extragePrimeleMasini(&heap, 3);

	dezalocareLista(&listaPret);
	dezalocareLista(&listaSofer);
	dezalocareVectorMasini(&vectorSortat, &nrVector);
	dezalocareHeap(&heap);
	return 0;
}
