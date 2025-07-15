#ifndef PACIJENT_H
#define PACIJENT_H
typedef struct {
    int broj_kartona;
    char ime[31];
    char prezime[31];
    char JMBG[14];
    float tezina;
    float visina;
    char alerg_polen[3];
} Pacijent;
#endif // PACIJENT_H
