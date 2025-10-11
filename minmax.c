#include "4enratlla.h"
#include "funcioUtilitat.h"

#include<stdio.h>

/*typedef struct arbre{
    int valoracio;
    arbre *fills;
}Arbre;*/



int triaNaive(QuatreEnRatlla *partida){
    int puntuacioMaxima = -__INT_MAX__+1;
    int millorMoviment=-1;
    for(int i=0; i<NCOLS; i++){
        if(comprovarMovimentLegal(partida,i)){
            realitzarMoviment(partida,i,2);
            int puntuacioMoviment =-puntuacioPerAdjacencia(partida);
            printf("Puntuació assignada a %i: %i\n", i, puntuacioMoviment);
            if(puntuacioMoviment>puntuacioMaxima){
                puntuacioMaxima = puntuacioMoviment;
                millorMoviment = i;
            }
            desferMoviment(partida,i);
        }
    }
    return millorMoviment;
}