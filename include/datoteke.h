#ifndef DATOTEKE_H
#define DATOTEKE_H
#include "log.h"
#include "pacijent_slog.h"
#include "pregled_slog.h"
extern char pacijenti_filename[100];
extern char pregledi_filename[100];
extern char rasute_filename[100];
void set_pacijenti_file(const char* filename);
void set_pregledi_file(const char* filename);
void set_rasuta_file(const char* filename);
void ispisi_pacijenta(Pacijent pacijent);
void formiraj_datoteku_pacijent(char filename[], int faktor_blokiranja, int* status);
void formiraj_datoteku_pregled(char filename[], int faktor_blokiranja, int* status);
void upisi_slog_datoteke_pacijenti(char filename[], int* status, PacijentSlog* pacijent_slog, int faktor_blokiranja);
void upisi_slog_datoteke_pregledi(char filename[], int* status, PregledSlog* pregled_slog, int faktor_blokiranja);
PacijentSlog* pronadji_slog_pacijent(const char* filename, int key);
PregledSlog* pronadji_slog_pregled(const char* filename, int key);
void prikaz_pritiska(char filename[]);
void prikazi_alergije(char filename[], int broj_kartona);
void modifikuj_pacijenta(char filename[], int broj_kartona, char ime[], char prezime[], char JMBG[], char datum_rodjenja[], float tezina, float visina, char alerg_polen[]);
LogSlog* pronadji_slog_log(const char* filename, int key);
void upisi_slog_datoteke_evidencije(char filename[], int* status, LogSlog* log_slog, int faktor_blokiranja);
void ispisi_datoteku_evidencija(const char filename[], int* status);
void formiraj_datoteku_evidencije(char filename[], int faktor_blokiranja, int* status);
#endif // DATOTEKE_H
