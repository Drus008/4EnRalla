#include "4enratlla.h"
#include "minmax.h"
#include "partides.h"
#include "funcioUtilitat.h"

#include "Xarxa.h"
#include "conexioXarxa4EnRatlla.h"


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>   

#define PROBABILITAT_ALEATORI 0.01

/**
 * @file partides.c
 * @brief Fitxer que inclou totes les funcions relacionades amb la interfície de la partida i la tria de moviments.
 */


int triarMovimentJugador(QuatreEnRatlla *partida, signed char jugador, void *ctx){
    int moviment = -1;
    while (moviment==-1){
        printf("Intodueix una columna: ");

        if (scanf(" %d", &moviment)!=1){
            while (getchar() != '\n'); //atois
            printf(" Input invàlid.\n");
            moviment = -1;
        }
        else if(moviment<0 || moviment>=partida->ncols){
            printf(" Moviment fora del taulell.\n");
            moviment=-1;}
        else if(comprovarColumnaPlena(partida,moviment)){
            printf(" Columna plena.\n");
            moviment=-1;}
        else realitzarMoviment(partida, moviment, jugador);
    }
    return moviment;
}

int triarMovimentBot(QuatreEnRatlla *partida, signed char jugador, void *ctx){
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

int triarMovimentBotAleatori(QuatreEnRatlla *partida, signed char jugador, void *ctx){
    if(ctx==NULL) fprintf(stderr, "(triarMovimentBotSenseText): No s'ha especificat cap funció heurística.");
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



void iniciarPartida(selectorDeMoviment decisioJ1, selectorDeMoviment decisioJ2, double esperaEntreTorns, void *ctx){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova, 8, 8, 4);
    bool partidaEnCurs = true;
    signed char jugadors[2] = {1, -1};
    
    while (partidaEnCurs){
        for(int i=0; i<2; i++){
            sleep(esperaEntreTorns);
            imprimirQuateEnRatlla(&prova);
            if(i==0) printf("Torn del jugador 1 🟥. A quina columna vols ficar la peça?\n");
            else printf("Torn del jugador 2 🟨. A quina columna vols ficar la peça?\n"); 

            int moviment;
            if(jugadors[i]==1) moviment = decisioJ1(&prova, jugadors[i], ctx);
            else moviment = decisioJ2(&prova, jugadors[i], ctx);

            if(comprovarSolucio(&prova, moviment)){
                if (i==0) printf("\nHA GUANYAT EL JUGADOR 1 🟥.\n");
                else printf("\nHA GUANYAT EL JUGADOR 2 🟨.\n");
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
    LlistaNKer[0] = 3; LlistaNKer[1] = 3;
    XarxaNeuronal *xarxa = crearXarxaAleatoria(2,nDimKer, LlistaNKer,8,8);
    XarxaNeuronal *xarxa2 = carregarXarxa("XarxaEntrenadaJ2FINAL.DrusCNN"); //crearXarxaAleatoria(2,nDimKer, LlistaNKer,8,8);

    imprimirXarxa(xarxa2);
    if(xarxa==NULL){
        printf("ERROR al carregar la xarxa\n");
        return;
    }

    heuristiques.funcio[0] = wrapperXarxa;
    heuristiques.funcio[1] = puntuacioPerAdjacencia;
    heuristiques.altres[0] = xarxa2;
    heuristiques.altres[1] = xarxa2;

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

int main(){
    validacioXarxa();
}