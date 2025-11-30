#include "Xarxa.h"
#include "Utilitats.h"

#include<stdlib.h>
#include<stdio.h>

/**
 * @file ValidacioML.c
 * @brief Fitxer per a validar les xarxes.
 * 
 * @warning Segurament està anticuat i algunes funcións d'han d'actualitzar.
 * 
 */



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
    capa.nombreKernels = 1;
    capa.dimKer = 5;
    capa.nMatrius = 2;
    capa.funcioActivacio = ACTIVACIO;

    /* Reservar array de kernels (cada kernel és un tensor 3D) */
    capa.kernels = malloc(sizeof(double***) * capa.nombreKernels);
    capa.biaixos = malloc(sizeof(double) * capa.nombreKernels);
    if(!capa.kernels || !capa.biaixos){
        perror("malloc capa arrays");
        return;
    }
    for(int i=0;i<capa.nombreKernels;i++) capa.biaixos[i] = -1.0;

    /* Crear matrius d'entrada (nMatrius x 8 x 8) */
    double ***matrius = malloc(capa.nMatrius * sizeof(double**));
    if(!matrius){ perror("malloc matrius"); return; }
    for(int m=0;m<capa.nMatrius;m++){
        matrius[m] = malloc(capa.dimFil * sizeof(double*));
        if(!matrius[m]){ perror("malloc matrius[m]"); return; }
        for(int i=0;i<capa.dimFil;i++){
            matrius[m][i] = malloc(capa.dimCol * sizeof(double));
            if(!matrius[m][i]){ perror("malloc matrius[m][i]"); return; }
        }
    }

    /* Inicialitzar entrades amb patrons senzills per validar */
    for(int m=0;m<capa.nMatrius;m++){
        for(int i=0;i<capa.dimFil;i++){
            for(int j=0;j<capa.dimCol;j++){
                if (m==0) matrius[m][i][j] = (j < 4) ? 1.0 : 0.0;
                else       matrius[m][i][j] = (i < 4) ? 1.0 : 0.0;
            }
        }
    }

    /* Reservar tots els kernels com tensors 3D (nombreKernels x profunditat x dimKer x dimKer) */
    for(int k=0; k<capa.nombreKernels; k++){
        /* Cada kernel[k] és un tensor 3D: profunditat canals, cadascun dimKer x dimKer */
        capa.kernels[k] = malloc(capa.nMatrius * sizeof(double**));
        if(!capa.kernels[k]){ perror("malloc kernel[k]"); return; }
        
        for(int iKer=0; iKer<capa.nMatrius; iKer++){
            capa.kernels[k][iKer] = malloc(capa.dimKer * sizeof(double*));
            if(!capa.kernels[k][iKer]){ perror("malloc kernel[k][iKer]"); return; }
            
            for(int i=0; i<capa.dimKer; i++){
                capa.kernels[k][iKer][i] = malloc(capa.dimKer * sizeof(double));
                if(!capa.kernels[k][iKer][i]){ perror("malloc kernel[k][iKer][i]"); return; }
                for(int j=0; j<capa.dimKer; j++){
                    capa.kernels[k][iKer][i][j] = 0.0;
                }
            }
        }
    }

    /* Donar una petita configuració als kernels per provar */
    for(int k=0; k<capa.nombreKernels; k++){
        for(int iKer=0; iKer<capa.nMatrius; iKer++){
            for(int i=1; i<4; i++){
                if(iKer==0){
                    capa.kernels[k][iKer][i][1] = 1.0;
                    capa.kernels[k][iKer][i][3] = -1.0;
                }
                else{
                    capa.kernels[k][iKer][1][i] = 1.0;
                    capa.kernels[k][iKer][3][i] = -1.0;
                }
                
            }
        }
    }
    printf("Kernels:\n");
    for(int i=0; i<capa.nombreKernels; i++) for(int j=0; j<capa.nMatrius;j++){
        imprimirMatriu(capa.kernels[i][j], capa.dimKer, capa.dimKer);
        printf("\n");
    }
    
    printf("Imprimint entrades:\n");
    for(int m=0; m<capa.nMatrius; m++){
        printf("Entrada %d:\n", m);
        imprimirMatriu(matrius[m], capa.dimFil, capa.dimCol);
        printf("\n");
    }

    /* Aplicar capa i validar resultat */
    double ***resultat = aplicarCapa(matrius, &capa);

    int dimOutFil = capa.dimFil - capa.dimKer + 1;
    int dimOutCol = capa.dimCol - capa.dimKer + 1;
    printf("Resultat: nombre kernels = %d, cada resultat és %dx%d\n", capa.nombreKernels, dimOutFil, dimOutCol);

    for(int k=0; k<capa.nombreKernels; k++){
        if(!resultat[k]){
            printf("Resultat[%d] = NULL\n", k);
            continue;
        }
        printf("Resultat kernel %d:\n", k);
        imprimirMatriu(resultat[k], dimOutFil, dimOutCol);
        /* alliberar la matriu resultat[k] */
        for(int i=0; i<dimOutFil; i++) free(resultat[k][i]);
        free(resultat[k]);
    }
    free(resultat);

    /* Alliberar entrades */
    for(int m=0; m<capa.nMatrius; m++){
        for(int i=0; i<capa.dimFil; i++) free(matrius[m][i]);
        free(matrius[m]);
    }
    free(matrius);

    /* Alliberar kernels i biaixos */
    for(int k=0; k<capa.nombreKernels; k++){
        for(int iKer=0; iKer<capa.nMatrius; iKer++){
            for(int i=0; i<capa.dimKer; i++){
                free(capa.kernels[k][iKer][i]);
            }
            free(capa.kernels[k][iKer]);
        }
        free(capa.kernels[k]);
    }
    free(capa.kernels);
    free(capa.biaixos);

    printf("exempleCapa finalitzat i memòria alliberada\n");
}

void exempleCapaAleatoria(){
    int dimKer = 5;
    int nKer = 2;
    int prof = 3;
    CapaXarxa *capaProva = crearCapaAleatoria(dimKer,nKer,prof, 20,20);

    imprimirCapa(capaProva);
}

void exempleXarxaAleatoria(){
    int nCapes = 3;
    int llistaDimsKers[] = {5,3,3};
    int llistaNombreKers[] = {3, 4, 1};
    int dimFilIn = 10;
    int dimColIn = 12;
    XarxaNeuronal *xarxaProva = crearXarxaAleatoria(nCapes,llistaDimsKers,llistaNombreKers,dimFilIn, dimColIn);
    printf("XARXA INICIAL:\n");
    imprimirXarxa(xarxaProva);

    actualitzarXarxa(xarxaProva, xarxaProva, 0.1);
    printf("SEGONA XARXA\n");
    imprimirXarxa(xarxaProva);
}

void exempleDesarXarxa(){
    int nCapes = 3;
    int llistaDimsKers[] = {5,3,3};
    int llistaNombreKers[] = {3, 4, 1};
    int dimFilIn = 10;
    int dimColIn = 12;
    XarxaNeuronal *xarxaProva = crearXarxaAleatoria(nCapes,llistaDimsKers,llistaNombreKers,dimFilIn, dimColIn);
    printf("XARXA INICIAL:\n");
    imprimirXarxa(xarxaProva);
    printf("DESANT XARXA:\n");
    desarXarxa(xarxaProva, "XarxaProva");
    printf("ALLIBERANT XARXA\n");
    printf("CARREGANT XARXA\n");
    alliberarXarxa(xarxaProva);
    xarxaProva = carregarXarxa("XarxaProva");
    imprimirXarxa(xarxaProva);
}


void exempleAplicarXarxa(){
    int nCapes = 3;
    int llistaDimsKers[] = {5,3,3};
    int llistaNombreKers[] = {3, 4, 1};
    int dimFilIn = 10;
    int dimColIn = 12;
    XarxaNeuronal *xarxaProva = crearXarxaAleatoria(nCapes,llistaDimsKers,llistaNombreKers,dimFilIn, dimColIn);
    printf("XARXA INICIAL:\n");
    imprimirXarxa(xarxaProva);

    double **matriu = malloc(sizeof(double*)*dimFilIn);
    for(int f=0; f<dimFilIn; f++) {
        matriu[f] = malloc(sizeof(double)*dimColIn);
        for(int c=0; c<dimColIn; c++){
            matriu[f][c] = f*f*c*c%19-8;
        }
    }
    imprimirMatriu(matriu,dimFilIn, dimColIn);
    double result = aplicarXarxa(matriu,xarxaProva);
    printf("RESULTAT %lf.\n", result);
}

//gcc -g ValidacioML.c Utilitats.c Xarxa.c -o ValidacioML -lm
int main(){
    exempleAplicarXarxa();
}