#ifndef PARTIDES_H
#define PARTIDES_H


/**
 * @brief Inicia la partida d'un jugador contra un bot.
 * @param tornJugador indica si el jugador comença al torn 1 o al torn 2.
*/
void pardidaPlayerVsBot(char tornJugador);

/**
 * @brief Inicia la partida d'un bot contra un bot
 * @param espera indica el temps que hi ha entre que un bot tira i l'altre comença a pensar.
*/
void pardidaBotVsBot(double espera);


/**
 * @brief Inicia la partida d'un jugador contra un jugador
*/
void pardidaDeDosJugadors();



#endif // PARTIDES_H