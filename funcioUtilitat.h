#ifndef FUNCIO_UTILITAT_H
#define FUNCIO_UTILITAT_H

#include "4enratlla.h"
#define KER_DIM 5
#define HALF_KER (int)KER_DIM/2



/**
 * @brief Compta quantes fitxer de cada tipos hi ha al voltant d'una posició en concret
 * 
 * @param partida és l'estat de partida actual
 * @param fila fila on es la posició que es vol comprovar
 * @param col columna on es la posició que es vol comprovar
 * @param fitxesJ1 direcció per on retorna el nombre de fitxes del J1
 * @param fitxesJ1 direcció per on retorna el nombre de fitxes del J2
 * 
 */
void comptarFitxesAdjacents(QuatreEnRatlla *partida, int fila, int col, int* fitxesJ1, int* fitxesJ2);

// Es podria millorar fent que no comproves casselles que estàn molt més amunt que les últimes tirades

/**
 * @brief Valora un estat de partida
 * 
 * @param partida és l'estat que es vol valorar
 * 
 * per a fer el càlcul, per a cada casella es compten quantes n'hi ha de cada jugador i es resten les d'un amb les de l'altre elevades al quadrat
 * 
 * @return la valoració de la partida, on un nombre positiu vol dir que J1 va guanyant i negatiu que J2 va guanyant
 */
double puntuacioPerAdjacencia(QuatreEnRatlla *partida, void *ctx);

#endif // FUNCIO_UTILITAT_H