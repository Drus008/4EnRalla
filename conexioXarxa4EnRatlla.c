#include "4enratlla.h"
#include "Xarxa.h"

#include "stdlib.h"


/**
 * @file conexioXarxa4EnRatlla.c
 * @brief Implementa un wrapper per a poder fer servir una xarxa com a funció heurística.
 */

double wrapperXarxa(QuatreEnRatlla *partida, void *xarxaCtx){

    XarxaNeuronal *xarxa = (XarxaNeuronal*) xarxaCtx;

    int nFil = partida->nfiles; int nCol = partida->ncols;
    double **matriu = malloc(sizeof(double*)*nFil);
    for(int f=0; f<partida->nfiles; f++){
        matriu[f] = malloc(sizeof(double)*nCol);
        for(int c=0;c<partida->ncols;c++){
            matriu[f][c] = (double) (partida->tauler[f][c]);
        }
    }
    double resultat = aplicarXarxa(matriu, xarxa);
    for(int f=0; f<nFil; f++) free(matriu[f]);
    free(matriu);
    return resultat;
}
