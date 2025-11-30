#ifndef CONEXIO_XARXA_4ENRATLLA
#define CONEXIO_XARXA_4ENRATLLA

#include "4enratlla.h"

/**
 * @file conexioXarxa4EnRatlla.h
 */


/**
 * @brief Wraper que serveix per a donar a una xarxa el tipus de funció heurísica
 * 
 * @param partida és la partida a la que es vol aplicar la xarxa.
 * @param xarxaCtx ha der ser un apuntador a una xarxaNeuronal, que es la que s'aplicarà a la partida.
 * 
 * 
 * 
 * @return la puntuació a l'estat actual de la partida
 */
double wrapperXarxa(QuatreEnRatlla *partida, void *xarxaCtx);


#endif //CONEXIO_XARXA_4ENRATLLA