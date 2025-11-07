#include "4enratlla.h"
#include "funcioUtilitat.h"
#include "minmax.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>



int minMax(QuatreEnRatlla *partida, char jugadorOriginal){
    int dumy1 = 0;
    double dumy2;
    
    int millorMoviment = iteracioMinmax(partida, jugadorOriginal, dumy1,&dumy2,&dumy1);
    return millorMoviment;
}


int iteracioMinmax(QuatreEnRatlla *partida, char jugadorOriginal, int nivellNode, double *puntuacioNode, int *profunditatNode){
    char jugador = (nivellNode % 2 == 0) ? 1 : -1;
    int nCols = partida->ncols;
    nivellNode++;

    int millorTirada=0;
    double multiplicador = 1;
    if (nivellNode%2==0) multiplicador = -1;

    double millorValoracio = -INFINITY;
    int millorNTirades=__INT_MAX__;

    for(int i=0; i<nCols; i++){
        
        if (!comprovarColumnaPlena(partida, i)) {
            
            realitzarMoviment(partida, i, jugador);
            
            double valoracioMoviment;
            int nMoviments;
            
            if (!omplirNodeTrivial(partida, i, jugadorOriginal, &valoracioMoviment, &nMoviments, nivellNode)){
                iteracioMinmax(partida, jugadorOriginal, nivellNode, &valoracioMoviment, &nMoviments);
            }
            //imprimirQuateEnRatlla(partida);
            //printf("(%i) punt:%lf, prof: %i. millor: %lf\n", nivellNode,-multiplicador* valoracioMoviment, nMoviments, millorValoracio);

            desferMoviment(partida, i); 
            triaMillorTirada(nivellNode,&millorTirada,i,&millorValoracio, -multiplicador* valoracioMoviment,&millorNTirades, nMoviments);
        }
    }
    
    *puntuacioNode = -multiplicador* millorValoracio;
    *profunditatNode = millorNTirades+1;
    return millorTirada;
}



bool omplirNodeTrivial(QuatreEnRatlla *partida, int moviment, char jugadorOriginal, double *valoracio, int *nMoviments, int profunditat){
    double multiplicador = -1;
    if (profunditat%2==0) multiplicador = 1;
    //Aquesta part es necessaria xq puntuacioPerAdj no te en compte a quin jugador li toca
    //pero si es guanya la partida sempre dona inf, encara que no toqui.
    if (comprovarSolucio(partida,moviment)) *valoracio = multiplicador* INFINITY; 
    else if(profunditat==PROFUNDITAT) *valoracio = -(double)puntuacioPerAdjacencia(partida);
    else return false;
    *nMoviments = 0;
    return true;
}


void triaMillorTirada(int nivellNode, int *millorMoviment, int movimentActual, double *millorValoracio, double valoracioActual, int *accioDesempat, int accioDesempatActual){
    if (valoracioActual>=*millorValoracio){
        if(valoracioActual!=*millorValoracio || accioDesempatActual<*accioDesempat){
        *millorMoviment = movimentActual;
        *millorValoracio = valoracioActual;
        *accioDesempat = accioDesempatActual;}

    }
}

int triaNaive(QuatreEnRatlla *partida){
    int puntuacioMaxima = -__INT_MAX__+1;
    int millorMoviment=-1;
    for(int i=0; i<partida->ncols; i++){
        if(!comprovarColumnaPlena(partida,i)){
            realitzarMoviment(partida,i,2);
            int puntuacioMoviment =-puntuacioPerAdjacencia(partida);
            if(puntuacioMoviment>puntuacioMaxima){
                puntuacioMaxima = puntuacioMoviment;
                millorMoviment = i;
            }
            desferMoviment(partida,i);
        }
    }
    return millorMoviment;
}