#include "../include/meni.h"
#include "../include/rasute_datoteke.h"
#include <string.h>
#define f1 4
#define f2 6
#include <stdio.h>
void prikazi_meni_rasute()
{
    printf("Meni:\n");
    printf("1. Formiraj rasutu datoteku\n");
    printf("2. Upisi slog u rasutu datoteku \n");
    printf("3. Prikazi prosecan sistolni i dijastolni pritisak pacijenta\n");
    printf("4. Prikaz svih pacijenata koji su bili na barem 3 pregleda,\n a razlika izmedju sistolnog i dijastolnog pritiska je manja ili jednaka 25\n");
    printf("5. Logičko brisanje aktuelnog sloga iz aktivne datoteke\n");
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

        PacijentSlog pacijent_slog;
        Pregled pregled;
        PregledSlog pregled_slog;
        PacijentPregledSlog pacijent_pregled_slog = { 0 };

        char filename[100];
        switch (izbor) {
        case 1:
            printf("Unesite ime datoteke: \n");
            scanf("%s", filename);
            set_rasuta_file(filename);
            formiraj_rasutu_datoteku(filename, pregledi_filename, pacijenti_filename, &status);
            printf("Formirana je rasuta datoteka pacijenata i pregleda.\n");
            break;
        case 2:
            if (rasute_filename[0] == '\0') {
                printf("Rasuta datoteka nije formirana. Molimo prvo formirajte rasutu datoteku.\n");
                break;
            }
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
            printf("Unesite broj pregleda: \n");
            scanf("%d", &pacijent_pregled_slog.pacijent_pregled.br_pregleda);
            int counter = 0;
            upisi_slog_rasuta(&pacijent_pregled_slog, &counter, rasute_filename, &status);
            break;

        case 3:

            if (rasute_filename[0] == '\0') {
                printf("Rasuta datoteka nije formirana. Molimo prvo formirajte rasutu datoteku.\n");
                break;
            }
            printf("Unesite broj kartona pacijenta: \n");
            int broj_kartona;
            scanf("%d", &broj_kartona);
            prikazi_prosecan_pritisak(rasute_filename, broj_kartona, &status);
            break;
        case 4:
            if (rasute_filename[0] == '\0') {
                printf("Rasuta datoteka nije formirana. Molimo prvo formirajte rasutu datoteku.\n");
                break;
            }
            printf("Prikaz svih pacijenata koji su bili na barem 3 pregleda, a razlika izmedju sistolnog i dijastolnog pritiska je manja ili jednaka 25.\n");
            prikazi_tri_pregleda(rasute_filename, &status);
            if (status == 1) {
                printf("Nema pacijenata koji ispunjavaju uslove.\n");
            }
            break;
        case 5:
            if (rasute_filename[0] == '\0') {
                printf("Rasuta datoteka nije formirana. Molimo prvo formirajte rasutu datoteku.\n");
                break;
            }
            printf("Unesite broj kartona pacijenta za brisanje: \n");
            int broj_kartona_br;
            scanf("%d", &broj_kartona_br);
            logicki_obrisi_slog(rasute_filename, broj_kartona_br, &status);
            if (status == 0) {
                printf("Slog je uspesno obrisan.\n");
            } else if (status == 1) {
                printf("Pacijent sa brojem kartona %d nije pronadjen.\n", broj_kartona_br);
            } else {
                printf("Doslo je do greske prilikom brisanja sloga.\n");
            }
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor, molimo pokusajte ponovo.\n");
        }
    } while (izbor != 0);
}
