#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "4enratlla.h"
#include "partides.h"

#include <stdbool.h>


#define PROFUNDITAT 3

/**
 * @file minmax.h
 * @todo s'hauria de ficar la prodfunditat com a paràmetre.
 */

/**
 * @brief Comença l'algorisme del MinMax
 * 
 * @param partida l'estat de la partida en la que es fa el minmax.
 * @param jugadorOriginal és el jugador al que li toca tirar.
 * @param fHeuristica és la funció heurísitca que s'utilitza per a fer el minmax.
 * @param ctxHeuristica és una estructura que conté dades necesàries per a la funció heurística.
 */
int minMax(QuatreEnRatlla *partida, signed char jugadorOriginal, funcioHeuristica fHeuristica, void *ctxHeuristica);

/**
 * @brief realitza l'algorisme del MinMax
 * 
 * @todo
 * 
 * @param partida és l'estat acutal de la partida (sense modificar el moviment actual).
 * @param jugadorOriginal és el jugador al que li toca fer el moviment a l'inici del minmax.
 * @param nivellNode és a quina profunditat de l'arbre ens trobem.
 * @param puntuacioNode és un apuntador que conté la puntuació assignada al node.
 * @param profunditatNode és un apuntador a la profunditat a la que es ha rebut el valor del minmax.
 * @param fHeuristica és la funció heurística que s'usa per a valorar nodes.
 * @param ctxHeuristica son altres paràmetres que pot necessitar la heurísitca.
 * 
 * @param retorna el int del moviment que ha trobat l'algorisme que és el millor
 * 
 * 
 */
int iteracioMinmax(QuatreEnRatlla *partida, signed char jugadorOriginal, int nivellNode, double *puntuacioNode, int *profunditatNode, funcioHeuristica fHeuristica, void *ctxHeuristica);


/**
 * @brief Gestiona les arrels de l'arbre
 * Donat un node comprova si:
 * - El moviment provoca una victoria (i aleshores assigna INF en cas de ser el jugador original i -INF en cas de ser l'altre)
 * - El moviment es de la profunditat màxima de l'arbre (i aleshores assigna la punuació corresponent si el jugador és l'original és el primer i l'oposat si és l'altre)
 * 
 * En cas de detectar alguna d'aquestes coses s'assigna fulla=true al node i és retorna true
 * En cas contrari retorna false.
 * 
 * No modifica l'estat acutal de la partida.
 * 
 * @param partida és l'estat de la partida que es vol puntuar.
 * @param moviment és el l'últim moviment fet per a arribar a aquest estat.
 * @param jugadorOriginal és el jugador que està fent el moviment actual.
 * @param valoracio és el un aptuntador per on es retornarà la valoració del node.
 * @param profunditat és a la profunditat que ens trobem a l'arbre.
 * @param fHeuristica és la funció heurística que s'usarà per a puntuar les fulles.
 * @param ctxHeuristica son altres paràmetres que pot requerir la heurística.
 * 
 * @return retorna cert en cas de que el node sigui una fulla i false en cas contrari.
 */
bool detectarArrel(QuatreEnRatlla *partida, int moviment, signed char jugadorOriginal, double *valoracio, int profunditat, funcioHeuristica fHeuristica, void *ctxHeuristica);


/**
 * @brief Compara el node actual amb el millor d'aquella bifuració.
 * 
 * @param millorMoviment és el millor moviment fet en aquella bifuracació. Si el moviment actual és millor, actualitza el valor
 * @param movimentActual és el moviment fet per a arribar a aquell estat.
 * @param millorValoracio és la valoració del millor moviment d'aquella biuració.
 * @param valoracioActual és la valoració del node actual.
 * @param accioDesempat és la profunditat relativa a la que s'ha arribat a la millor valoració de la bifurcació.
 * @param accioDesempatActual és la profunditat relativa a la que s'ha arribat a la fulla corresponent al node actual.
 * 
 */
void triaMillorTirada(int *millorMoviment, int movimentActual, double *millorValoracio, double valoracioActual, int *accioDesempat, int accioDesempatActual);



#endif // MIN_MAX_H