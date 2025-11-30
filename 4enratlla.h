#ifndef QUATRE_EN_RATLLA_H
#define QUATRE_EN_RATLLA_H


#include <stdbool.h>


/**
 * @file 4enratlla.h
 */



/**
 * @brief Estructura que conté l'estat d'una partida
 * 
 */
typedef struct quatreEnRatlla {
    signed char **tauler; /**<Taulell actual amb les fitxes colocades. Ha de ser una matriu de mida nfiles x ncols. L'índex 0,0 és la posició superior a l'esquerra */
    int nfiles; /**<nombre de files del taulell */
    int ncols; /**<nombre de columnes del taulell */
    int nVictoria; /**<Nombre de peces iguals que s'han d'alinear per a guanyar la partida */
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
 * @brief Fica tots els valors del tauler a 0.
 * 
 * @param partida és la partida que vols reiniciar.
 */
void reiniciarQuatreEnRatlla(QuatreEnRatlla *partida);


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
void realitzarMoviment(QuatreEnRatlla *partida, int moviment, signed char jugador);

/**
 * @brief Desfar un moviment d'una partida
 * 
 * El que fa és eliminar l'ultim moviment fer.
 * 
 * No té en compte si el movment s'ha fet anteriorment. (no verifica que l'acció sigui válida)
 * 
 * @param partida és un apuntador a la partida en la que es vol desfer un moviment
 * @param moviment és el número de columna del que es vol desfer l'últim moviment
 */
void desferMoviment(QuatreEnRatlla *partida, int moviment);



/**
 * @brief Donada una partida y una columna retorna la fila on és hi ha la peça més alta.
 * 
 * @param partida és la partida de la qual es vol comprovar la fila superior.
 * @param columna és la columna de la qual es vol comprovar la fila superior.
 * 
 * @return l'índex de la fila amb la peça més alta de la columna indicada. Si la fila es buida retorna -1.
 * 
 */
int filaSuperior(QuatreEnRatlla *partida, int columna);


/**
 * @brief Donat on s'ha inserit l'última peça retorna si aquest moviment provoca un k en ratlla horitzontal.
 * 
 * @todo Segurament aquesta funció es pot fusionar d'alguna manera amb les altes comprovar solució, ja que son molt semblans.
 * 
 * @param partida és la partida que es vol comprovar.
 * @param fila és la fila on s'ha fet el moviment.
 * @param col és la columna on s'ha fet el movient
 * 
 * \return true si s'ha guanyat la partida i false en cas de no ser la solució horitzontal.
 */
bool comprovarSolucioHoritzontal(QuatreEnRatlla *partida, int fila, int col);

/**
 * @brief Donat on s'ha inserit l'última peça retorna si aquest moviment provoca un k en ratlla verical.
 * 
 * @todo Segurament aquesta funció es pot fusionar d'alguna manera amb les altes comprovar solució, ja que son molt semblans.
 * 
 * @param partida és la partida que es vol comprovar.
 * @param fila és la fila on s'ha fet el moviment.
 * @param col és la columna on s'ha fet el movient.
 * 
 * \return true si s'ha guanyat la partida i false en cas de no ser la solució vertical.
 */
bool comprovarSolucioVertical(QuatreEnRatlla *partida, int fila, int col);

/**
 * @brief Donat on s'ha inserit l'última peça retorna si aquest moviment provoca un k en ratlla diagonal d'esquerra a dretas.
 * 
 * @todo Segurament aquesta funció es pot fusionar d'alguna manera amb les altes comprovar solució, ja que son molt semblans.
 * 
 * @param partida és la partida que es vol comprovar.
 * @param fila és la fila on s'ha fet el moviment.
 * @param col és la columna on s'ha fet el movient.
 * 
 * \return true si s'ha guanyat la partida i false en cas de no ser la solució diagonal.
 */
bool comprovarSolucioDiagonal1(QuatreEnRatlla *partida, int fila, int col);

/**
 * @brief Donat on s'ha inserit l'última peça retorna si aquest moviment provoca un k en ratlla diagonal de dreta a esquerra.
 * 
 * @todo Segurament aquesta funció es pot fusionar d'alguna manera amb les altes comprovar solució, ja que son molt semblans.
 * 
 * @param partida és la partida que es vol comprovar.
 * @param fila és la fila on s'ha fet el moviment.
 * @param col és la columna on s'ha fet el movient.
 * 
 * \return true si s'ha guanyat la partida i false en cas de no ser la solució diagonal.
 */
bool comprovarSolucioDiagonal2(QuatreEnRatlla *partida, int fila, int col);

/**
 * @brief Comprova si un moviment fet ha guanyat la partida.
 * 
 * No comprova que l'ultim moviment donat realment ho sigui. Es important no ficar un moviment que no 'shagi fet.
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