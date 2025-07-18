#ifndef PACIJENT_PREGLED_SLOG_H
#define PACIJENT_PREGLED_SLOG_H
#define OBRISAN 1
#define NIJE_OBRISAN 0
#include "pacijent_pregled.h"
typedef struct {
    int key;
    int obrisan;
    PacijentPregled pacijent_pregled;
} PacijentPregledSlog;
#endif // PACIJENT_PREGLED_SLOG_H
