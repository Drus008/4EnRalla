#ifndef QUATRE_EN_RATLLA_H
#define QUATRE_EN_RATLLA_H


#include <stdbool.h>


//Crec que és millor incloure un array que indique a quina fila s'ha colocat la ultima peça
//ja que aixi no s'ha de calcular cada cop

// També crec que és millor introduir un comptador de fitxes colocades i així podem decidir si s'ha empatat la partida amb una sola comprovació

// També crec que sería millor reservar més memòria per a no fer comprovacions de si estem mirant a dins de la matriu

/**
 * @brief Estructura que conté l'estat d'una partida
 * 
 */
typedef struct quatreEnRatlla {
    signed char **tauler; /**<Taulell actual amb les fitxes colocades> */
    int nfiles; /**<nombre de files del taulell> */
    int ncols; /**<nombre de columnes del taulell> */
    int nVictoria; /**<Nombre de peces iguals que s'han d'alinear per a guanyar la partida> */
    int *filesSuperiors; /*<Indica a cada columna a quina fila està la peça més alta (Sense usar actualment)*/
}QuatreEnRatlla;



/**
 * @brief Imprimeix un taulell del 4 en ratlla
 * 
 * @param partida és un apuntador la partida de la que es vol imprimir l'estat
 */
void imprimirQuateEnRatlla(QuatreEnRatlla *partida);

/**
 * @brief Inicia una partida del 4 en ratlla des de zero.
 * 
 * @param partida és un apuntador a la partida que es vol iniciar.
 * @param nFils és el nombre de files de la taula.
 * @param nCols és el nombre de columnes de la taula.
 * @param nVictoria és el nombre de peces adjacents alineades que s'han de tenir per a guanyar la partida
 */
void inicialitzarQuatreEnRatlla(QuatreEnRatlla *partida, int nFils, int nCols, int nVictoria);

/**
 * @brief Allibera la partida del 4 en ratlla quan ja no es necessita.
 * 
 * @param partida és un apuntador a la partida que es vol alliberar.
 */
void alliberarQuatreEnRatlla(QuatreEnRatlla *partida);

/**
 * @brief Comprova si hi ha espai en una columna per a introduïr fitxes.
 * 
 * Només té en compte si les columnes están plenes, no considera si el nombre està fora del taulell.
 * 
 * @param partida és una apuntador a la partida que es vol comprovar si es pot fer el moviment.
 * @param moviment és el número de columna que es vol comprovar si es pot introduïr una fitxa.
 * 
 * 
 * @return true si la columna es plena i false si hi ha espai.
 */
bool comprovarColumnaPlena(QuatreEnRatlla *partida, int moviment);


/**
 * @brief Introdueix una fitxa d'un jugador en una columna.
 * 
 * No té en compte si el moviment no es pot fer.
 * 
 * @param partida és un apuntador a la partida en la que es vol fer el moviment.
 * @param moviment és el número de la columna en la que es vol introduïr la fitxa.
 * @param jugador és el jugador que realitza el moviment.
 */
void realitzarMoviment(QuatreEnRatlla *partida, int moviment, char jugador);

/**
 * @brief Desfar un moviment d'una partida
 * 
 * El que fa és eliminar
 * 
 * No té en compte si el movment s'ha fet anteriorment
 * 
 * @param partida és un apuntador a la partida en la que es vol desfer un moviment
 * @param moviment és el número de columna del que es vol desfer l'últim moviment
 */
void desferMoviment(QuatreEnRatlla *partida, int moviment);

// Aquesta crec que s'hauria d'eliminar
int filaSuperior(QuatreEnRatlla *partida, int columna);


// Aquest codi és horrible, estaria bé una funció que ho englobes tot i fos eficient
bool comprovarSolucioHoritzontal(QuatreEnRatlla *partida, int fila, int col);
bool comprovarSolucioVertical(QuatreEnRatlla *partida, int fila, int col);
bool comprovarSolucioDiagonal1(QuatreEnRatlla *partida, int fila, int col);
bool comprovarSolucioDiagonal2(QuatreEnRatlla *partida, int fila, int col);

/**
 * @brief Comprova si un moviment fet ha guanyat la partida.
 * 
 * @param partida és un apuntador a la partida que es vol comprovar.
 * @param int és l'ultim moviment fet on es comprovarà si forma part d'una solució.
 * 
 * @return true si s'ha guanyat la partida i false si la partida continua.
 */
bool comprovarSolucio(QuatreEnRatlla *partida, int ultimMoviment);

/**
 * @brief Comprova si s'ha empatat la partida.
 * 
 * @param partida un apuntador a la partida que es comprova.
 * 
 * @return true si la partida s'ha empatat i false en cas contrari.
 */
bool comprovarEmpat(QuatreEnRatlla *partida);




#endif