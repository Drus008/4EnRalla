#include<stdio.h>
#include<stdlib.h>

#include"ML.h"
#include"matrius.h"
#include"4enratlla.h"

#define DIM_KER 5


#define PARAM_LReLU 0.5
#define ACTIVACIO leakyReLU

double leakyReLU(double x){
    if (x>=0) return x;
    else return PARAM_LReLU*x;
}


double **convolucio(double **matriu, int dimFilMat, int dimColMat, double **kernel, double biaix, funcioActivacio activacio){
    int dimFilConvolucio = dimFilMat - DIM_KER+1;
    int dimColConvolucio = dimColMat - DIM_KER+1;
    double **matConvolucio = malloc(sizeof(double*)*dimFilConvolucio);
    for(int f=0; f<dimFilConvolucio; f++) matConvolucio[f]=malloc(sizeof(double)*dimColConvolucio);


    int dimMitgKer = DIM_KER/2;
    for(int f=0; f<dimFilConvolucio; f++){
        for(int c=0; c<dimColConvolucio; c++){
            
            matConvolucio[f][c] = biaix;
            //printf("Calculant (%i,%i)\n", f,c);
            for(int df=0; df<=2*dimMitgKer; df++){
                for(int dc=0; dc<=2*dimMitgKer; dc++){
                    //printf("  (%i,%i): %.2f\n",df,dc,kernel[df][dc]*matriu[f+df][c+dc]);
                    matConvolucio[f][c] = matConvolucio[f][c] + kernel[df][dc]*matriu[f+df][c+dc];
                }
            }
            printf("%.1f ", matConvolucio[f][c]);
            matConvolucio[f][c] = activacio(matConvolucio[f][c]);
            printf("%.1f\n", matConvolucio[f][c]);
        }
    }
    return matConvolucio;
}



double ***aplicarCapa(double ***llistaMatrius, int nombreMatrius, CapaXarxa *capa){
    int nombreMatriusProcesades = nombreMatrius*(capa->nombreKernels);
    double ***matriusProcessades = malloc(nombreMatriusProcesades*sizeof(double**));
    for(int iMat=0; iMat<nombreMatrius; iMat++){
        double **matriuActual = llistaMatrius[iMat];
        for(int iKer=0; iKer<capa->nombreKernels; iKer++){
            double **kerActual = (capa->kerners)[iKer];
            double biaixActual = (capa->biaixos)[iKer];
            int indexNovaMatriu = iMat*capa->nombreKernels + iKer;
            matriusProcessades[indexNovaMatriu] = convolucio(matriuActual,capa->dimFil, capa->dimCol, kerActual, biaixActual, ACTIVACIO);
        }
    }
    return matriusProcessades;
}





















void exempleConvolucio(){
    printf("Creant funcions");
    double **exemple = malloc(8*sizeof(double*));
    for(int i=0; i<8; i++){
        exemple[i] = malloc(8*sizeof(double));
        for(int j=0;j<4; j++) exemple[i][j] = 1;
    }
    for(int i=0; i<8; i++) for(int j=4;j<8; j++) exemple[i][j] = -1;
    
    double **ker = malloc(5*sizeof(double*));
    for(int i=0; i<5; i++){
        ker[i] = malloc(5*sizeof(double));
        for(int j=0;j<5;j++) ker[i][j] = 0;
    }
    for(int i=1;i<4;i++){
        ker[i][1] = 1;
        ker[i][3] = -1;
    }
    printf("Imprimint matriu\n");
    imprimirMatriu(exemple,8,8);



    printf("Iniciant convolucio\n");
    double **resultat = convolucio(exemple, 8,8,ker, 1, ACTIVACIO);
    printf("Imprimint matriu\n");
    imprimirMatriu(resultat,4,4);
}






void exempleCapa(){
    printf("Creant capa\n");
    CapaXarxa capa;
    capa.dimCol = 8;
    capa.dimFil = 8;
    capa.nombreKernels = 2;
    capa.kerners = malloc(sizeof(double**)*capa.nombreKernels);
    capa.biaixos =malloc(sizeof(double)*capa.nombreKernels);

    for(int i=0; i<capa.nombreKernels; i++) capa.biaixos[i] = -1;

    printf("Creant funcions\n");
    double ***matrius = malloc(2*sizeof(double**));
    matrius[0] = malloc(8*sizeof(double*));
    matrius[1] = malloc(8*sizeof(double*));
    for(int i=0; i<8; i++){
        matrius[0][i] = malloc(8*sizeof(double));
        matrius[1][i] = malloc(8*sizeof(double));
    }
    for(int i=0; i<8; i++){
        for(int j=0;j<4; j++){
            matrius[0][i][j] = 1;
            matrius[1][j][i] = 1;
        }
        for(int j=4;j<8; j++) {
        matrius[0][i][j] = 0;
        matrius[1][j][i] = 0;
        }
    }
    
    capa.kerners[0] = malloc(5*sizeof(double*));
    capa.kerners[1] = malloc(5*sizeof(double*));
    for(int i=0; i<5; i++){
        capa.kerners[0][i] = malloc(5*sizeof(double));
        capa.kerners[1][i] = malloc(5*sizeof(double));
        for(int j=0;j<5;j++) {
            capa.kerners[0][i][j] = 0;
            capa.kerners[1][i][j] = 0;
        }
    }
    for(int i=1;i<4;i++){
        capa.kerners[0][i][1] = 1;
        capa.kerners[0][i][3] = -1;
        capa.kerners[1][1][i] = 1;
        capa.kerners[1][3][i] = -1;
    }
    printf("Imprimint matriu\n");
    imprimirMatriu(matrius[0],8,8);
    printf("\n");
    imprimirMatriu(matrius[1],8,8);



    printf("Iniciant convolucio\n");
    double ***resultat = aplicarCapa(matrius, 2, &capa);
    printf("Imprimint matriu\n");
    
    for(int i =0; i<4; i++){
        imprimirMatriu(resultat[i],4,4);
        printf("\n");
    }

}


int main(){
    exempleCapa();
}