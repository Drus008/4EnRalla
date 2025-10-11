#ifndef FUNCIO_UTILITAT_H
#define FUNCIO_UTILITAT_H

#include "4enratlla.h"
#define KER_DIM 5
#define HALF_KER (int)KER_DIM/2

float utilitat(QuatreEnRatlla *partida, char jugador);



// Donada una partida, una fila i una columna, retorna per entrada el nombre de fitxes de cada jugador adjacents a la donada
void comptarFitxesAdjacents(QuatreEnRatlla *partida, int fila, int col, int* fitxesJ1, int* fitxesJ2);

// Retorna una puntuació de l'estat de la partida en funció de les fitxes dayacents de cada jugador
int puntuacioPerAdjacencia(QuatreEnRatlla *partida);

#endif // FUNCIO_UTILITAT_H