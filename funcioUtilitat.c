#include "4enratlla.h"
#include "funcioUtilitat.h"

float utilitat(QuatreEnRatlla *partida, char jugador){
    return 0.0;
}


float aplicarKerAPosicio(int fila, int col, signed char partida[NCOLS][NFILES], float kernel[KER_DIM][KER_DIM]){
    float sum = 0;
    for(int df=-HALF_KER; df<=KER_DIM; df++){
        for(int dc=-HALF_KER; dc<=KER_DIM; dc++){
            sum = sum + partida[fila+df][col+dc]*kernel[df][dc];
        }
    }
}

void convolucio(char partida[NFILES][NCOLS], float kernel[KER_DIM][KER_DIM], float resultatConvolucio[NFILES-HALF_KER][NCOLS-HALF_KER]){
    for(int f=HALF_KER; f+HALF_KER<NFILES; f++){
        for(int c=HALF_KER; c+HALF_KER<NCOLS; c++){
            resultatConvolucio[f-HALF_KER][c-HALF_KER] = aplicarKerAPosicio(f,c,partida, kernel);
        }
    }
}