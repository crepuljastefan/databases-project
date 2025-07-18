#ifndef PREGLED_SLOG_H
#define PREGLED_SLOG_H
#define OBRISAN 1
#define NIJE_OBRISAN 0
#include "pregled.h"
typedef struct {
    int key; // Ključ za pretragu
    int obrisan;
    Pregled pregled;
} PregledSlog;
#endif // PREGLED_SLOG_H
//
