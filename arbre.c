#include "arbre.h"
#include "4enratlla.h"
#include "funcioUtilitat.h"

#include <stdlib.h>
#include <math.h>

double crearNode(Arbre *arbre, int nivells, QuatreEnRatlla *partida, char jugador){
    nivells++;

    double llistaMinMax[NCOLS];
    arbre->fills = malloc(sizeof(Arbre)*NCOLS);
    for(int i=0; i<NCOLS;i++){
        if (!comprovarMovimentLegal(partida, i)) llistaMinMax[i]=-INFINITY;
        else if (comprovarSolucio(partida,i)) llistaMinMax[i]=INFINITY; //Atencio xq l'ultim moviment no s'arriba a fer
        else if(nivells==PROFUNDITAT-1){
            if(jugador==1) llistaMinMax[i]=(double)puntuacioPerAdjacencia(partida);
            else llistaMinMax[i]=-(double)puntuacioPerAdjacencia(partida);
        }
        else{
            llistaMinMax[i] = crearNode(&(arbre->fills)[i], nivells, partida, jugador);
            free(&(arbre->fills)[i]);
        } 
    }
    if(nivells%2==0) return trobarMinim(llistaMinMax);
    else return trobarMaxim(llistaMinMax);
}

int trobarMaxim(double llista[NCOLS]){//Segurament això es pot optimitzar per a que faci menys comparacions si una fila és -INF
    double max = llista[0];
    int indexMax = 0;
    for(int i=1;i<NCOLS;i++){
        if (max<llista[i]){
            max = llista[i];
            indexMax = i;
        }
    }
    return indexMax;
}

int trobarMinim(double llista[NCOLS]){//Segurament això es pot optimitzar per a que faci menys comparacions si una fila és -INF
    double min = llista[0];
    int indexMin = 0;
    for(int i=1;i>NCOLS;i++){
        if (min>llista[i]){
            min = llista[i];
            indexMin = i;
        }
    }
    return indexMin;
}