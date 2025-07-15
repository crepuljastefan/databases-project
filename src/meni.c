#include "../include/meni.h"
#include "../include/datoteke.h"
#define f1 4
#define f2 6
#include <stdio.h>
void prikazi_meni()
{
    printf("Meni:\n");
    printf("1. Formiraj datoteku pacijenata\n");
    printf("2. Formiraj datoteku pregleda\n");
    printf("3. Upisi slog u datoteku pacijenata\n");
    printf("4. Upisi slog u datoteku pregleda\n");
    printf("5. Prikazi alergiju pacijenta\n");
    printf("6. Prikazi pacijente sa jednakim pritiskom\n");
    printf("7. Modifikuj podatke o pacijentu\n");
    printf("0. Izlaz iz programa\n");
}
void meni()
{
    int status = -1;
    int izbor;
    do {
        prikazi_meni();
        printf("Unesite izbor: ");
        scanf("%d", &izbor);

        char filename[100];

        Pacijent pacijent;
        PacijentSlog pacijent_slog;
        Pregled pregled;
        PregledSlog pregled_slog;
        switch (izbor) {
        case 1:
            printf("Unesite ime datoteke: ");
            scanf("%s", filename);
            formiraj_datoteku_pacijent(filename, f1, &status);
            printf("Formirana je datoteka pacijenata.\n");
            break;
        case 2:
            printf("Unesite ime datoteke: ");
            scanf("%s", filename);
            formiraj_datoteku_pregled(filename, f2, &status);
            printf("Formirana je datoteka pacijenata.\n");
            break;
        case 3:
            printf("Unesite ime datoteke: ");
            scanf("%s", filename);
            printf("Popunite podatke pacijenta:\n");
            printf("Ime:\n");
            scanf("%s", pacijent.ime);
            printf("Prezime:\n");
            scanf("%s", pacijent.prezime);
            printf("Broj kartona:\n");
            scanf("%d", &pacijent.broj_kartona);
            printf("JMBG:\n");
            scanf("%s", pacijent.JMBG);
            printf("Tezina:\n");
            scanf("%f", &pacijent.tezina);
            printf("Visina:\n");
            scanf("%f", &pacijent.visina);
            printf("Alergija na polen (da/ne):\n");
            scanf("%s", pacijent.alerg_polen);
            pacijent_slog.pacijent = pacijent;
            pacijent_slog.key = pacijent.broj_kartona;
            pacijent_slog.obrisan = 0;
            upisi_slog_datoteke_pacijenti(filename, &status, &pacijent_slog, f1);
            // upisi_slog_datoteke_pacijenti(filename, );
            break;
        case 4:
            printf("Unesite ime datoteke: ");
            scanf("%s", filename);
            printf("Popunite podatke pregleda:\n");
            printf("ID pregleda:\n");
            scanf("%d", &pregled.id);
            printf("Broj kartona:\n");
            scanf("%d", &pregled.broj_kartona);
            printf("Datum pregleda (dd.mm.yyyy):\n");
            scanf("%s", pregled.datum_pregleda);
            printf("Sistolni pritisak:\n");
            scanf("%d", &pregled.sistolni_pritisak);
            printf("Dijastolni pritisak:\n");
            scanf("%d", &pregled.dijastolni_pritisak);
            pregled_slog.pregled = pregled;
            pregled_slog.key = pregled.id;
            pregled_slog.obrisan = 0;
            upisi_slog_datoteke_pregledi(filename, &status, &pregled_slog, f2);
            break;
        case 5:
            // prikazi_alergiju_pacijenta();
            break;
        case 6:
            // prikazi_pacijente_jednak_pritisak();
            break;
        case 7:
            // modifikuj_pacijenta();
            break;
        case 0:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznat izbor, molimo pokusajte ponovo.\n");
        }
    } while (izbor != 0);
}
