#include "4enratlla.h"
#include "funcioUtilitat.h"
#include "minmax.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>


void imprimirNode(nodeArbre *arbre){
    for(int i=0; i<arbre->nivell;i++){
        printf("  ");
    }
    printf("%i:%lf\n",arbre->nivell, arbre->puntuacio);
}

void alliberarFills(nodeArbre *arbre, int nfills){
    for(int i=0;i<nfills; i++){
        if(!arbre->fills[i]->fulla) free(arbre->fills[i]);
    }
}


void imprimirArbre(nodeArbre* arbre, int profunditatMaxima, int nfills){
    imprimirNode(arbre);
    if(arbre->nivell<profunditatMaxima && !arbre->fulla){
        
        for(int i=0;i<nfills; i++){
            imprimirArbre(arbre->fills[i],profunditatMaxima, nfills);
        }
    }
}


int minMax(QuatreEnRatlla *partida, char jugadorOriginal){
    nodeArbre *arbreProba = malloc(sizeof(nodeArbre));
    arbreProba->nivell = 0;
    int millorMoviment = iteracioMinmax(arbreProba, partida, jugadorOriginal);
    //imprimirArbre(arbreProba,3);
    free(arbreProba);
    return millorMoviment;
}


int iteracioMinmax(nodeArbre *arbre, QuatreEnRatlla *partida, char jugadorOriginal){
    char jugador = (char)((arbre->nivell+1)%2+1);
    int nCols = partida->ncols;
    arbre->fills = malloc(sizeof(nodeArbre*)*nCols);
    int nouNivell = arbre->nivell+1;
    for(int i=0; i<nCols; i++){
        arbre->fills[i] = malloc(sizeof(nodeArbre));
        arbre->fills[i]->nivell = nouNivell;
        arbre->fills[i]->tirada = i;
        
        
        if (!omplirNodeTrivial(arbre->fills[i], partida, jugador)){
            realitzarMoviment(partida, i, jugador);
            iteracioMinmax(arbre->fills[i], partida, jugadorOriginal);
            desferMoviment(partida, i);
        }
    }
    
    int millorTirada;
    if((arbre->nivell)%2==1) millorTirada = trobarMinim(arbre, nCols);
    else millorTirada = trobarMaxim(arbre, nCols);
    arbre->puntuacio = arbre->fills[millorTirada]->puntuacio;
    arbre->profunditatSolucio = arbre->fills[millorTirada]->profunditatSolucio;

    alliberarFills(arbre, nCols);

    return millorTirada;
}


bool omplirNodeTrivial(nodeArbre* arbre, QuatreEnRatlla *partida, char jugador){
    int moviment = arbre->tirada;
    double multiplicador=-1;
    if (jugador==2) multiplicador=1; //Això hem dona que no haria de ser aixi, ns hauria de dependre del jugador, però clarament deixa de funcionar si ho canvio

    if (comprovarColumnaPlena(partida, moviment)) {
        arbre->puntuacio = -multiplicador*INFINITY;
        arbre->profunditatSolucio = arbre->nivell;
    }
    else{
        realitzarMoviment(partida, moviment, jugador);
        if (comprovarSolucio(partida,moviment)) {
            arbre->puntuacio = multiplicador*INFINITY;
            arbre->profunditatSolucio = arbre->nivell;
        } 
        else if((arbre->nivell)==PROFUNDITAT){
            //imprimirQuateEnRatlla(partida);
            arbre->puntuacio = multiplicador* (double)puntuacioPerAdjacencia(partida);
        }
        else{desferMoviment(partida, moviment); arbre->fulla=false; return false;}
        desferMoviment(partida, moviment);
    }
    arbre->fulla=true;
    return true;
}//Segurament reorganitzant condicionals i fent que no es faci el moviment si no cal es pot millorar l'eficiència


int trobarMaxim(nodeArbre *arbre, int midaLlista){//Segurament això es pot optimitzar per a que faci menys comparacions si una fila és INF
    double max = arbre->fills[0]->puntuacio;
    int indexMax = 0;
    int desempat = arbre->fills[0]->profunditatSolucio;
    double llista[midaLlista];
    for(int i=0; i<midaLlista; i++) llista[i] = arbre->fills[i]->puntuacio;

    for(int i=1;i<midaLlista;i++){
        if (max<llista[i]){
            max = llista[i];
            indexMax = i;
        }
        //Aquesta part del codi serveix per a desempatar en cas de que dos estats tinguin la mateixa valoració
        //El que es fa és triar la que menys passos hagi necessitat. Això estalvia un parell de tirades i per tant bastants cálculs
        else if(max==llista[i] && arbre->fills[i]->profunditatSolucio<desempat){
            max = llista[i];
            indexMax = i;
        }
    }
    return indexMax;
}

int trobarMinim(nodeArbre *arbre, int midaLlista){
    double min = arbre->fills[0]->puntuacio;
    int indexMin = 0;
    int desempat = arbre->fills[0]->profunditatSolucio;
    double llista[midaLlista];
    for(int i=0; i<midaLlista; i++) llista[i] = arbre->fills[i]->puntuacio;

    for(int i=1;i<midaLlista;i++){
        if (min>llista[i]){
            min = llista[i];
            indexMin = i;
        }
        else if(min==llista[i] && arbre->fills[i]->profunditatSolucio>desempat){
            min = llista[i];
            indexMin = i;
        }
    }
    return indexMin;
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