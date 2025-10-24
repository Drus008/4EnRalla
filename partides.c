#include "4enratlla.h"
#include "minmax.h"
#include "partides.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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


void pardidaPlayerVsBot(char tornJugador){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova, 8, 8, 4);
    bool partidaEnCurs = true;
    while (partidaEnCurs){
        for(char jugador=1; jugador<3; jugador++){
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %hhd. A quina columna vols ficar la peça?\n", jugador);
            int moviment;
            if(jugador==tornJugador) moviment = triarMovimentJugador(&prova, jugador);
            else {
                moviment = minMax(&prova, jugador);
                printf("Movent a %d\n", moviment);
                realitzarMoviment(&prova,moviment,jugador);
            }
            if(comprovarSolucio(&prova, moviment)){
                printf("\nHA GUANYAT EL JUGADOR %hhd\n", jugador);
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


void pardidaBotVsBot(double espera){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova, 20, 10, 6);
    bool partidaEnCurs = true;
    while (partidaEnCurs){
        for(char jugador=1; jugador<3; jugador++){
            sleep(espera);
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %hhd. A quina columna vols ficar la peça?\n", jugador);
            int moviment;
            moviment = minMax(&prova, jugador);
            printf("Movent a %d\n", moviment);
            realitzarMoviment(&prova,moviment,jugador);
            if(comprovarSolucio(&prova, moviment)){
                printf("\nHA GUANYAT EL JUGADOR %hhd\n", jugador);
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
}


void pardidaDeDosJugadors(){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova, 8, 8, 4);
    bool partidaEnCurs = true;
    while (partidaEnCurs){
        for(char jugador=1; jugador<3; jugador++){
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %hhd. A quina columna vols ficar la peça?\n", jugador);
            int moviment;
            moviment = triarMovimentJugador(&prova, jugador);
            if(comprovarSolucio(&prova, moviment)){
                printf("\nHA GUANYAT EL JUGADOR %hhd\n", jugador);
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
}


int main(){
    int configuracióDePartida[2] = {8,8};
    pardidaBotVsBot(0);
}