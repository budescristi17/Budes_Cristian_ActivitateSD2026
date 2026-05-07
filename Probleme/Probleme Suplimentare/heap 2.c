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
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
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

void inserareMasinaInHeap(Heap* heap, Masina masina) {
	heap->vector[heap->nrElemVizibile] = masina;
	heap->nrElemVizibile++;
	reconstruiesteHeap(*heap);
}

void marestePretDupaId(Heap* heap, int id, float procent) {
	for (int i = 0; i < heap->nrElemVizibile; i++) {
		if (heap->vector[i].id == id) {
			heap->vector[i].pret += heap->vector[i].pret * procent / 100;
		}
	}
	reconstruiesteHeap(*heap);
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemVizibile; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrElemVizibile; i++) {
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemVizibile = 0;
}

int main() {
	Heap h = initializareHeap(5);
	inserareMasinaInHeap(&h, initializareMasina(1, 4, 9000, "Logan", "Ion", 'A'));
	inserareMasinaInHeap(&h, initializareMasina(2, 2, 25000, "BMW", "Ana", 'B'));
	inserareMasinaInHeap(&h, initializareMasina(3, 5, 18000, "Dacia", "Maria", 'C'));

	marestePretDupaId(&h, 1, 300);
	afisareHeap(h);

	dezalocareHeap(&h);
	return 0;
}
