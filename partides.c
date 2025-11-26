#include "4enratlla.h"
#include "minmax.h"
#include "partides.h"
#include "funcioUtilitat.h"

#include"Xarxa.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>   


#define PROBABILITAT_ALEATORI 0.00

int triarMovimentJugador(QuatreEnRatlla *partida, char jugador, void *ctx){
    int moviment = -1;
    while (moviment==-1){
        printf("Intodueix una columna: ");

        if (scanf(" %d", &moviment)!=1){
            while (getchar() != '\n'); //atois
            printf("Moviment invàlid.\n");
            moviment = -1;
        }
        
        else if(moviment>=0 && moviment<partida->ncols && !comprovarColumnaPlena(partida,moviment)) realitzarMoviment(partida, moviment, jugador);
        else{
            printf("Moviment il·legal.\n");
            moviment = -1;
        }
    }
    return moviment;
}

int triarMovimentBot(QuatreEnRatlla *partida, char jugador, void *ctx){
    ContextHeuristica *context = (ContextHeuristica*) ctx;

    int index;
    if (jugador==1) index = 0;
    else index = 1;

    funcioHeuristica f = context->funcio[index];
    
    int moviment = minMax(partida, jugador, f,context->altres[index]);
    printf("Movent a %d\n", moviment);
    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}

int triarMovimentBotAleatori(QuatreEnRatlla *partida, char jugador, void *ctx){
    ContextHeuristica *context = (ContextHeuristica*) ctx;

    int index;
    if (jugador==1) index = 0;
    else index = 1;

    funcioHeuristica f = context->funcio[index];
    double probabilitat = (double) rand() / RAND_MAX;
    int moviment;
    if (probabilitat<PROBABILITAT_ALEATORI){
        printf("Triant aleatoriament\n");
        bool movimentValid = false;
        while (!movimentValid) {
            moviment = rand()%(partida->ncols);
            movimentValid = !comprovarColumnaPlena(partida, moviment);
        }
        printf("Fent moviment aleatori. ");
    } else moviment = minMax(partida, jugador, f, context->altres[index]);

    printf("Movent a %d\n", moviment);
    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}


int triarMovimentBotAleatoriSenseText(QuatreEnRatlla *partida, char jugador, void *ctx){
    ContextHeuristica *context = (ContextHeuristica*) ctx;

    int index;
    if (jugador==1) index = 0;
    else index = 1;

    funcioHeuristica f = context->funcio[index];
    double probabilitat = (double) rand() / RAND_MAX;
    int moviment;
    if (probabilitat<PROBABILITAT_ALEATORI){
        bool movimentValid = false;
        while (!movimentValid) {
            moviment = rand()%(partida->ncols);
            movimentValid = !comprovarColumnaPlena(partida, moviment);
        }
    } else moviment = minMax(partida, jugador, f, context->altres[index]);

    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}


void iniciarPartida(selectorDeMoviment decisioJ1, selectorDeMoviment decisioJ2, double esperaEntreTorns, void *ctx){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova, 8, 8, 4);
    bool partidaEnCurs = true;
    char jugadors[2] = {1, -1};
    
    while (partidaEnCurs){
        for(int i=0; i<2; i++){
            sleep(esperaEntreTorns);
            imprimirQuateEnRatlla(&prova);
            if(i==0) printf("Torn del jugador 1 🟥. A quina columna vols ficar la peça?\n");
            else printf("Torn del jugador 2 🟦. A quina columna vols ficar la peça?\n"); 

            int moviment;
            if(jugadors[i]==1) moviment = decisioJ1(&prova, jugadors[i], ctx);
            else moviment = decisioJ2(&prova, jugadors[i], ctx);

            if(comprovarSolucio(&prova, moviment)){
                printf("\nHA GUANYAT EL JUGADOR %d\n", 
                       jugadors[i] == 1 ? 1 : 2); 
                imprimirQuateEnRatlla(&prova);
                partidaEnCurs = false;
                break;
            }
            if(comprovarEmpat(&prova)){
                printf("PARTIDA EMPATADA\n");
                imprimirQuateEnRatlla(&prova);
                partidaEnCurs = false;
                break;
            }
        }
    }
    alliberarQuatreEnRatlla(&prova);
}













void validacioXarxa(){
    
    srand(time(NULL));
    ContextHeuristica heuristiques;
    int *nDimKer = malloc(sizeof(int)*2);
    nDimKer[0] = 3; nDimKer[1] = 3;
    
    int *LlistaNKer = malloc(sizeof(int)*2);
    nDimKer[0] = 3; nDimKer[1] = 3;
    XarxaNeuronal *xarxa2 = crearXarxaAleatoria(2,nDimKer, LlistaNKer,8,8);
    XarxaNeuronal *xarxa = carregarXarxa("XarxaCalculada.DrusCNN"); //crearXarxaAleatoria(2,nDimKer, LlistaNKer,8,8);

    imprimirXarxa(xarxa);
    if(xarxa==NULL){
        printf("ERROR al carregar la xarxa\n");
        return;
    }

    heuristiques.funcio[0] = wrapperXarxa;
    heuristiques.funcio[1] = puntuacioPerAdjacencia;
    heuristiques.altres[0] = xarxa;
    heuristiques.altres[1]=xarxa2;

    iniciarPartida(triarMovimentBot, triarMovimentBot, 0, &heuristiques);
}


void validacioNormal(){
    ContextHeuristica ctx;
    ctx.funcio[0] = puntuacioPerAdjacencia;
    ctx.funcio[1] = puntuacioPerAdjacencia;
    ctx.altres[0] = NULL;
    ctx.altres[1] = NULL;
    iniciarPartida(triarMovimentBot, triarMovimentJugador, 0, &ctx);
}

//gcc -g -fopenmp -ffast-math -funroll-loops -flto -march=native Entrenament.c 4enratlla.c  minmax.c Xarxa.c Utilitats.c partides.c funcioUtilitat.c -O3 -o partida -lm

int main(){
    validacioXarxa();
}