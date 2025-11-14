#include <stdio.h>
#include <stdlib.h>
#include<math.h>

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






// Fet pel xat (Marsaglia polar)
double rand_normal_fast() {
    static int has_spare = 0;
    static double spare;

    if (has_spare) {
        has_spare = 0;
        return spare;
    }
    
    has_spare = 1;

    double u, v, s;
    do {
        u = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
        v = (rand() / ((double) RAND_MAX)) * 2.0 - 1.0;
        s = u*u + v*v;
    } while (s >= 1.0 || s == 0.0);

    double mul = sqrt(-2.0 * log(s) / s);
    spare = v * mul;
    return u * mul;
}


#define PARAM_LReLU 0.5

double leakyReLU(double x){
    if (x>=0) return x;
    else return PARAM_LReLU*x;
}