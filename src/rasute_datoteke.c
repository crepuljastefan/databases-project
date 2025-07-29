#include "../include/rasute_datoteke.h"
#include "../include/datoteke.h"
#include "../include/log.h"
#include "../include/pacijent_pregled_slog.h"
#include "../include/pacijent_slog.h"
#include "../include/pregled_slog.h"
#include <stdio.h>
#include <string.h>

#define KRAJ_DATOTEKE -1
#define f1 4
#define f2 6
#define B 9
#define b 4
#define k 1
int pronadji_pomocnog_pacijenta(int broj_kartona, PomocniPacijent pomocni_pacijenti[])
{
    for (int i = 0; i < B * b; i++) {
        if (pomocni_pacijenti[i].pacijent.broj_kartona == broj_kartona) {
            return i;
        }
    }
    return -1;
}
void formiraj_rasutu_datoteku(char filename[], char pregledi_filename[], char pacijenti_filename[], int* status)
{
    FILE* fp = fopen(filename, "wb");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    printf("%s", pacijenti_filename);
    printf("%s", pregledi_filename);
    FILE* pacijenti_fp = fopen(pacijenti_filename, "rb");
    FILE* pregledi_fp = fopen(pregledi_filename, "rb");
    if (pacijenti_fp == NULL || pregledi_fp == NULL) {
        *status = 2;
        fclose(fp);
        return;
    }
    PacijentSlog pacijentBlok[f1];
    PregledSlog pregledBlok[f2];
    PacijentPregledSlog bucketi[B * b];
    PomocniPacijent pomocni_pacijenti[B * b];
    memset(pomocni_pacijenti, 0, sizeof pomocni_pacijenti);
    int trenutni_pacijent = 0;
    ucitaj_pacijente(pacijenti_fp, pacijentBlok, pomocni_pacijenti);
    ucitaj_preglede(pregledi_fp, pregledBlok, pomocni_pacijenti);
    inicijalizuj_buckete(bucketi);
    pronadji_mesto_u_bucketu(pomocni_pacijenti, bucketi);
    for (int i = 0; i < B * b; i++) {
        if (bucketi[i].key != -1) {
            fwrite(&bucketi[i], sizeof(PacijentPregledSlog), 1, fp);
        }
    }
    fclose(fp);
    fclose(pacijenti_fp);
    fclose(pregledi_fp);
}
void pronadji_mesto_u_bucketu(PomocniPacijent* pomocni_pacijenti, PacijentPregledSlog* bucketi)
{
    for (int i = 0; i < B * b; i++) {
        int broj_kartona = pomocni_pacijenti[i].pacijent.broj_kartona;
        if (broj_kartona == 0) {
            continue;
        }
        int bucket_index = pomocni_pacijenti[i].pacijent.broj_kartona % B;
        int init_idx = bucket_index;
        int ind_psl = 0;
        for (int j = 0; j < B; j++) {
            for (int slog = 0; slog < b; slog++) {
                int index = bucket_index * b + slog;
                if (bucketi[index].obrisan != NIJE_OBRISAN) {
                    continue;
                }
                if (bucketi[index].key == -1) {
                    bucketi[index].key = broj_kartona;
                    bucketi[index].pacijent_pregled.broj_kartona = pomocni_pacijenti[i].pacijent.broj_kartona;
                    bucketi[index].pacijent_pregled.tezina = pomocni_pacijenti[i].pacijent.tezina;
                    bucketi[index].pacijent_pregled.visina = pomocni_pacijenti[i].pacijent.visina;
                    strcpy(bucketi[index].pacijent_pregled.ime, pomocni_pacijenti[i].pacijent.ime);
                    strcpy(bucketi[index].pacijent_pregled.prezime, pomocni_pacijenti[i].pacijent.prezime);
                    strcpy(bucketi[index].pacijent_pregled.JMBG, pomocni_pacijenti[i].pacijent.JMBG);
                    if (pomocni_pacijenti[i].br_pregleda == 0) {
                        bucketi[index].pacijent_pregled.prosecan_dijastolni = 0;
                        bucketi[index].pacijent_pregled.prosecan_sistolni = 0;
                    } else {
                        bucketi[index].pacijent_pregled.br_pregleda = pomocni_pacijenti[i].br_pregleda;
                        bucketi[index].pacijent_pregled.prosecan_dijastolni = pomocni_pacijenti[i].dijastolni_suma / pomocni_pacijenti[i].br_pregleda;
                        bucketi[index].pacijent_pregled.prosecan_sistolni = pomocni_pacijenti[i].sistolni_suma / pomocni_pacijenti[i].br_pregleda;
                    }
                    ind_psl = 1;
                    break;
                }
            }
            if (ind_psl) {
                break;
            }
            bucket_index = (bucket_index + k) % B;
            if (bucket_index == init_idx) {
                break;
            }
        }
    }
}
void inicijalizuj_buckete(PacijentPregledSlog* bucketi)
{
    PacijentPregled pacijent_pregled = { 0 };
    for (int i = 0; i < B * b; i++) {
        bucketi[i].key = -1;
        bucketi[i].obrisan = NIJE_OBRISAN;
        bucketi[i].pacijent_pregled = pacijent_pregled;
    }
}
void ucitaj_preglede(FILE* pregledi_fp, PregledSlog* pregledBlok, PomocniPacijent* pomocni_pacijenti)
{
    while (fread(pregledBlok, sizeof(PregledSlog), f2, pregledi_fp) == f2) {
        for (int i = 0; i < f2; i++) {
            if (pregledBlok[i].obrisan != NIJE_OBRISAN) {
                continue;
            }
            if (pregledBlok[i].key == KRAJ_DATOTEKE) {
                break;
            }
            int index = pronadji_pomocnog_pacijenta(pregledBlok[i].key, pomocni_pacijenti);
            if (index != -1) {
                pomocni_pacijenti[index].sistolni_suma += pregledBlok[i].pregled.sistolni_pritisak;
                pomocni_pacijenti[index].dijastolni_suma += pregledBlok[i].pregled.dijastolni_pritisak;
                pomocni_pacijenti[index].br_pregleda++;
            }
        }
    }
}
void ucitaj_pacijente(FILE* pacijenti_fp, PacijentSlog* pacijentBlok, PomocniPacijent* pomocni_pacijenti)
{
    int trenutni_pacijent = 0;
    while (fread(pacijentBlok, sizeof(PacijentSlog), f1, pacijenti_fp) == f1) {
        for (int i = 0; i < f1; i++) {
            if (pacijentBlok[i].obrisan != NIJE_OBRISAN) {
                continue;
            }
            if (pacijentBlok[i].key == KRAJ_DATOTEKE) {
                break;
            }
            if (trenutni_pacijent >= B * b) {
                break;
            }
            pomocni_pacijenti[trenutni_pacijent].pacijent.broj_kartona = pacijentBlok[i].pacijent.broj_kartona;
            pomocni_pacijenti[trenutni_pacijent].pacijent.tezina = pacijentBlok[i].pacijent.tezina;
            pomocni_pacijenti[trenutni_pacijent].pacijent.visina = pacijentBlok[i].pacijent.visina;
            strcpy(pomocni_pacijenti[trenutni_pacijent].pacijent.ime, pacijentBlok[i].pacijent.ime);
            strcpy(pomocni_pacijenti[trenutni_pacijent].pacijent.prezime, pacijentBlok[i].pacijent.prezime);
            strcpy(pomocni_pacijenti[trenutni_pacijent].pacijent.JMBG, pacijentBlok[i].pacijent.JMBG);
            pomocni_pacijenti[trenutni_pacijent].sistolni_suma = 0;
            pomocni_pacijenti[trenutni_pacijent].dijastolni_suma = 0;
            trenutni_pacijent++;
        }
    }
}
int nadji_slobodan_index(FILE* fp, int* counter, int broj_kartona)
{
    int trazenje_counter = 0;
    int bucket_index = broj_kartona % B;
    int init_idx = bucket_index;
    PacijentPregledSlog slog;
    PacijentSlog* pslog = pronadji_slog_pacijent(pacijenti_filename, broj_kartona);
    if (pslog != NULL) {
        evidentiraj_pristup_datoteci("log.dat", "trazenje", broj_kartona, trazenje_counter);
        return -1;
    }
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < b; j++) {
            int index = bucket_index * b + j;
            long offset = index * sizeof(PacijentPregledSlog);
            fseek(fp, offset, SEEK_SET);
            fread(&slog, sizeof(PacijentPregledSlog), 1, fp);
            trazenje_counter++;
            (*counter)++;
            if (slog.key == -1 || slog.obrisan != NIJE_OBRISAN) {
                evidentiraj_pristup_datoteci("log.dat", "trazenje", broj_kartona, trazenje_counter);
                return index;
            }
        }
        bucket_index = (bucket_index + k) % B;
        if (bucket_index == init_idx) {
            break;
        }
    }

    evidentiraj_pristup_datoteci("log.dat", "trazenje", broj_kartona, trazenje_counter);
    return -1;
}
void upisi_slog_rasuta(PacijentPregledSlog* slog, int* counter, const char* filename, int* status)
{
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    int index = nadji_slobodan_index(fp, counter, slog->pacijent_pregled.broj_kartona);
    if (index == -1) {
        *status = 3;
        printf("Nema slobodnog mesta u datoteci.\n");
        fclose(fp);
        return;
    }
    slog->obrisan = NIJE_OBRISAN;
    slog->key = slog->pacijent_pregled.broj_kartona;
    fseek(fp, index * sizeof(PacijentPregledSlog), SEEK_SET);
    fwrite(slog, sizeof(PacijentPregledSlog), 1, fp);
    (*counter)++;
    *status = 0;
    fclose(fp);
}
void prikazi_prosecan_pritisak(const char filename[], int broj_kartona, int* status)
{
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    int bucket = broj_kartona % B;
    int init_idx = bucket;
    PacijentPregledSlog slog;
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < b; j++) {
            int index = bucket * b + j;
            long offset = index * sizeof(PacijentPregledSlog);
            fseek(fp, offset, SEEK_SET);
            fread(&slog, sizeof(PacijentPregledSlog), 1, fp);

            if (slog.key == broj_kartona && slog.pacijent_pregled.broj_kartona == broj_kartona && slog.obrisan == NIJE_OBRISAN) {
                printf("Ime: %s\n", slog.pacijent_pregled.ime);
                printf("Prezime: %s\n", slog.pacijent_pregled.prezime);
                printf("JMBG: %s\n", slog.pacijent_pregled.JMBG);
                printf("Broj kartona: %d\n", slog.pacijent_pregled.broj_kartona);
                printf("Prosecni sistolni pritisak: %.2f\n", slog.pacijent_pregled.prosecan_sistolni);
                printf("Prosecni dijastolni pritisak: %.2f\n", slog.pacijent_pregled.prosecan_dijastolni);

                printf("Adresa baketa: %d, \n Broj sloga: %d \n", bucket, j);
                fclose(fp);
                *status = 0;
                return;
            }
        }
        bucket = (bucket + k) % B;
        if (bucket == init_idx) {
            break;
        }
    }
    *status = 1;
    printf("Pacijent sa datim brojem kartona nije pronadjen");
    fclose(fp);
}
int is_prekoracilac(int index, PacijentPregledSlog slog)
{
    int bucket_od_ključa = slog.key % B;
    int bucket_trenutni = index / b;
    return bucket_trenutni != bucket_od_ključa;
}
void prikazi_tri_pregleda(const char filename[], int* status)
{
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    PacijentPregledSlog slog;
    int index = 0;
    int found = 0;
    while (fread(&slog, sizeof(PacijentPregledSlog), 1, fp) == 1) {
        int razlika = slog.pacijent_pregled.prosecan_sistolni - slog.pacijent_pregled.prosecan_dijastolni;

        if (slog.obrisan == NIJE_OBRISAN && slog.pacijent_pregled.br_pregleda >= 3 && razlika <= 25) {

            if (is_prekoracilac(index, slog)) {
                printf("Slog je prekoracilac!\n");
            }
            int bucket = slog.key % B;
            int slog_u_baketu = index % b;
            found++;
            PacijentSlog* pacijentSlog = pronadji_slog_pacijent(pacijenti_filename, slog.pacijent_pregled.broj_kartona);
            ispisi_pacijenta(pacijentSlog->pacijent);
            printf("Prosecni sistolni pritisak: %.2f\n", slog.pacijent_pregled.prosecan_sistolni);
            printf("Prosecni dijastolni pritisak: %.2f\n", slog.pacijent_pregled.prosecan_dijastolni);
            printf("Adresa baketa: %d\n", bucket);
            printf("Broj sloga: %d\n", slog_u_baketu);
            *status = 0;
        }
        index++;
    }
    if (found == 0) {
        printf("Nije pronadjen nijedan takav pacijent\n");
        *status = 2;
    }
    fclose(fp);
}
void logicki_obrisi_slog(const char filename[], int broj_kartona, int* status)
{
    int counter = 0;
    int trazenje_counter = 0;
    FILE* fp = fopen(filename, "rb+");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    int bucket = broj_kartona % B;
    int init_idx = bucket;
    PacijentPregledSlog slog;
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < b; j++) {
            int index = bucket * b + j;
            long offset = index * sizeof(PacijentPregledSlog);
            fseek(fp, offset, SEEK_SET);
            fread(&slog, sizeof(PacijentPregledSlog), 1, fp);
            counter++;
            trazenje_counter++;
            if (slog.key == broj_kartona && slog.pacijent_pregled.broj_kartona == broj_kartona && slog.obrisan == NIJE_OBRISAN) {
                evidentiraj_pristup_datoteci("log.dat", "trazenje", broj_kartona, trazenje_counter);
                slog.obrisan = OBRISAN;
                fseek(fp, offset, SEEK_SET);
                fwrite(&slog, sizeof(PacijentPregledSlog), 1, fp);
                counter++;
                *status = 0;
                fclose(fp);
                evidentiraj_pristup_datoteci("log.dat", "brisanje", broj_kartona, counter);
                return;
            }
        }
        bucket = (bucket + k) % B;
        if (bucket == init_idx) {
            break;
        }
    }
    *status = 1;
    printf("Pacijent sa datim brojem kartona nije pronadjen");
    evidentiraj_pristup_datoteci("log.dat", "brisanje", broj_kartona, counter);
    fclose(fp);
}
void propagiraj_pacijenta_u_rasutu(PacijentSlog pacijentSlog, const char rasuta_datoteka[], const char operacija[], int* status)
{
    int counter = 0;
    int trazenje_counter = 0;
    FILE* fp = fopen(rasuta_datoteka, "rb+");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    int broj_kartona = pacijentSlog.key;
    printf("Broj kartona: %d\n", broj_kartona);
    int bucket = broj_kartona % B;
    int init_idx = bucket;

    PacijentPregledSlog slog;
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < b; j++) {
            int index = bucket * b + j;
            long offset = index * sizeof(PacijentPregledSlog);
            if (strcmp(operacija, "modifikacija") == 0) {
                fseek(fp, offset, SEEK_SET);
                fread(&slog, sizeof(PacijentPregledSlog), 1, fp);
                trazenje_counter++;
                counter++;
                if (slog.key == broj_kartona && slog.pacijent_pregled.broj_kartona == broj_kartona && slog.obrisan == NIJE_OBRISAN) {
                    evidentiraj_pristup_datoteci("log.dat", "trazenje", broj_kartona, trazenje_counter);
                    slog.pacijent_pregled.tezina = pacijentSlog.pacijent.tezina;
                    slog.pacijent_pregled.visina = pacijentSlog.pacijent.visina;
                    strcpy(slog.pacijent_pregled.ime, pacijentSlog.pacijent.ime);
                    strcpy(slog.pacijent_pregled.prezime, pacijentSlog.pacijent.prezime);
                    strcpy(slog.pacijent_pregled.JMBG, pacijentSlog.pacijent.JMBG);
                    slog.pacijent_pregled.broj_kartona = pacijentSlog.pacijent.broj_kartona;
                    slog.obrisan = NIJE_OBRISAN;
                    fseek(fp, offset, SEEK_SET);
                    fwrite(&slog, sizeof(PacijentPregledSlog), 1, fp);
                    counter++;

                    evidentiraj_pristup_datoteci("log.dat", operacija, broj_kartona, counter);
                    *status = 0;
                    fclose(fp);
                    return;
                }
            }
            if (strcmp(operacija, "upis") == 0) {
                PacijentPregledSlog slog = { 0 };
                slog.pacijent_pregled.broj_kartona = pacijentSlog.pacijent.broj_kartona;
                slog.pacijent_pregled.tezina = pacijentSlog.pacijent.tezina;
                slog.pacijent_pregled.visina = pacijentSlog.pacijent.visina;
                slog.key = pacijentSlog.key;
                printf("evo me ovde, %d %d", slog.key, pacijentSlog.key);
                slog.obrisan = NIJE_OBRISAN;
                strcpy(slog.pacijent_pregled.ime, pacijentSlog.pacijent.ime);
                strcpy(slog.pacijent_pregled.prezime, pacijentSlog.pacijent.prezime);
                strcpy(slog.pacijent_pregled.JMBG, pacijentSlog.pacijent.JMBG);
                // upisi_slog_rasuta(&slog, &counter, rasuta_datoteka, status);
                fseek(fp, offset, SEEK_SET);
                fwrite(&slog, sizeof(PacijentPregledSlog), 1, fp);
                counter++;
                *status = 0;
                *status = 0;
                evidentiraj_pristup_datoteci("log.dat", operacija, broj_kartona, counter);
                fclose(fp);
                return;
            }
        }
        bucket = (bucket + k) % B;
        if (bucket == init_idx) {
            break;
        }
    }

    *status = 1;
    printf("Pacijent sa datim brojem kartona nije pronadjen");
    evidentiraj_pristup_datoteci("log.dat", operacija, broj_kartona, counter);
    fclose(fp);
}
void propagiraj_pregled_u_rasutu(const char rasuta_datoteka[], PregledSlog pregled_slog, int* status)
{
    int counter = 0;
    int trazenje_counter = 0;
    FILE* fp = fopen(rasuta_datoteka, "rb+");
    if (fp == NULL) {
        *status = 2;
        return;
    }
    int broj_kartona = pregled_slog.pregled.broj_kartona;
    int bucket = broj_kartona % B;
    int init_idx = bucket;

    PacijentPregledSlog slog;
    for (int i = 0; i < B; i++) {
        for (int j = 0; j < b; j++) {
            int index = bucket * b + j;
            long offset = index * sizeof(PacijentPregledSlog);
            fseek(fp, offset, SEEK_SET);
            fread(&slog, sizeof(PacijentPregledSlog), 1, fp);
            trazenje_counter++;
            counter++;
            if (slog.key == broj_kartona && slog.pacijent_pregled.broj_kartona == broj_kartona && slog.obrisan == NIJE_OBRISAN) {
                evidentiraj_pristup_datoteci("log.dat", "trazenje", broj_kartona, trazenje_counter);
                slog.pacijent_pregled.br_pregleda++;
                slog.obrisan = NIJE_OBRISAN;
                // Dodajemo nove vrednosti u sumu
                slog.pacijent_pregled.suma_sistolni += pregled_slog.pregled.sistolni_pritisak;
                slog.pacijent_pregled.suma_dijastolni += pregled_slog.pregled.dijastolni_pritisak;

                // Računamo prosek deljenjem sume sa brojem pregleda
                slog.pacijent_pregled.prosecan_sistolni = slog.pacijent_pregled.suma_sistolni / slog.pacijent_pregled.br_pregleda;
                slog.pacijent_pregled.prosecan_dijastolni = slog.pacijent_pregled.suma_dijastolni / slog.pacijent_pregled.br_pregleda;
                fseek(fp, offset, SEEK_SET);
                fwrite(&slog, sizeof(PacijentPregledSlog), 1, fp);
                counter++;
                *status = 0;
                fclose(fp);
                evidentiraj_pristup_datoteci("log.dat", "upis", broj_kartona, counter);
                return;
            }
        }
        bucket = (bucket + k) % B;
        if (bucket == init_idx) {
            break;
        }
    }

    *status = 1;
    printf("Pacijent sa datim brojem kartona nije pronadjen");
    evidentiraj_pristup_datoteci("log.dat", "upis", broj_kartona, counter);
    fclose(fp);
}
