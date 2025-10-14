#include "4enratlla.h"
#include "funcioUtilitat.h"
#include "minmax.h"


#include <unistd.h>
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


bool comprovarColumnaPlena(QuatreEnRatlla *partida, int moviment){
    if (partida->tauler[0][moviment] == 0) return false;
    else return true;
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


int filaSuperior(QuatreEnRatlla *partida, int columna){
    for(int f=0; f<NFILES; f++){
        if(partida->tauler[f][columna]!=0) return f;
    }
    return -1;
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

bool comprovarSolucio(QuatreEnRatlla *partida, int ultimMoviment){
    int filaPeça = filaSuperior(partida, ultimMoviment);
    return comprovarSolucioHoritzontal(partida, filaPeça, ultimMoviment) || comprovarSolucioVertical(partida, filaPeça, ultimMoviment) || comprovarSolucioDiagonal1(partida, filaPeça, ultimMoviment) || comprovarSolucioDiagonal2(partida, filaPeça, ultimMoviment);
}

bool comprovarEmpat(QuatreEnRatlla *partida){
    for(int col=0; col<NCOLS; col++) if(partida->tauler[0][col]==0) return false;
    return true;
}



