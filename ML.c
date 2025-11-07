#include<stdio.h>
#include<stdlib.h>

#include"ML.h"
#include"4enratlla.h"

#define DIM_KER 5





double **convolucio(double **matriu, int dimFilMat, int dimColMat, double **kernel){
    int dimFilConvolucio = dimFilMat - DIM_KER+1;
    int dimColConvolucio = dimColMat - DIM_KER+1;
    double **matConvolucio = malloc(sizeof(double*)*dimFilConvolucio);
    for(int f=0; f<dimFilConvolucio; f++) matConvolucio[f]=malloc(sizeof(double*)*dimColConvolucio);


    int dimMitgKer = DIM_KER/2;
    for(int f=0; f<dimFilConvolucio; f++){
        for(int c=0; c<dimColConvolucio; c++){
            
            matConvolucio[f][c] = 0;
            printf("Calculant (%i,%i)\n", f,c);
            for(int df=0; df<=2*dimMitgKer; df++){
                for(int dc=0; dc<=2*dimMitgKer; dc++){
                    printf("  (%i,%i): %.2f\n",df,dc,kernel[df][dc]*matriu[f+df][c+dc]);
                    matConvolucio[f][c] = matConvolucio[f][c] + kernel[df][dc]*matriu[f+df][c+dc];
                }
            }
        }
    }
    return matConvolucio;
}
























void exempleConvolucio(){
    
    double **exemple = malloc(8*sizeof(double*));
    for(int i=0; i<8; i++){
        exemple[i] = malloc(8*sizeof(double));
        for(int j=0;j<4; j++) exemple[i][j] = 1;
    }
    for(int i=0; i<8; i++) for(int j=4;j<8; j++) exemple[i][j] = -1;
    
    double ker[5][5] = {{0,0,0,0,0},{0,1,0,-1,0},{0,1,0,-1,0},{0,1,0,-1,0},{0,0,0,0,0}};

    printf("Imprimint matriu\n");
    imprimirMatriu(exemple,8,8);



    printf("Iniciant convolucio\n");
    double **resultat = convolucio(exemple, 8,8,ker);
    printf("Imprimint matriu\n");
    imprimirMatriu(resultat,4,4);
}

int main(){

}