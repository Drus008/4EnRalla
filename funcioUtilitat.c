#include "4enratlla.h"
#include "funcioUtilitat.h"


float utilitat(QuatreEnRatlla *partida, char jugador){
    return 0.0;
}


// Donada una partida, una fila i una columna, retorna per entrada el nombre de fitxes de cada jugador adjacents a la donada
void comptarFitxesAdjacents(QuatreEnRatlla *partida, int fila, int col, int* fitxesJ1, int* fitxesJ2){
    for(int i=fila-1; i<fila+2; i++){
        for(int j=col-1; j<col+2; j++){
            if (i>=0 && j>=0 && i<NFILES && j<NCOLS){
                if(partida->tauler[i][j]==1){
                    (*fitxesJ1)++;}
                else if (partida->tauler[i][j]==2){
                    (*fitxesJ2)++;}
            }
        }
    }
}

// Retorna una puntuació de l'estat de la partida en funció de les fitxes dayacents de cada jugador
int puntuacioPerAdjacencia(QuatreEnRatlla *partida){
    int puntuacio = 0;
    for(int f=0; f<NFILES; f++){
        for(int c=0; c<NCOLS; c++){
            int fitxesJ1 = 0;
            int fitxesJ2 = 0;
            comptarFitxesAdjacents(partida, f, c, &fitxesJ1, &fitxesJ2);
            puntuacio = puntuacio + fitxesJ1*fitxesJ1 - fitxesJ2*fitxesJ2;
        }
    }
    return puntuacio;
}












float aplicarKerAPosicio(int fila, int col, signed char partida[NCOLS][NFILES], float kernel[KER_DIM][KER_DIM]){
    float sum = 0;
    for(int df=-HALF_KER; df<=KER_DIM; df++){
        for(int dc=-HALF_KER; dc<=KER_DIM; dc++){
            sum = sum + partida[fila+df][col+dc]*kernel[df][dc];
        }
    }
}

void convolucio(signed char partida[NFILES][NCOLS], float kernel[KER_DIM][KER_DIM], float resultatConvolucio[NFILES-HALF_KER][NCOLS-HALF_KER]){
    for(int f=HALF_KER; f+HALF_KER<NFILES; f++){
        for(int c=HALF_KER; c+HALF_KER<NCOLS; c++){
            resultatConvolucio[f-HALF_KER][c-HALF_KER] = aplicarKerAPosicio(f,c,partida, kernel);
        }
    }
}