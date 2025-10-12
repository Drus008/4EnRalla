#include "4enratlla.h"
#include "minmax.h"
#include "partides.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void pardidaPlayerVsBot(char tornJugador){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova);
    bool partidaEnCurs = true;
    while (partidaEnCurs){
        for(char jugador=1; jugador<3; jugador++){
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %hhd. A quina columna vols ficar la peça?\n", jugador);
            int moviment;
            if(jugador==tornJugador) moviment = triarMovimentJugador(&prova, jugador);
            else {
                Arbre *arbreProba = malloc(sizeof(Arbre));
                arbreProba->nivell = 0;
                moviment = ferMinmax(arbreProba,&prova, jugador);
                imprimirArbre(arbreProba,3);
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
}


void pardidaBotVsBot(double espera){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova);
    bool partidaEnCurs = true;
    while (partidaEnCurs){
        for(char jugador=1; jugador<3; jugador++){
            sleep(espera);
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %hhd. A quina columna vols ficar la peça?\n", jugador);
            int moviment;
            Arbre *arbreProba = malloc(sizeof(Arbre));
            arbreProba->nivell = 0;
            moviment = ferMinmax(arbreProba,&prova, jugador);
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
    inicialitzarQuatreEnRatlla(&prova);
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
    pardidaPlayerVsBot(1);
}