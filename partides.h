#ifndef PARTIDES_H
#define PARTIDES_H

typedef int (*selectorDeMoviment)(QuatreEnRatlla*, char);

/**
 * @brief Pregunta al jugador quin moviment vol fer
 * 
 * No para de preguntar fins a rebre una jugada válidad. En cas de no rebre una jugada vàlida imprimeix per
 * consolo si es per un error ortogràfic o per una columna de fora del taulell.
 * 
 * @param partida és un apuntador a la partida que s'està jugant
 * 
 * @param jugador és el jugador que fa el moviment
 * 
 * @return el número de la columna que el jugador ha dit que vol tirar. Sempre retorna un moviment vàlid
 * 
 */
int triarMovimentJugador(QuatreEnRatlla *partida, char jugador);


/**
 * @brief Utilitza el min-max per a trobar la millor jugada
 * 
 * 
 * Fa servir la funció huristica normal.
 * Per cridar a la funció s'ha de poder fer algun moviment.
 * 
 * @param partida és un apuntador a la partida que s'està jugant
 * 
 * @param jugador és el jugador que fa el moviment
 * 
 * @return el número de la columna que l'algorisme ha decidit. Sempre retorna un moviment vàlid
 * 
 */
int triarMovimentBot(QuatreEnRatlla *partida, char jugador);


/**
 * @brief Utilitza el min-max per a trobar la millor jugada amb una probabilitat de fer un moviment aleatori
 * 
 * 
 * Fa servir la funció huristica normal.
 * Per cridar a la funció s'ha de poder fer algun moviment.
 * 
 * @param partida és un apuntador a la partida que s'està jugant
 * 
 * @param jugador és el jugador que fa el moviment
 * 
 * @return el número de la columna que l'algorisme ha decidit. Sempre retorna un moviment vàlid
 * 
 */
int triarMovimentBotAleatori(QuatreEnRatlla *partida, char jugador);


#endif // PARTIDES_H