#ifndef PARTIDES_H
#define PARTIDES_H

#include"4enratlla.h"


/**
 * @file partides.h
 */


/**
 * @brief Tipus de dada que representa una funció que donat un QuatreEnRatlla y un jugador, decideix quin moviment es realitzarà.
 * 
 */
typedef int (*selectorDeMoviment)(QuatreEnRatlla*, signed char, void*);




/**
 * @brief Tipus de dada que representa una funció que donada una partida assigna un double. Aquest representa lo ben posicionat que està el jugador que ha començat la partida per a guanyar-la.
 * 
 * Reb com a paràmetres l'estat de la partida actual y el context. Aquest inclou altres paràmetres que reb la funció.
 */
typedef double (*funcioHeuristica)(QuatreEnRatlla *partida, void *ctx);

/**
 * @brief Estructura que conté les funcions heurístiques de la partida i altres paràmetres necessaris per a l'execució d'aquesta
 * @todo potser es podría implementar la profunditat variable per aquí.
 * */
typedef struct contextHeuristica{
    funcioHeuristica funcio[2]; /**< Un apuntador a las funcions heurístiques del J1 y del J2 */
    void *altres[2]; /**< Un apuntador a altres paràmetres necessaris per al funcionament de les funcions */
} ContextHeuristica;



/**
 * @brief Pregunta al jugador quin moviment vol fer.
 * 
 * No para de preguntar fins a rebre una jugada válidad. En cas de no rebre una jugada vàlida imprimeix per
 * consola si es per un error ortogràfic o per una columna de fora del taulell.
 * 
 * @param partida és un apuntador a la partida que s'està jugant
 * @param jugador és el jugador que fa el moviment.
 * @param ctx és un apuntador que no ha de contenir res, només li dona el tipus de selectorDeMoviment.
 * 
 * @return el número de la columna que el jugador ha dit que vol tirar. Sempre retorna un moviment vàlid
 * 
 */
int triarMovimentJugador(QuatreEnRatlla *partida, signed char jugador, void *ctx);


/**
 * @brief Utilitza el min-max per a trobar la millor jugada
 * 
 * La funció per decidir el moviment s'ha de pasar com a context.
 * 
 * @param partida és un apuntador a la partida que s'està jugant.
 * @param jugador és el jugador que fa el moviment.
 * @param ctx serveix per a passar paràmetres extra (con funcions heurístiques) però preservant el tipus selectorDeMoviment. Aquest ha de ser de la classe context heurística.
 * 
 * @return el número de la columna que l'algorisme ha decidit. Sempre retorna un moviment vàlid
 * 
 */
int triarMovimentBot(QuatreEnRatlla *partida, signed char jugador, void *ctx);


/**
 * @brief Utilitza el min-max per a trobar la millor jugada amb una probablitat de fer un moviment aleatori.
 * 
 * La funció per decidir el moviment s'ha de pasar com a context.
 * 
 * @param partida és un apuntador a la partida que s'està jugant.
 * @param jugador és el jugador que fa el moviment.
 * @param ctx serveix per a passar paràmetres extra (con funcions heurístiques) però preservant el tipus selectorDeMoviment. Aquest ha de ser de la classe context heurística.
 * 
 * @return el número de la columna que l'algorisme ha decidit. Sempre retorna un moviment vàlid
 * 
 */
int triarMovimentBotAleatori(QuatreEnRatlla *partida, signed char jugador, void*ctx);


#endif // PARTIDES_H