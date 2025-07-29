#include "../include/datoteke.h"
#include "../include/log.h"
#include "../include/rasute_datoteke.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char pacijenti_filename[100] = "";
char pregledi_filename[100] = "";
char rasute_filename[100] = "";

void set_pacijenti_file(const char* filename)
{
    strncpy(pacijenti_filename, filename, 100 - 1);
    pacijenti_filename[100 - 1] = '\0';
}

void set_pregledi_file(const char* filename)
{
    strncpy(pregledi_filename, filename, 100 - 1);
    pregledi_filename[100 - 1] = '\0';
}

void set_rasuta_file(const char* filename)
{
    strncpy(rasute_filename, filename, 100 - 1);
    rasute_filename[100 - 1] = '\0';
}
#define KRAJ_DATOTEKE -1
#define f1 4
#define f2 6
#define f3 7
int status = -1;
void formiraj_datoteku_pacijent(char filename[], int faktor_blokiranja, int* status)
{
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
        return;
    }

    PacijentSlog* temp_slog = pronadji_slog_pacijent(filename, pacijent_slog->key);
    if (temp_slog != NULL) {
        *status = 2;
        free(temp_slog);
        fclose(fp);
        return;
    }

    PacijentSlog blok[faktor_blokiranja];

    // Pomeri se na kraj bloka
    fseek(fp, -sizeof(PacijentSlog) * faktor_blokiranja, SEEK_END);
    fread(blok, sizeof(PacijentSlog), faktor_blokiranja, fp);

    int i;
    for (i = 0; i < faktor_blokiranja; i++) {
        if (blok[i].key == KRAJ_DATOTEKE) {
            memcpy(&blok[i], pacijent_slog, sizeof(PacijentSlog));
            break;
        }
    }

    i++;
    if (i < faktor_blokiranja) {
        blok[i].key = KRAJ_DATOTEKE;
        fseek(fp, -sizeof(PacijentSlog) * faktor_blokiranja, SEEK_CUR);
        fwrite(blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
        propagiraj_pacijenta_u_rasutu(*pacijent_slog, rasute_filename, "upis", status);
    } else {
        fseek(fp, -sizeof(PacijentSlog) * faktor_blokiranja, SEEK_CUR);
        fwrite(blok, sizeof(PacijentSlog), faktor_blokiranja, fp);

        PacijentSlog novi_blok[faktor_blokiranja];
        Pacijent prazan_pacijent = { 0 };
        for (int j = 0; j < faktor_blokiranja; j++) {
            novi_blok[j].pacijent = prazan_pacijent;
            novi_blok[j].obrisan = 0;
            novi_blok[j].key = 0;
        }
        novi_blok[0] = *pacijent_slog;
        novi_blok[0].obrisan = 0;
        novi_blok[1].key = KRAJ_DATOTEKE;

        fwrite(novi_blok, sizeof(PacijentSlog), faktor_blokiranja, fp);
        propagiraj_pacijenta_u_rasutu(*pacijent_slog, rasute_filename, "upis", status);
    }

    *status = 0;
    fclose(fp);
}
PacijentSlog* pronadji_slog_pacijent(const char* filename, int key)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Greška u otvaranju!");
        return NULL;
    }
    fseek(fp, 0, SEEK_SET);
    PacijentSlog blok[f1];
    int trenutni_blok = 0;
    while (fread(blok, sizeof(PacijentSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {

            if (blok[i].key == KRAJ_DATOTEKE) {
                fclose(fp);
                return NULL;
            }
            if (blok[i].key == key && blok[i].obrisan == 0) {
                PacijentSlog* pronadjen = malloc(sizeof(PacijentSlog));
                memcpy(pronadjen, &blok[i], sizeof(PacijentSlog));
                fclose(fp);
                return pronadjen;
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
        return;
    }
    PregledSlog* temp_slog = pronadji_slog_pregled(filename, pregled_slog->pregled.id);
    if (temp_slog != NULL) {
        printf("Slog sa datim ID-om vec postoji.\n");
        *status = 2;
        free(temp_slog);
        fclose(fp);
        return;
    }
    PregledSlog blok[faktor_blokiranja];
    fseek(fp, -sizeof(PregledSlog) * faktor_blokiranja, SEEK_END);
    fread(blok, sizeof(PregledSlog), faktor_blokiranja, fp);
    int i = 0;
    for (i = 0; i < faktor_blokiranja; i++) {
        if (blok[i].key == KRAJ_DATOTEKE) {
            memcpy(&blok[i], pregled_slog, sizeof(PregledSlog));
            break;
        }
    }

    i++;
    if (i < faktor_blokiranja) {
        blok[i].key = KRAJ_DATOTEKE;
        fseek(fp, -sizeof(PregledSlog) * faktor_blokiranja, SEEK_CUR);
        fwrite(blok, sizeof(PregledSlog), faktor_blokiranja, fp);
        propagiraj_pregled_u_rasutu(rasute_filename, *pregled_slog, status);
    } else {
        fseek(fp, -sizeof(PregledSlog) * faktor_blokiranja, SEEK_CUR);
        fwrite(blok, sizeof(PregledSlog), faktor_blokiranja, fp);
        PregledSlog novi_blok[faktor_blokiranja];
        Pregled prazan_pregled = { 0 };
        for (int i = 0; i < faktor_blokiranja; i++) {
            novi_blok[i].pregled = prazan_pregled;
            novi_blok[i].obrisan = 0; // Pretpostavljam da obrisan nije KRAJ_DATOTEKE nego 0
            novi_blok[i].key = 0;
        }
        novi_blok[0] = *pregled_slog;
        novi_blok[0].obrisan = 0;
        novi_blok[1].key = KRAJ_DATOTEKE;
        fwrite(novi_blok, sizeof(PregledSlog), faktor_blokiranja, fp);
        // DODATO propagiranje
        propagiraj_pregled_u_rasutu(rasute_filename, *pregled_slog, status);
    }
    *status = 0;
    fclose(fp);
}
PregledSlog* pronadji_slog_pregled(const char* filename, int key)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Greška u otvaranju!\n");
        return NULL;
    }
    fseek(fp, 0, SEEK_SET);
    PregledSlog blok[f2];
    while (fread(blok, sizeof(PregledSlog), f2, fp) == f2) {
        for (int i = 0; i < f2; i++) {
            if (blok[i].key == KRAJ_DATOTEKE) {
                fclose(fp);
                return NULL;
            }
            if (blok[i].key == key && blok[i].obrisan == 0) {
                PregledSlog* pronadjen = malloc(sizeof(PregledSlog));
                if (pronadjen != NULL) {
                    memcpy(pronadjen, &blok[i], sizeof(PregledSlog));
                }
                fclose(fp);
                return pronadjen;
            }
        }
    }
    fclose(fp);
    return NULL;
}
void prikazi_alergije(char filename[], int broj_kartona)
{
    FILE* fp = fopen(pacijenti_filename, "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke pacijenti.dat.\n");
        return;
    }

    fseek(fp, 0, SEEK_SET);
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
            if (blok[i].key == broj_kartona) {
                Pacijent pacijent = blok[i].pacijent;
                ispisi_pacijenta(pacijent);
                printf("Adresa bloka: %d\n", trenutni_blok);
                printf("Broj sloga: %d\n", i);
                fclose(fp);
                return;
            }
        }
        trenutni_blok++;
    }
    printf("Pacijent sa ovim brojem kartona nije pronadjen....\n");
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
void prikaz_pritiska(char filename[])
{
    FILE* fp = fopen(pregledi_filename, "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke pacijenti.dat.\n");
        return;
    }

    fseek(fp, 0, SEEK_SET);
    PregledSlog blok[f2];
    PacijentSlog* pacijentSlog;
    int trenutni_blok = 0;
    while (fread(blok, sizeof(PregledSlog), f2, fp) == f2) {
        for (int i = 0; i < f2; i++) {

            pacijentSlog = pronadji_slog_pacijent(pacijenti_filename, blok[i].key);
            if (pacijentSlog == NULL) {
                free(pacijentSlog);
                continue;
            }
            if (blok[i].key == KRAJ_DATOTEKE) {
                free(pacijentSlog);
                break;
            }
            if (blok[i].obrisan != 0) {
                free(pacijentSlog);
                continue; // Preskoci obrisane slogove
            }

            if (blok[i].pregled.dijastolni_pritisak == blok[i].pregled.sistolni_pritisak && blok[i].pregled.sistolni_pritisak > 0) {
                pacijentSlog = pronadji_slog_pacijent(pacijenti_filename, blok[i].key);
                if (pacijentSlog == NULL) {
                    continue;
                }
                ispisi_pacijenta(pacijentSlog->pacijent);
                printf("Sistolni pritisak: %d\n", blok[i].pregled.sistolni_pritisak);
                printf("Adresa bloka: %d\n", trenutni_blok);
                printf("Broj sloga: %d\n", i);
            }
            free(pacijentSlog);
        }
        trenutni_blok++;
    }

    fclose(fp);
}
void modifikuj_pacijenta(char filename[], int broj_kartona, char ime[], char prezime[], char JMBG[], char datum_rodjenja[], float tezina, float visina, char alerg_polen[])
{
    PacijentSlog* pacijent_slog = pronadji_slog_pacijent(pacijenti_filename, broj_kartona);
    if (pacijent_slog == NULL) {
        printf("Pacijent sa brojem kartona %d nije pronadjen.\n", broj_kartona);
        return;
    }
    strcpy(pacijent_slog->pacijent.ime, ime);
    strcpy(pacijent_slog->pacijent.prezime, prezime);
    strcpy(pacijent_slog->pacijent.JMBG, JMBG);
    strcpy(pacijent_slog->pacijent.datum_rodjenja, datum_rodjenja);
    pacijent_slog->pacijent.tezina = tezina;
    pacijent_slog->pacijent.visina = visina;
    strcpy(pacijent_slog->pacijent.alerg_polen, alerg_polen);
    FILE* fp = fopen(pacijenti_filename, "rb+");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke pacijenti.dat.\n");
        free(pacijent_slog);
        return;
    }
    PacijentSlog blok[f1];
    while (fread(blok, sizeof(PacijentSlog), f1, fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (blok[i].key == pacijent_slog->key && blok[i].obrisan == 0) {
                blok[i] = *pacijent_slog;
                fseek(fp, sizeof(PacijentSlog) * -f1, SEEK_CUR);
                fwrite(blok, sizeof(PacijentSlog), f1, fp);
                printf("Podaci pacijenta su uspešno modifikovani.\n");
                propagiraj_pacijenta_u_rasutu(*pacijent_slog, rasute_filename, "modifikacija", &status);
                free(pacijent_slog);
                fclose(fp);
                return;
            }
        }
    }
    printf("Nije moguće pronaći slog u datoteci.\n");
    free(pacijent_slog);
    fclose(fp);
}
void formiraj_datoteku_evidencije(char filename[], int faktor_blokiranja, int* status)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    LogSlog prazan_blok[faktor_blokiranja];
    Log prazan_log = { 0 };
    for (int i = 0; i < faktor_blokiranja; i++) {

        prazan_blok[i].log = prazan_log;
        prazan_blok[i].obrisan = KRAJ_DATOTEKE;
        if (i == 0) {
            prazan_blok[i].key = KRAJ_DATOTEKE;
        } else {
            prazan_blok[i].key = 0;
        }
    }
    fwrite(prazan_blok, sizeof(LogSlog), faktor_blokiranja, fp);
    fclose(fp);
    *status = 0;
}
void upisi_slog_datoteke_evidencije(char filename[], int* status, LogSlog* log_slog, int faktor_blokiranja)
{
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s.\n", filename);
        *status = 2;
        return;
    }
    LogSlog* temp_slog = pronadji_slog_log(filename, log_slog->log.id);
    if (temp_slog != NULL) {
        *status = 2;
        free(temp_slog);
        fclose(fp);
        return;
    }
    LogSlog blok[faktor_blokiranja];
    fseek(fp, sizeof(LogSlog) * -faktor_blokiranja, SEEK_END);
    fread(blok, sizeof(LogSlog), faktor_blokiranja, fp);
    int i = 0;
    while (i < faktor_blokiranja) {
        if (blok[i].key == KRAJ_DATOTEKE) {
            log_slog->key = log_slog->log.id;
            log_slog->obrisan = 0;
            blok[i] = *log_slog;
            i++;
            break;
        }
        i++;
    }
    if (i < faktor_blokiranja) {
        blok[i].key = KRAJ_DATOTEKE;
        fseek(fp, sizeof(LogSlog) * -faktor_blokiranja, SEEK_CUR);
        fwrite(blok, sizeof(LogSlog), faktor_blokiranja, fp);
    } else {
        fseek(fp, sizeof(LogSlog) * -faktor_blokiranja, SEEK_CUR);
        fwrite(blok, sizeof(LogSlog), faktor_blokiranja, fp);
        LogSlog novi_blok[faktor_blokiranja];
        Log prazan_log = { 0 };
        for (int i = 0; i < faktor_blokiranja; i++) {
            novi_blok[i].log = prazan_log;
            novi_blok[i].obrisan = KRAJ_DATOTEKE;
            novi_blok[i].key = 0;
        }
        novi_blok[0].key = log_slog->log.id;
        novi_blok[0] = *log_slog;
        novi_blok[0].obrisan = 0;
        novi_blok[1].key = KRAJ_DATOTEKE;
        fwrite(novi_blok, sizeof(LogSlog), faktor_blokiranja, fp);
    }
    *status = 0;
    fclose(fp);
}
LogSlog* pronadji_slog_log(const char* filename, int key)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Greška u otvaranju!");
        return NULL;
    }
    fseek(fp, 0, SEEK_SET);
    LogSlog blok[f3];
    int trenutni_blok = 0;
    while (fread(blok, sizeof(LogSlog), f3, fp) == f3) {
        for (int i = 0; i < f3; i++) {
            if (blok[i].key == key && blok[i].obrisan == 0) {
                LogSlog* pronadjen = malloc(sizeof(LogSlog));
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
void ispisi_datoteku_evidencija(const char filename[], int* status)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    LogSlog blok[f3];
    int trenutni_blok = 0;
    while (fread(blok, sizeof(LogSlog), f3, fp) == f3) {
        for (int i = 0; i < f3; i++) {
            if (blok[i].key == KRAJ_DATOTEKE) {
                fclose(fp);
                return;
            }
            if (blok[i].obrisan == 1) {
                continue;
            }
            printf("ID: %d\n", blok[i].log.id);
            printf("ID za pristup: %d\n", blok[i].log.id_za_pristup);
            printf("Naziv operacije: %s\n", blok[i].log.naziv_operacije);
            printf("Broj pristupa: %d\n", blok[i].log.broj_pristupa);
            printf("Adresa bloka: %d\n", trenutni_blok);
            printf("Broj sloga: %d\n", i);
        }
        trenutni_blok++;
    }
    fclose(fp);
}
