#include "../include/meni.h"
#include "../include/rasute_datoteke.h"
#define f1 4
#define f2 6
#include <stdio.h>
void prikazi_meni_rasute()
{
    printf("Meni:\n");
    printf("1. Formiraj datoteku pacijenata\n");
    printf("2. Formiraj datoteku pregleda\n");
    printf("3. Upisi slog u datoteku pacijenata\n");
    printf("0. Izlaz iz rada sa rasutim datotekama\n");
}
void meni_rasute()
{
    int status = -1;
    int izbor;
    do {
        prikazi_meni_rasute();
        printf("Unesite izbor: ");
        scanf("%d", &izbor);

        char pregledi_filename[100];
        char pacijenti_filename[100];
        Pacijent pacijent;
        PacijentSlog pacijent_slog;
        Pregled pregled;
        PregledSlog pregled_slog;
        PacijentPregledSlog pacijent_pregled_slog;
        switch (izbor) {
        case 1:
            printf("Unesite ime datoteke: \n");
            char filename[100];
            scanf("%s", filename);
            formiraj_rasutu_datoteku(filename, "pregledi.dat", "pacijenti.dat", &status);
            printf("Formirana je rasuta datoteka pacijenata i pregleda.\n");
            break;
        case 2:
            printf("Unesite broj kartona: \n");
            scanf("%d", &pacijent_pregled_slog.pacijent_pregled.broj_kartona);
            printf("Unesite prosecan sistolni pritisak: \n");
            scanf("%f", &pacijent_pregled_slog.pacijent_pregled.prosecan_sistolni);
            printf("Unesite prosecan dijastolni pritisak: \n");
            scanf("%f", &pacijent_pregled_slog.pacijent_pregled.prosecan_dijastolni);
            printf("Unesite ime pacijenta: \n");
            scanf("%s", pacijent_pregled_slog.pacijent_pregled.ime);
            printf("Unesite prezime pacijenta: \n");
            scanf("%s", pacijent_pregled_slog.pacijent_pregled.prezime);
            printf("Unesite JMBG pacijenta: \n");
            scanf("%s", pacijent_pregled_slog.pacijent_pregled.JMBG);
            printf("Unesite tezinu pacijenta: \n");
            scanf("%f", &pacijent_pregled_slog.pacijent_pregled.tezina);
            printf("Unesite visinu pacijenta: \n");
            scanf("%f", &pacijent_pregled_slog.pacijent_pregled.visina);
            printf("Unesite prosek sistolnog pritiska: \n");
            scanf("%f", &pacijent_pregled_slog.pacijent_pregled.prosecan_sistolni);
            printf("Unesite prosek dijastolnog pritiska: \n");
            scanf("%f", &pacijent_pregled_slog.pacijent_pregled.prosecan_dijastolni);
            printf("Unesite broj pregleda: \n");
            scanf("%d", &pacijent_pregled_slog.pacijent_pregled.br_pregleda);
            upisi_slog_rasuta(&pacijent_pregled_slog, filename, &status);
            break;

        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor, molimo pokusajte ponovo.\n");
        }
    } while (izbor != 0);
}
