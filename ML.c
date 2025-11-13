#include<stdio.h>
#include<stdlib.h>

#include"ML.h"
#include"matrius.h"
#include"4enratlla.h"



#define PARAM_LReLU 0.5
#define ACTIVACIO leakyReLU


double leakyReLU(double x){
    if (x>=0) return x;
    else return PARAM_LReLU*x;
}


double **convolucio(double **matriu, int dimFilMat, int dimColMat, double **kernel, double biaix, funcioReal activacio){
    int dimKer = 1;
    int dimFilConvolucio = dimFilMat - dimKer+1;
    int dimColConvolucio = dimColMat - dimKer+1;
    double **matConvolucio = malloc(sizeof(double*)*dimFilConvolucio);
    for(int f=0; f<dimFilConvolucio; f++) matConvolucio[f]=malloc(sizeof(double)*dimColConvolucio);


    for(int f=0; f<dimFilConvolucio; f++){
        for(int c=0; c<dimColConvolucio; c++){
            
            matConvolucio[f][c] = biaix;
            //printf("Calculant (%i,%i)\n", f,c);
            for(int df=0; df<dimKer; df++){
                for(int dc=0; dc<dimKer; dc++){
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

void imprimirCapa(CapaXarxa *capa){
    for(int iKer=0; iKer<capa->nombreKernels; iKer++){
        printf("Imprimint ker %i:\n", iKer);
        for(int p=0;p<capa->nMatrius;p++){
            printf(" Imprimint slice %i:\n", p);
            imprimirMatriu(capa->kerners[iKer][p], capa->dimKer, capa->dimKer);
            printf("\n");
        }
        printf("\n");
    }
}

void imprimirXarxa(XarxaNeuronal *xarxa){
    for(int iCapa =0; iCapa<xarxa->nCapes; iCapa++){
        printf("Imprimint capa %i:\n", iCapa);
        imprimirCapa(xarxa->capes[iCapa]);
    }
}


double **aplicarKer(double ***llistaMatrius, int dimFilMat, int dimColMat, int profunditat, double ***kernel, int dimKer, double biaix, funcioReal activacio){

    int dimFilConvolucio = dimFilMat - dimKer+1;
    int dimColConvolucio = dimColMat - dimKer+1;
    double **matResultant = malloc(sizeof(double*)*dimFilConvolucio);
    for(int f=0; f<dimFilConvolucio; f++) matResultant[f]=malloc(sizeof(double)*dimColConvolucio);


    for(int f=0; f<dimFilConvolucio; f++){
        for(int c=0; c<dimColConvolucio; c++){
            double elementResultant = biaix;
            //printf("Calculant (%i,%i)\n", f,c);
            for(int iKer=0; iKer<profunditat; iKer++){
                for(int df=0; df<dimKer; df++){
                    for(int dc=0; dc<dimKer; dc++){
                        //printf("  (%i,%i): %.2f\n",df,dc,kernel[iKer][df][dc]*llistaMatrius[iKer][f+df][c+dc]);
                        elementResultant = elementResultant + kernel[iKer][df][dc]*llistaMatrius[iKer][f+df][c+dc];
                    }
                }
                //printf("\nImprimint intermitg\n");
                //imprimirMatriu(matResultant, dimFilConvolucio, dimColConvolucio);
            }
            //printf("%.1f ", matResultant[f][c]);
            matResultant[f][c] = activacio(elementResultant);
            //printf("%.1f\n", matResultant[f][c]);
        }
    }
    return matResultant;
}

double ***aplicarCapa(double ***llistaMatrius, CapaXarxa *capa){
    double ***matriusProcessades = malloc(capa->nombreKernels*sizeof(double**));
    
    for(int iKer=0; iKer<capa->nombreKernels; iKer++){
        
        matriusProcessades[iKer] = aplicarKer(
            llistaMatrius,
            capa->dimFil, capa->dimCol, capa->nMatrius,
            (capa->kerners)[iKer],
            capa->dimKer,
            (capa->biaixos)[iKer],
            capa->funcioActivacio
        );
    }
    return matriusProcessades;
}


double aplicarXarxa(double **matriu, XarxaNeuronal *xarxa){

    double ***matriuUnitaria = malloc(sizeof(double**));
    matriuUnitaria[0] = matriu;
    double ***resultatCapa = aplicarCapa(matriuUnitaria, xarxa->capes[0]);
    free(matriuUnitaria);

    for(int iCapa = 1; iCapa<xarxa->nCapes; iCapa++){
        double ***llistaAux = aplicarCapa(resultatCapa, xarxa->capes[iCapa]);
        alliberarLlistaMatrius(resultatCapa, xarxa->capes[iCapa-1]->nMatrius, xarxa->capes[iCapa-1]->dimFil);
        resultatCapa = llistaAux;
    }
    return resultatCapa[0][0][0];
}


CapaXarxa *crearCapaAleatoria(int dimKer, int nKer, int profunditatKer, int dimFilIn, int dimColIn){
    CapaXarxa *novaCapa = malloc(sizeof(CapaXarxa));
    novaCapa->nombreKernels = nKer;
    novaCapa->dimKer = dimKer;
    novaCapa->nMatrius = profunditatKer;
    novaCapa->nombreKernels;
    novaCapa->funcioActivacio = ACTIVACIO;
    novaCapa->dimFil = dimFilIn; novaCapa->dimCol = dimColIn;

    novaCapa->kerners = malloc(sizeof(double***)*nKer);
    novaCapa->biaixos = malloc(sizeof(double)*nKer);
    for(int iKer=0;iKer<nKer; iKer++){
        novaCapa->kerners[iKer] = malloc(sizeof(double**)*profunditatKer);
        novaCapa->biaixos[iKer] = 5*(((double)rand())/RAND_MAX - 0.5);
        for(int p=0; p<profunditatKer; p++){
            novaCapa->kerners[iKer][p] = malloc(sizeof(double*)*dimKer);
            for(int f=0; f<dimKer; f++){
                novaCapa->kerners[iKer][p][f] = malloc(sizeof(double)*dimKer);
                for(int c=0; c<dimKer; c++) novaCapa->kerners[iKer][p][f][c] = 5*(((double)rand())/RAND_MAX - 0.5);
            }
        }
    }
    
    return novaCapa;
}

XarxaNeuronal *crearXarxaAleatoria(int nCapes, int *llistaDimKernels, int *llistaNKernels, int dimFilIn, int dimColIn){
    XarxaNeuronal *novaXarxa = malloc(sizeof(XarxaNeuronal));
    novaXarxa->nCapes = nCapes;
    novaXarxa->capes = malloc(sizeof(CapaXarxa)*nCapes);

    int dimFil = dimFilIn;
    int dimCol = dimColIn;
    int profunditat = 1;
    
    for(int iCapa=0; iCapa<nCapes; iCapa++){
        novaXarxa->capes[iCapa] = crearCapaAleatoria(
            llistaDimKernels[iCapa],
            llistaNKernels[iCapa],
            profunditat, dimFil, dimCol);
        profunditat = llistaNKernels[iCapa];
        dimFil = dimFil -llistaDimKernels[iCapa] +1;
        dimCol = dimCol -llistaDimKernels[iCapa] +1;
    }
    return novaXarxa;
}




















void desarXarxa(XarxaNeuronal *xarxa, char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("ERROR");
        return;
    }

    fwrite(&xarxa->nCapes, sizeof(int), 1, f);

    for (int i = 0; i < xarxa->nCapes; i++) {
        CapaXarxa *capa = xarxa->capes[i];
        fwrite(&capa->nombreKernels, sizeof(int), 1, f);
        fwrite(&capa->nMatrius, sizeof(int), 1, f);
        fwrite(&capa->dimKer, sizeof(int), 1, f);
        fwrite(&capa->dimFil, sizeof(int), 1, f);
        fwrite(&capa->dimCol, sizeof(int), 1, f);

        fwrite(capa->biaixos, sizeof(double), capa->nombreKernels, f);

        for (int k = 0; k < capa->nombreKernels; k++) {
            for (int m = 0; m < capa->nMatrius; m++) {
                for (int iFila = 0; iFila < capa->dimKer; iFila++) {
                    fwrite(capa->kerners[k][m][iFila], sizeof(double), capa->dimKer, f);
                }
            }
        }
    }

    fclose(f);
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
    capa.nombreKernels = 1;
    capa.dimKer = 5;
    capa.nMatrius = 2;
    capa.funcioActivacio = ACTIVACIO;

    /* Reservar array de kernels (cada kernel és un tensor 3D) */
    capa.kerners = malloc(sizeof(double***) * capa.nombreKernels);
    capa.biaixos = malloc(sizeof(double) * capa.nombreKernels);
    if(!capa.kerners || !capa.biaixos){
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
        capa.kerners[k] = malloc(capa.nMatrius * sizeof(double**));
        if(!capa.kerners[k]){ perror("malloc kernel[k]"); return; }
        
        for(int iKer=0; iKer<capa.nMatrius; iKer++){
            capa.kerners[k][iKer] = malloc(capa.dimKer * sizeof(double*));
            if(!capa.kerners[k][iKer]){ perror("malloc kernel[k][iKer]"); return; }
            
            for(int i=0; i<capa.dimKer; i++){
                capa.kerners[k][iKer][i] = malloc(capa.dimKer * sizeof(double));
                if(!capa.kerners[k][iKer][i]){ perror("malloc kernel[k][iKer][i]"); return; }
                for(int j=0; j<capa.dimKer; j++){
                    capa.kerners[k][iKer][i][j] = 0.0;
                }
            }
        }
    }

    /* Donar una petita configuració als kernels per provar */
    for(int k=0; k<capa.nombreKernels; k++){
        for(int iKer=0; iKer<capa.nMatrius; iKer++){
            for(int i=1; i<4; i++){
                if(iKer==0){
                    capa.kerners[k][iKer][i][1] = 1.0;
                    capa.kerners[k][iKer][i][3] = -1.0;
                }
                else{
                    capa.kerners[k][iKer][1][i] = 1.0;
                    capa.kerners[k][iKer][3][i] = -1.0;
                }
                
            }
        }
    }
    printf("Kernels:\n");
    for(int i=0; i<capa.nombreKernels; i++) for(int j=0; j<capa.nMatrius;j++){
        imprimirMatriu(capa.kerners[i][j], capa.dimKer, capa.dimKer);
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
                free(capa.kerners[k][iKer][i]);
            }
            free(capa.kerners[k][iKer]);
        }
        free(capa.kerners[k]);
    }
    free(capa.kerners);
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

    imprimirXarxa(xarxaProva);
}

int main(){
    exempleXarxaAleatoria();
}