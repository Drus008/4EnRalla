#include "4enratlla.h"
#include "minmax.h"
#include "partides.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>   


#define PROBABILITAT_ALEATORI 0.05

int triarMovimentJugador(QuatreEnRatlla *partida, char jugador){
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

int triarMovimentBot(QuatreEnRatlla *partida, char jugador){
    
    int moviment = minMax(partida, jugador);
    printf("Movent a %d\n", moviment);
    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}

int triarMovimentBotAleatori(QuatreEnRatlla *partida, char jugador){
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
    } else moviment = minMax(partida, jugador);

    printf("Movent a %d\n", moviment);
    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}


void inciarPartida(selectorDeMoviment decisioJ1, selectorDeMoviment decisioJ2, double esperaEntreTorns){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova, 8, 8, 4);
    bool partidaEnCurs = true;
    char jugadors[2] = {1, -1};
    
    while (partidaEnCurs){
        for(int i=0; i<2; i++){
            sleep(esperaEntreTorns);
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %d. A quina columna vols ficar la peça?\n", 
                   jugadors[i] == 1 ? 1 : 2); 

            int moviment;
            if(jugadors[i]==1) moviment = decisioJ1(&prova, jugadors[i]);
            else moviment = decisioJ2(&prova, jugadors[i]);

            if(comprovarSolucio(&prova, moviment)){
                printf("\nHA GUANYAT EL JUGADOR %d\n", 
                       jugadors[i] == 1 ? 1 : 2);  // Para mostrar 1 o 2 en mensajes
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




int main(){
    srand(time(NULL));
    inciarPartida(triarMovimentBot, triarMovimentJugador, 0);
}