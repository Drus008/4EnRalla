#ifndef QUATRE_EN_RATLLA_H
#define QUATRE_EN_RATLLA_H


#include <stdbool.h>

#define NFILES 8
#define NCOLS 8
#define MODIFICADOR 4


typedef struct quatreEnRatlla {
    signed char tauler[NFILES][NCOLS];
}QuatreEnRatlla;



void imprimirQuateEnRatlla(QuatreEnRatlla *partida);

void inicialitzarQuatreEnRatlla(QuatreEnRatlla *partida);

bool comprovarMovimentLegal(QuatreEnRatlla *partida, int moviment);

void realitzarMoviment(QuatreEnRatlla *partida, int moviment, char jugador);

int filaSuperior(QuatreEnRatlla *partida, int columna);

bool comprovarSolucioHoritzontal(QuatreEnRatlla *partida, int fila, int col);
bool comprovarSolucioVertical(QuatreEnRatlla *partida, int fila, int col);
bool comprovarSolucioDiagonal1(QuatreEnRatlla *partida, int fila, int col);
bool comprovarSolucioDiagonal2(QuatreEnRatlla *partida, int fila, int col);

bool comprovarSolucio(QuatreEnRatlla *partida, int ultimMoviment);
bool comprovarEmpat(QuatreEnRatlla *partida);

void pardidaDeDosJugadors();


#endif // QUATRE_EN_RATLLA_H
