#include "4enratlla.h"
#include "funcioUtilitat.h"



/**
 * @file funcioUtilitat.c
 * @brief Fitxer que implementa una funció heurísitca que valora l'adjacència.
 */

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


double puntuacioPerAdjacencia(QuatreEnRatlla *partida, void *ctx){
    int puntuacio = 0;
    for(int f=0; f<partida->nfiles; f++){
        for(int c=0; c<partida->ncols; c++){
            int fitxesJ1 = 0;
            int fitxesJ2 = 0;
            comptarFitxesAdjacents(partida, f, c, &fitxesJ1, &fitxesJ2);
            puntuacio = puntuacio + fitxesJ1*fitxesJ1 - fitxesJ2*fitxesJ2;
        }
    }
    return (double)puntuacio;
}



