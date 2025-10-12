#include "4enratlla.h"
#include "funcioUtilitat.h"
#include "minmax.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void imprimirQuateEnRatlla(QuatreEnRatlla *partida){
    for(int fil = 0; fil<NFILES; fil++){
        for(int col = 0; col<NCOLS; col++){
            if((partida->tauler)[fil][col]==1) printf("|🟥");
            else if((partida->tauler)[fil][col]==2) printf("|🟦");
            else(printf("|  "));
        }
        printf("|\n");
    }
}

void inicialitzarQuatreEnRatlla(QuatreEnRatlla *partida){
    for(int fil=0; fil<NFILES; fil++){
        for(int col=0; col<NCOLS; col++){
            partida->tauler[fil][col] = 0;
        }
    }
}

//Retorna true si el moviment es pot fer i false en cas contrari
bool comprovarMovimentLegal(QuatreEnRatlla *partida, int moviment){
    if (partida->tauler[0][moviment] == 0) return true;
    else return false;
}

void realitzarMoviment(QuatreEnRatlla *partida, int moviment, char jugador){
    for(int fil = NFILES-1; fil>=0; fil--){
        if(partida->tauler[fil][moviment]==0) {
            partida->tauler[fil][moviment] = jugador;
            break;
        }
    }
}

void desferMoviment(QuatreEnRatlla *partida, int moviment){
    partida->tauler[filaSuperior(partida, moviment)][moviment] = 0;
}

// donat un joc del 4enRalla y una columna, retorna la fila en la que hi ha la peça superior
int filaSuperior(QuatreEnRatlla *partida, int columna){
    for(int f=0; f<NFILES; f++){
        if(partida->tauler[f][columna]!=0) return f;
    }
}//Potser millor que això és incloure al 4enRalla la fila en que s'acaba cada columna



// Aquest codi és horrible, estaria bé una funció que ho englobes tot i fos eficient
bool comprovarSolucioHoritzontal(QuatreEnRatlla *partida, int fila, int col){
    char casellaInicial = partida->tauler[fila][col]; //Crec que es millor si això ho paso com a parametre en les 4 funcions
    int comptadorFitxer = 1;
    for(int i=1; i<MODIFICADOR; i++){
        if(col+i>=0 && col+i<NCOLS){
            if(partida->tauler[fila][col + i]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    if (comptadorFitxer==MODIFICADOR) return true;
    for(int i=1; i<MODIFICADOR; i++){
        if(col-i>=0 && col-i<NCOLS){
            if(partida->tauler[fila][col - i]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    if (comptadorFitxer>=MODIFICADOR) return true;
    return false;
}
bool comprovarSolucioVertical(QuatreEnRatlla *partida, int fila, int col){
    char casellaInicial = partida->tauler[fila][col];
    int comptadorFitxer = 1;
    for(int i=1; i<MODIFICADOR; i++){
        if(fila+i>=0 && fila+i<NFILES){
            if(partida->tauler[fila + i][col]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    if (comptadorFitxer>=MODIFICADOR) return true;
    return false;
}
bool comprovarSolucioDiagonal1(QuatreEnRatlla *partida, int fila, int col){
    char casellaInicial = partida->tauler[fila][col];
    int comptadorFitxer = 1;
    for(int i=1; i<MODIFICADOR; i++){
        if(col+i>=0 && col+i<NCOLS && fila+i>=0 && fila+i<NFILES){
            if(partida->tauler[fila + i][col+i]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    if (comptadorFitxer>=MODIFICADOR) return true;
    for(int i=1; i<MODIFICADOR; i++){
        if(col-i>=0 && col-i<NCOLS && fila-i>=0 && fila-i<NFILES){
            if(partida->tauler[fila - i][col-i]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    if (comptadorFitxer>=MODIFICADOR) return true;
    return false;
}
bool comprovarSolucioDiagonal2(QuatreEnRatlla *partida, int fila, int col){
    char casellaInicial = partida->tauler[fila][col];
    int comptadorFitxer = 1;
    for(int i=1; i<MODIFICADOR; i++){
        if(col-i>=0 && col-i<NCOLS && fila+i>=0 && fila+i<NFILES){
            if(partida->tauler[fila + i][col-i]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    
    if (comptadorFitxer>=MODIFICADOR) return true;
    for(int i=1; i<MODIFICADOR; i++){
        if(col+i>=0 && col+i<NCOLS && fila-i>=0 && fila-i<NFILES){
            if(partida->tauler[fila - i][col+i]!=casellaInicial) break;
            else comptadorFitxer++;
        }
    }
    if (comptadorFitxer>=MODIFICADOR) return true;
    return false;
}

// Donat un joc del 4enRalla i l'ultim moviment fet, retorna cert si s'ha guañat la partida
bool comprovarSolucio(QuatreEnRatlla *partida, int ultimMoviment){
    int filaPeça = filaSuperior(partida, ultimMoviment);
    return comprovarSolucioHoritzontal(partida, filaPeça, ultimMoviment) || comprovarSolucioVertical(partida, filaPeça, ultimMoviment) || comprovarSolucioDiagonal1(partida, filaPeça, ultimMoviment) || comprovarSolucioDiagonal2(partida, filaPeça, ultimMoviment);
}

// Donat un joc del 4enRalla retorna cert si ha empatat la partida
bool comprovarEmpat(QuatreEnRatlla *partida){
    int filaSup = NFILES-1;
    for(int j=0; j<NCOLS; j++){
        if(partida->tauler[j][filaSup]==0){ return false; }
    }
    return true;
}

int triarMovimentJugador(QuatreEnRatlla *partida, char jugador){
    int moviment = -1;
    while (moviment==-1){
        printf("Intodueix una columna: ");

        if (scanf(" %d", &moviment)!=1){
            while (getchar() != '\n'); //atois
            printf("Moviment invàlid.\n");
            moviment = -1;
        }
        
        else if(moviment>=0 && moviment<NCOLS && comprovarMovimentLegal(partida,moviment)) realitzarMoviment(partida, moviment, jugador);
        else{
            printf("Moviment il·legal.\n");
            moviment = -1;
        }
    }
}



void pardidaDeDosJugadors(){
    QuatreEnRatlla prova;
    inicialitzarQuatreEnRatlla(&prova);
    bool partidaEnCurs = true;
    while (partidaEnCurs){
        for(char jugador=1; jugador<3; jugador++){
            printf("Puntuacio: %i\n", puntuacioPerAdjacencia(&prova));
            imprimirQuateEnRatlla(&prova);
            printf("Torn del jugador %hhd. A quina columna vols ficar la peça?\n", jugador);
            int moviment;
            if(jugador==1) moviment = triarMovimentJugador(&prova, jugador);
            else {
                Arbre *arbreProba = malloc(sizeof(Arbre));
                arbreProba->nivell = 0;
                moviment = ferMinmax(arbreProba,&prova, jugador); //S'ha d'arreglar que es fiqui be el jugador
                imprimirArbre(arbreProba,3,0);
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

void main(){
    pardidaDeDosJugadors();
}