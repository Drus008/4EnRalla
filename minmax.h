#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "4enratlla.h"
#include "partides.h"

#include <stdbool.h>


#define PROFUNDITAT 4



/**
 * @brief Comença l'algorisme del MinMax
 * 
 * @param partida l'estat de la partida en la que es fa el minmax.
 * @param jugadorOriginal és el jugador al que li toca tirar.
 * @param fHeuristica és la funció heurísitca que s'utilitza per a fer el minmax.
 */
int minMax(QuatreEnRatlla *partida, char jugadorOriginal, funcioHeuristica fHeuristica, void *ctxHeuristica);

/**
 * @brief realitza l'algorisme del MinMax
 * 
 * Aniria bé explicar-ho millor aquí
 * 
 * @param arbre és el node en el que s'aplicarà el minmax recursivament.
 * @param partida és l'estat acutal de la partida (sense modificar el moviment actual).
 * @param jugador és el jugador que farà el moviment actual
 * 
 * @param retorna el int del moviment que ha trobat l'algorisme que és el millor
 * 
 * Segurament estaria bé separar la funció en 2, una que retorna int i és la que s'executa des de fora
 * i una altre que es la que fa el MinMax recursiu.
 * 
 */
int iteracioMinmax(QuatreEnRatlla *partida, char jugadorOriginal, int nivellNode, double *puntuacioNode, int *profunditatNode, funcioHeuristica fHeuristica, void *ctxHeuristica);


/** S'Ha de reescriure la descripcio!
 * @brief Gestiona les arrels de l'arbre
 * Donat un node comprova si:
 * - El moviment no es pot fer (i aleshores assigna -INF en cas de ser el jugador original i INF en cas de ser l'altre)
 * - El moviment provoca una victoria (i aleshores assigna INF en cas de ser el jugador original i -INF en cas de ser l'altre)
 * - El moviment es de la profunditat màxima de l'arbre (i aleshores assigna la punuació corresponent si el jugador és l'original i l'oposat si és l'altre)
 * 
 * En cas de detectar alguna d'aquestes coses s'assigna fulla=true al node i és retorna true
 * En cas contrari retorna false.
 * 
 * No modifica l'estat acutal de la partida.
 * 
 * @param arbre és el node de l'arbre que es configurarà.
 * @param partida és l'estat actual de la partida (sense haver realitzat encara el moviment del node).
 * @param jugador és el jugador que està fent el moviment actual.
 * 
 * @return retorna cert en cas de que el node sigui una fulla i false en cas contrari.
 */
bool omplirNodeTrivial(QuatreEnRatlla *partida, int moviment, char jugadorOriginal, double *valoracio, int *nMoviments, int profunditat, funcioHeuristica fHeuristica, void *ctxHeuristica);

void triaMillorTirada(int nivellNode, int *millorMoviment, int movimentActual, double *millorValoracio, double valoracioActual, int *accioDesempat, int accioDesempatActual);



#endif // MIN_MAX_H