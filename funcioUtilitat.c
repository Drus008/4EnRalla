#include "4enratlla.h"
#include "funcioUtilitat.h"


float utilitat(QuatreEnRatlla *partida, char jugador){
    return 0.0;
}


void comptarFitxesAdjacents(QuatreEnRatlla *partida, int fila, int col, int* fitxesJ1, int* fitxesJ2){
    for(int i=fila-1; i<fila+2; i++){
        for(int j=col-1; j<col+2; j++){
            if (i>=0 && j>=0 && i<partida->nfiles && j<partida->ncols){
                if(partida->tauler[i][j]==1){
                    (*fitxesJ1)++;}
                else if (partida->tauler[i][j]==-1){
                    (*fitxesJ2)++;}
            }
        }
    }
}


int puntuacioPerAdjacencia(QuatreEnRatlla *partida){
    int puntuacio = 0;
    for(int f=0; f<partida->nfiles; f++){
        for(int c=0; c<partida->ncols; c++){
            int fitxesJ1 = 0;
            int fitxesJ2 = 0;
            comptarFitxesAdjacents(partida, f, c, &fitxesJ1, &fitxesJ2);
            puntuacio = puntuacio + fitxesJ1*fitxesJ1 - fitxesJ2*fitxesJ2;
        }
    }
    return puntuacio;
}












float aplicarKerAPosicio(int fila, int col, signed char **partida, float kernel[KER_DIM][KER_DIM]){
    float sum = 0;
    for(int df=-HALF_KER; df<=KER_DIM; df++){
        for(int dc=-HALF_KER; dc<=KER_DIM; dc++){
            sum = sum + partida[fila+df][col+dc]*kernel[df][dc];
        }
    }
}

void convolucio(signed char **partida, float kernel[KER_DIM][KER_DIM], float **resultatConvolucio, int nFiles, int nCols){
    for(int f=HALF_KER; f+HALF_KER<nFiles; f++){
        for(int c=HALF_KER; c+HALF_KER<nCols; c++){
            resultatConvolucio[f-HALF_KER][c-HALF_KER] = aplicarKerAPosicio(f,c,partida, kernel);
        }
    }
}