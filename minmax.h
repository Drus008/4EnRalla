#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "4enratlla.h"

#include <stdbool.h>


#define PROFUNDITAT 6


typedef struct arbre {
    int nivell;
    double puntuacio;
    int tirada;
    struct arbre* fills;
    bool fulla;
} Arbre;






void imprimirArbre(Arbre* arbre, int prof);

bool omplirNodeTrivial(Arbre* arbre, QuatreEnRatlla *partida, char jugador);

int ferMinmax(Arbre *arbre, QuatreEnRatlla *partida, char jugador);

int trobarMaxim(double llista[NCOLS]);

int trobarMinim(double llista[NCOLS]);

int triaNaive(QuatreEnRatlla *partida);

#endif // MIN_MAX_H