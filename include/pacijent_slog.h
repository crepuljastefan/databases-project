#ifndef PACIJENT_SLOG_H
#define PACIJENT_SLOG_H
#define OBRISAN 1
#define NIJE_OBRISAN 0
#include "pacijent.h"
typedef struct {
    int key;
    int obrisan;
    Pacijent pacijent;
} PacijentSlog;
#endif // PACIJENT_SLOG_H
