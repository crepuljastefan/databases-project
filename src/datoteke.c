#include "../include/datoteke.h"
#include <cerrno>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define f1 4
#define f2 6
#define KRAJ_DATOTEKE -1
void formiraj_datoteku_pacijent(char filename[], int faktor_blokiranja, int* status)
{
    faktor_blokiranja *= 3;
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    PacijentSlog prazan_blok[faktor_blokiranja];
    Pacijent prazan_pacijent = { 0 };
    for (int i = 0; i < faktor_blokiranja; i++) {

        prazan_blok[i].pacijent = prazan_pacijent;
        prazan_blok[i].obrisan = KRAJ_DATOTEKE;
        if (i == 0) {
            prazan_blok[i].key = KRAJ_DATOTEKE;
        } else {
            prazan_blok[i].key = 0;
        }
    }
    fwrite(prazan_blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
    fclose(fp);
    *status = 0;
}
void formiraj_datoteku_pregled(char filename[], int faktor_blokiranja, int* status)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    PregledSlog prazan_blok[faktor_blokiranja];
    Pregled prazan_pregled = { 0 };
    for (int i = 0; i < faktor_blokiranja; i++) {

        prazan_blok[i].pregled = prazan_pregled;
        prazan_blok[i].obrisan = KRAJ_DATOTEKE;
        if (i == 0) {
            prazan_blok[i].key = KRAJ_DATOTEKE;
        } else {
            prazan_blok[i].key = 0;
        }
    }
    fwrite(prazan_blok, sizeof(PregledSlog), faktor_blokiranja, fp);
    fclose(fp);
    *status = 0;
}
void upisi_slog_datoteke_pacijenti(char filename[], int* status, PacijentSlog* pacijent_slog, int faktor_blokiranja)
{
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s.\n", filename);
        *status = 2;
    } else {
        PacijentSlog* temp_slog = pronadji_slog_pacijent(filename, pacijent_slog->key);
        if (temp_slog != NULL) {
            *status = 2;
        } else {
            PacijentSlog blok[f1];
            fseek(fp, sizeof(PacijentSlog) * -faktor_blokiranja, SEEK_END);
            fread(blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
            int i = 0;
            while (i < faktor_blokiranja) {
                if (blok[i].key == KRAJ_DATOTEKE) {
                    blok[i] = *pacijent_slog;
                    if (i + 1 < faktor_blokiranja) {
                        blok[i + 1].key = KRAJ_DATOTEKE;
                    }
                    i++;
                    break;
                }
                i++;
            }
            if (i < faktor_blokiranja) {
                blok[i].key = KRAJ_DATOTEKE;
                fseek(fp, sizeof(PacijentSlog) * -faktor_blokiranja, SEEK_END);
                fwrite(blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
                printf("Slog je uspešno upisan.\n");
            } else {
                fseek(fp, sizeof(PacijentSlog) * -faktor_blokiranja, SEEK_END);
                fwrite(blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
                PacijentSlog novi_blok[faktor_blokiranja];
                Pacijent prazan_pacijent = { 0 };
                for (int i = 0; i < faktor_blokiranja; i++) {
                    novi_blok[i].obrisan = -1;
                    novi_blok[i].pacijent = prazan_pacijent;
                }
                novi_blok[0].key = KRAJ_DATOTEKE;
                fwrite(novi_blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
                printf("Slog je uspešno upisan, ali je došlo do prepunjenja bloka. Novi blok je formiran.\n");
            }
        }
    }
}
PacijentSlog* pronadji_slog_pacijent(const char* filename, int key)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Greška u otvaranju!");
        return NULL;
    }
    rewind(fp);
    PacijentSlog blok[f1];
    int trenutni_blok = 0;
    while (fread(blok, sizeof(PacijentSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (blok[i].key == key && blok[i].obrisan == 0) {
                PacijentSlog* pronadjen = malloc(sizeof(PacijentSlog));
                if (pronadjen != NULL) {
                    *pronadjen = blok[i];
                }
                fclose(fp);
                return pronadjen;
            }
            if (blok[i].key == KRAJ_DATOTEKE) {
                fclose(fp);
                return NULL;
            }
        }
        trenutni_blok++;
    }
    fclose(fp);
    return NULL;
}
void upisi_slog_datoteke_pregledi(char filename[], int* status, PregledSlog* pregled_slog, int faktor_blokiranja)
{
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s.\n", filename);
        *status = 2;
    } else {
        PregledSlog* temp_slog = pronadji_slog_pregled(filename, pregled_slog->key);
        if (temp_slog != NULL) {
            *status = 2;
        } else {
            PregledSlog blok[f1];
            fseek(fp, sizeof(PregledSlog) * -faktor_blokiranja, SEEK_END);
            fread(blok, sizeof(PregledSlog), faktor_blokiranja, fp);
            int i = 0;
            while (i < faktor_blokiranja) {
                if (blok[i].key == KRAJ_DATOTEKE) {
                    blok[i] = *pregled_slog;
                    if (i + 1 < faktor_blokiranja) {
                        blok[i + 1].key = KRAJ_DATOTEKE;
                    }
                    i++;
                    break;
                }
                i++;
            }
            if (i < faktor_blokiranja) {
                blok[i].key = KRAJ_DATOTEKE;
                fseek(fp, sizeof(PregledSlog) * -faktor_blokiranja, SEEK_END);
                fwrite(blok, sizeof(PregledSlog), faktor_blokiranja, fp);
            } else {
                fseek(fp, sizeof(PregledSlog) * -faktor_blokiranja, SEEK_END);
                fwrite(blok, sizeof(PregledSlog), faktor_blokiranja, fp);
                PregledSlog novi_blok[faktor_blokiranja];
                Pregled prazan_pregled = { 0 };
                for (int i = 0; i < faktor_blokiranja; i++) {
                    novi_blok[i].obrisan = -1;
                    novi_blok[i].pregled = prazan_pregled;
                }
                novi_blok[0].key = KRAJ_DATOTEKE;
                fwrite(novi_blok, sizeof(PregledSlog), faktor_blokiranja, fp);
            }
        }
    }
}
PregledSlog* pronadji_slog_pregled(const char* filename, int key)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Greška u otvaranju!");
        return NULL;
    }
    rewind(fp);
    PregledSlog blok[f1];
    int trenutni_blok = 0;
    while (fread(blok, sizeof(PregledSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (blok[i].key == key && blok[i].obrisan == 0) {
                PregledSlog* pronadjen = malloc(sizeof(PregledSlog));
                if (pronadjen != NULL) {
                    *pronadjen = blok[i];
                }
                fclose(fp);
                return pronadjen;
            }
            if (blok[i].key == KRAJ_DATOTEKE) {
                fclose(fp);
                return NULL;
            }
        }
        trenutni_blok++;
    }
    fclose(fp);
    return NULL;
}
Pacijent pronadji_pacijenta(int broj_kartona)
{
    FILE* fp = fopen("pacijenti.dat", "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke pacijenti.dat.\n");
        return (Pacijent) { 0 };
    }
    PacijentSlog blok[f1];
    while (fread(blok, sizeof(PacijentSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (blok[i].pacijent.broj_kartona == broj_kartona) {
                printf("Pacijent: %s %s\n", blok[i].pacijent.ime, blok[i].pacijent.prezime);
                printf("Broj kartona: %d\n", blok[i].pacijent.broj_kartona);
                fclose(fp);
                return blok[i].pacijent;
            }
        }
    }
    printf("Pacijent sa ovim brojem kartona nije pronadjen..\n");
    return (Pacijent) { 0 };
    fclose(fp);
}
void prikazi_alergije(int broj_kartona, int* adresa_bloka, int* broj_sloga)
{
    FILE* fp = fopen("pacijenti.dat", "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke pacijenti.dat.\n");
        *adresa_bloka = -1;
        *broj_sloga = -1;
        return;
    }
    PacijentSlog blok[f1];
    int trenutni_blok = 0;
    while (fread(blok, sizeof(PacijentSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (blok[i].key == KRAJ_DATOTEKE) {
                break;
            }
            if (blok[i].obrisan == 1) {
                continue; // Preskoci obrisane slogove
            }
            if (blok[i].pacijent.broj_kartona == broj_kartona) {
                Pacijent pacijent = blok[i].pacijent;
                ispisi_pacijenta(pacijent);
                printf("Adresa bloka: %d\n", trenutni_blok);
                *adresa_bloka = trenutni_blok;
                *broj_sloga = i;
                fclose(fp);
                return;
            }
        }
        trenutni_blok++;
    }
    printf("Pacijent sa ovim brojem kartona nije pronadjen..\n");
    fclose(fp);
}
void ispisi_pacijenta(Pacijent pacijent)
{
    printf("Ime: %s\n", pacijent.ime);
    printf("Prezime: %s\n", pacijent.prezime);
    printf("Datum rodjenja: %s\n", pacijent.datum_rodjenja);
    printf("Broj kartona: %d\n", pacijent.broj_kartona);
    printf("JMBG: %s\n", pacijent.JMBG);
    printf("Visina: %.2f cm\n", pacijent.visina);
    printf("Tezina: %.2f kg\n", pacijent.tezina);
    printf("Alergija na polen: %s\n", pacijent.alerg_polen);
}
void prikaz_pritiska(int* adresa_bloka, int* broj_sloga)
{
    FILE* fp = fopen("pregledi.dat", "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke pacijenti.dat.\n");
        *adresa_bloka = -1;
        *broj_sloga = -1;
        return;
    }
    PregledSlog blok[f1];
    Pacijent pacijent;
    int trenutni_blok = 0;
    while (fread(blok, sizeof(PregledSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (blok[i].key == KRAJ_DATOTEKE) {
                break;
            }
            if (blok[i].obrisan == 1) {
                continue; // Preskoci obrisane slogove
            }
            if (blok[i].pregled.dijastolni_pritisak == blok[i].pregled.sistolni_pritisak) {
                pacijent = pronadji_pacijenta(blok[i].pregled.broj_kartona);
                ispisi_pacijenta(pacijent);
                *adresa_bloka = trenutni_blok;
                *broj_sloga = i;
            }
        }
        trenutni_blok++;
    }

    fclose(fp);
}
