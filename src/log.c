#include "../include/log.h"
#include "../include/datoteke.h"
#include <stdio.h>
#include <string.h>
static int global_id = 1;
void evidentiraj_pristup_datoteci(char filename[], const char naziv_operacije[], int broj_pristupa, int id_sloga)

{
    int status;
    FILE* fp = fopen("log.dat", "rb+");
    if (fp == NULL) {
        formiraj_datoteku_pacijent("log.dat", 7, &status);
        if (status != 0) {
            printf("Greska pri formiranju datoteke %s.\n", filename);
            return;
        }
    }
    fp = fopen("log.dat", "rb+");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke i nakon formiranja %s.\n", filename);
        return;
    }
    LogSlog log_slog;
    log_slog.log.id = global_id++;
    log_slog.log.id_za_pristup = id_sloga;
    strncpy(log_slog.log.naziv_operacije, naziv_operacije, sizeof(log_slog.log.naziv_operacije) - 1);
    log_slog.log.naziv_operacije[sizeof(log_slog.log.naziv_operacije) - 1] = '\0';
    log_slog.log.broj_pristupa = broj_pristupa;
    log_slog.key = log_slog.log.id;
    log_slog.obrisan = NIJE_OBRISAN;
    upisi_slog_datoteke_evidencije("log.dat", &status, &log_slog, 7);
    fclose(fp);
}
void prikazi_izvestaj_evidencija(const char filename[], int prag)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke %s.\n", filename);
        return;
    }
    LogSlog blok[7];
    StatistikaLog statistika[4] = { 0 };
    strcpy(statistika[0].naziv_operacije, "upis");
    strcpy(statistika[1].naziv_operacije, "brisanje");
    strcpy(statistika[2].naziv_operacije, "modifikacija");
    strcpy(statistika[3].naziv_operacije, "trazenje");
    while (fread(blok, sizeof(LogSlog), 7, fp) == 7) {
        for (int i = 0; i < 7; i++) {
            if (blok[i].key == -1) {
                fclose(fp);
                return;
            }
            if (blok[i].obrisan == OBRISAN) {
                continue; // Preskoci obrisane slogove
            }
            for (int j = 0; j < 4; j++) {
                if (strcmp(statistika[j].naziv_operacije, blok[i].log.naziv_operacije) == 0) {
                    statistika[j].broj_slogova++;
                    statistika[j].broj_pristupa += blok[i].log.broj_pristupa;
                }
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (statistika[i].broj_slogova > 0) {
            statistika[i].prosecno_pristupa = (float)statistika[i].broj_pristupa / statistika[i].broj_slogova;
        }
        if (strcmp(statistika[i].naziv_operacije, "trazenje") == 0 && statistika[i].prosecno_pristupa > prag) {
            printf("Prosečan broj pristupa za operaciju 'trazenje' je %.2f, što je veće od praga %d.\n", statistika[i].prosecno_pristupa, prag);
            // Pokreni reorganizaciju rasute datoteke ovde
        }
        printf("Operacija: %s, Broj slogova: %d, Broj pristupa: %d, Prosečan broj pristupa: %.2f\n",
            statistika[i].naziv_operacije, statistika[i].broj_slogova,
            statistika[i].broj_pristupa, statistika[i].prosecno_pristupa);
    }
}
