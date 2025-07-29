#ifndef PACIJENT_PREGLED_H
#define PACIJENT_PREGLED_H
#include "pacijent.h"
#include "pregled.h"
typedef struct {
    int broj_kartona;
    char ime[31];
    char prezime[31];
    char JMBG[14];
    float tezina;
    float visina;
    float suma_sistolni;
    float suma_dijastolni;
    ;
    float prosecan_sistolni;
    float prosecan_dijastolni;
    int br_pregleda;
} PacijentPregled;
#endif
