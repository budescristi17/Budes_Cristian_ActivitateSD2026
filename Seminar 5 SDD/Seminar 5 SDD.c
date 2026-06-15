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
typedef struct ListaDubla ListaDubla;
typedef struct Nod Nod;

struct Nod {
    Masina info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* prim;
    Nod* ultim;
};

int citireMasinaDinFisier(FILE* file, Masina* m) {
    char buffer[100];
    char sep[] = ",\n";

    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        return 0;
    }

    // daca linia e goala, nu o procesam
    if (strlen(buffer) <= 1) {
        return 0;
    }

    char* aux = strtok(buffer, sep);
    if (aux == NULL) return 0;
    m->id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return 0;
    m->nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return 0;
    m->pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) return 0;
    m->model = (char*)malloc(strlen(aux) + 1);
    strcpy(m->model, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(m->model);
        return 0;
    }
    m->numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m->numeSofer, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) {
        free(m->model);
        free(m->numeSofer);
        return 0;
    }
    m->serie = aux[0];

    return 1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
    Nod* p = lista.prim;
    while (p) {
        afisareMasina(p->info);
        p = p->next;
    }
}

void afisareInversaListaMasini(ListaDubla lista) {
    Nod* p = lista.ultim;
    while (p) {
        afisareMasina(p->info);
        p = p->prev;   // corect: mergem inapoi
    }
}

void adaugaMasinaInLista(ListaDubla* ld, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;
    nou->prev = ld->ultim;

    if (ld->ultim) {
        ld->ultim->next = nou;
    }
    else {
        ld->prim = nou;
    }

    ld->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* ld, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = ld->prim;
    nou->prev = NULL;

    if (ld->prim) {
        ld->prim->prev = nou;
    }
    else {
        ld->ultim = nou;
    }

    ld->prim = nou;
}

ListaDubla citireListaMasiniDinFisier(const char* numeFisier) {
    ListaDubla ld;
    ld.prim = NULL;
    ld.ultim = NULL;

    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        return ld;
    }

    Masina m;
    while (citireMasinaDinFisier(f, &m)) {
        adaugaMasinaInLista(&ld, m);
    }

    fclose(f);
    return ld;
}

void dezalocareListaMasini(ListaDubla* ld) {
    Nod* p = ld->prim;
    while (p) {
        Nod* temp = p;
        p = p->next;

        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }

    ld->prim = NULL;
    ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    Nod* p = lista.prim;
    float suma = 0;
    int contor = 0;

    while (p) {
        suma += p->info.pret;
        contor++;
        p = p->next;
    }

    if (contor == 0) {
        return 0;
    }

    return suma / contor;
}

void stergeMasiniDupaId(ListaDubla* ld, int id) {
    Nod* p = ld->prim;

    while (p && p->info.id != id) {
        p = p->next;
    }

    if (!p) {
        return;
    }

    if (p->prev) {
        p->prev->next = p->next;
    }
    else {
        ld->prim = p->next;
    }

    if (p->next) {
        p->next->prev = p->prev;
    }
    else {
        ld->ultim = p->prev;
    }

    free(p->info.model);
    free(p->info.numeSofer);
    free(p);
}

int main() {
    ListaDubla lista = citireListaMasiniDinFisier("masini.txt");

    printf("Lista initiala:\n\n");
    afisareListaMasini(lista);

    float pretMediu = calculeazaPretMediu(lista);
    printf("Pretul mediu: %.2f\n\n", pretMediu);

    stergeMasiniDupaId(&lista, 2);

    printf("Dupa stergerea masinii cu id = 2:\n\n");
    afisareListaMasini(lista);

    printf("Afisare inversa:\n\n");
    afisareInversaListaMasini(lista);

    dezalocareListaMasini(&lista);
    return 0;
}