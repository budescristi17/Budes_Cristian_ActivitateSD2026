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

Masina citireMasinaDinLinie(char* buffer) {
    char sep[] = ",\n";
    char* aux;
    Masina m1;

    aux = strtok(buffer, sep);
    m1.id = atoi(aux);

    aux = strtok(NULL, sep);
    m1.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    m1.pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    aux = strtok(NULL, sep);
    m1.serie = aux[0];

    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
    while (cap != NULL) {
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info = masinaNoua;
    nodNou->next = NULL;

    if (*cap == NULL) {
        *cap = nodNou;
    }
    else {
        Nod* aux = *cap;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = nodNou;
    }
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        return NULL;
    }

    Nod* cap = NULL;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        Masina m = citireMasinaDinLinie(buffer);
        adaugaMasinaInLista(&cap, m);
    }

    fclose(file);
    return cap;
}

void dezalocareListaMasini(Nod** cap) {
    while (*cap != NULL) {
        Nod* p = *cap;
        *cap = p->next;

        if (p->info.numeSofer != NULL) {
            free(p->info.numeSofer);
        }
        if (p->info.model != NULL) {
            free(p->info.model);
        }
        free(p);
    }
}

float calculeazaPretMediu(Nod* cap) {
    float suma = 0;
    int contor = 0;

    while (cap != NULL) {
        suma += cap->info.pret;
        contor++;
        cap = cap->next;
    }

    if (contor > 0) {
        return suma / contor;
    }
    return 0;
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
    while (*cap != NULL && (*cap)->info.serie == serieCautata) {
        Nod* temp = *cap;
        *cap = (*cap)->next;

        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }

    Nod* p = *cap;
    while (p != NULL && p->next != NULL) {
        if (p->next->info.serie == serieCautata) {
            Nod* temp = p->next;
            p->next = temp->next;

            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);
        }
        else {
            p = p->next;
        }
    }
}

int main() {
    Nod* cap = citireListaMasiniDinFisier("C:\\Users\\CRISTI\\SDD\\Seminar 4 SDD\\x64\\Debug\\masini.txt");

    printf("Lista initiala:\n\n");
    afisareListaMasini(cap);

    float medie = calculeazaPretMediu(cap);
    printf("\nPretul mediu este: %.2f\n", medie);

    stergeMasiniDinSeria(&cap, 'A');
    printf("\nDupa stergerea seriei A:\n\n");
    afisareListaMasini(cap);

    dezalocareListaMasini(&cap);
    return 0;
}