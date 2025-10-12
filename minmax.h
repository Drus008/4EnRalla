#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "4enratlla.h"

#include <stdbool.h>


#define PROFUNDITAT 6


typedef struct arbre {
    int nivell;
    int profunditatSolucio;
    double puntuacio;
    int tirada;
    struct arbre* fills;
    bool fulla;
} Arbre;






void imprimirArbre(Arbre* arbre, int prof, int n);

bool omplirNodeTrivial(Arbre* arbre, QuatreEnRatlla *partida, char jugador, char jugadorOriginal);

int ferMinmax(Arbre *arbre, QuatreEnRatlla *partida, char jugador);

int trobarMaxim(Arbre *arbre);

int trobarMinim(Arbre *arbre);

int triaNaive(QuatreEnRatlla *partida);

#endif // MIN_MAX_H