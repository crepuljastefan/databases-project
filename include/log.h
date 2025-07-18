#ifndef LOG_H
#define LOG_H
typedef struct {
    int id;
    int id_za_pristup;
    char naziv_operacije[16];
    int broj_pristupa;
} Log;
typedef struct {
    Log log;
    int key;
    int obrisan;
} LogSlog;
typedef struct {
    char naziv_operacije[16];
    int broj_slogova;
    int broj_pristupa;
    float prosecno_pristupa;
} StatistikaLog;
void evidentiraj_pristup_datoteci(char filename[], const char naziv_operacije[], int broj_pristupa, int id_sloga);
void prikazi_izvestaj_evidencija(const char filename[], int prag);
#endif
