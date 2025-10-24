#ifndef MIN_MAX_H
#define MIN_MAX_H

#include "4enratlla.h"

#include <stdbool.h>


#define PROFUNDITAT 6


/**
 * @brief Estructura per a fer un arbre per al MinMax
 * 
 */
typedef struct nodeArbre {
    int tirada; /**< Indica quina tirada s'ha fet per a arribar a aquesta posició. */
    struct nodeArbre** fills; /**< Array d'apuntadors a arbres que apareixen si fas un moviment des d'aquesta posició*/
    int nivell; /**< Quantitat de moviments que s'han fet des de la creació de l'arbre.*/
    int profunditatSolucio; /**< Si ha trobat un estat final, indica a quants passos es troba de l'actual.*/
    double puntuacio; /**< Puntuació assignada pel MinMax, pot ser -INF o +INF arriben a un estat final.*/
} nodeArbre;

/**
 * @brief Allibera els fills d'un arbre.
 * @param arbre és l'arbre del que vols alliberar els fills.
 * @param nFills és el nombre de fills que te l'arbre.
 */
void alliberarFills(nodeArbre *arbre, int nFills);

/**
 * @brief Imprimeix un arbre
 * @param arbre és l'arbre que es vol imprimir
 * @param profunditatMaxima és la profunditat fins a la que s'imprimeix l'arbre
 * @param nFills és el nombre de fills que te l'arbre.
 */
void imprimirArbre(nodeArbre* arbre, int profunditatMaxima, int nFills);


/**
 * @brief Comença l'algorisme del MinMax
 * 
 * @param partida l'estat de la partida en la que es fa el minmax
 * @param jugadorOriginal és el jugador al que li toca tirar
 * 
 */
int minMax(QuatreEnRatlla *partida, char jugadorOriginal);
 
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
int iteracioMinmax(nodeArbre *arbre, QuatreEnRatlla *partida, char jugador);


/**
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
bool omplirNodeTrivial(nodeArbre* arbre, QuatreEnRatlla *partida, char jugador);


/**
 * @brief Troba l'index del node fill amb una puntuació més alta
 * 
 * @param arbre L'arbre del que es vol trobar el fill amb la puntuació més alta
 * @param midaLlista Mida de la llista de la qual es vol trobar el màxim
 * 
 * @return l'index del moviment que provoca el node amb puntuació màxima
 * 
 * En cas d'empat, fa servir profunditat solució per a descartar el node que triga més passos en guanyar
 */
int trobarMaxim(nodeArbre *arbre, int midaLlista);
/**
 * @brief Troba l'index del node fill amb una puntuació més baixa
 * 
 * @param arbre L'arbre del que es vol trobar el fill amb la puntuació més baixa
 * @param midaLlista Mida de la llista de la qual es vol trobar el mínim
 * 
 * @return l'index del moviment que provoca el node amb puntuació mínima
 * 
 * En cas d'empat, fa servir profunditat solució per a descartar el node que triga menys passos en guanyar
 */
int trobarMinim(nodeArbre *arbre, int midaLlista);

int triaNaive(QuatreEnRatlla *partida);

#endif // MIN_MAX_H