#include <stdio.h>
#include <stdlib.h>

void alliberarLlistaMatrius(double ***llistaMatrius, int nMatrius, int dimFil){
    for(int i = 0; i<nMatrius; i++){
        for(int f=0; f<dimFil; f++) free(llistaMatrius[i][f]);
        free(llistaMatrius[i]);
    }
    free(llistaMatrius);
}


void imprimirMatriu(double **matriu, int dimFil, int dimCol){
    for(int f=0; f<dimFil; f++){
        for(int c=0; c<dimCol; c++){
            printf("%.2f ", matriu[f][c]);
        }
        printf("\n");
    }
}