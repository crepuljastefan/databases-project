#ifndef RASUTE_DATOTEKE_H
#define RASUTE_DATOTEKE_H
#include "pacijent_pregled_slog.h"
#include "pacijent_slog.h"
#include "pregled_slog.h"
#include <stdio.h>
typedef struct {
    Pacijent pacijent;
    float sistolni_suma;
    float dijastolni_suma;
    int br_pregleda;
} PomocniPacijent;
void ispisi_rasutu_datoteku(const char* filename, int broj_baketa, int faktor);
void formiraj_rasutu_datoteku(char filename[], char pregledi_filename[], char pacijenti_filename[], int* status);
void ucitaj_pacijente(FILE* pacijenti_fp, PacijentSlog* pacijentBlok, PomocniPacijent* pomocni_pacijenti);
void ucitaj_preglede(FILE* pregledi_fp, PregledSlog* pregledBlok, PomocniPacijent* pomocni_pacijenti);
void inicijalizuj_buckete(PacijentPregledSlog* bucketi);
void pronadji_mesto_u_bucketu(PomocniPacijent* pomocni_pacijenti, PacijentPregledSlog* bucketi);
void upisi_slog_rasuta(PacijentPregledSlog* slog, int* counter, const char* filename, int* status);
int nadji_slobodan_index(FILE* fp, int* counter, int broj_kartona);
int is_prekoracilac(int index, PacijentPregledSlog slog);
void prikazi_tri_pregleda(const char filename[], int* status);
void prikazi_prosecan_pritisak(const char filename[], int broj_kartona, int* status);
void propagiraj_pacijenta_u_rasutu(PacijentSlog pacijentSlog, const char rasuta_datoteka[], const char operacija[], int* status);
void propagiraj_pregled_u_rasutu(const char rasuta_datoteka[], PregledSlog pregled_slog, int* status);
void logicki_obrisi_slog(const char filename[], int broj_kartona, int* status);
#endif // RASUTE_DATOTEKE_H
