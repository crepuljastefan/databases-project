#ifndef DATOTEKE_H
#define DATOTEKE_H
#include "pacijent_slog.h"
#include "pregled_slog.h"
void ispisi_pacijenta(Pacijent pacijent);
void formiraj_datoteku_pacijent(char filename[], int faktor_blokiranja, int* status);
void formiraj_datoteku_pregled(char filename[], int faktor_blokiranja, int* status);
void upisi_slog_datoteke_pacijenti(char filename[], int* status, PacijentSlog* pacijent_slog, int faktor_blokiranja);
void upisi_slog_datoteke_pregledi(char filename[], int* status, PregledSlog* pregled_slog, int faktor_blokiranja);
PacijentSlog* pronadji_slog_pacijent(const char* filename, int key);
PregledSlog* pronadji_slog_pregled(const char* filename, int key);
void prikaz_pritiska();
void prikazi_alergije(int broj_kartona);
void modifikuj_pacijenta(int broj_kartona, char ime[], char prezime[], char JMBG[], char datum_rodjenja[], float tezina, float visina, char alerg_polen[]);
#endif // DATOTEKE_H
