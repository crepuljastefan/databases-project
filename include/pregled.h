#ifndef PREGLED_H
#define PREGLED_H
typedef struct {
    int id;
    int broj_kartona;
    char datum_pregleda[11];
    int sistolni_pritisak;
    int dijastolni_pritisak;
} Pregled;
#endif // PREGLED_H
