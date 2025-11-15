#include<stdio.h>
#include<stdlib.h>

#include"Xarxa.h"
#include"Utilitats.h"
#include"4enratlla.h"


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
    printf("nKers: %i, dimKers: %i, dimMats: %ix%i\n", capa->nombreKernels, capa->dimKer, capa->dimFil, capa->dimCol);
    for(int iKer=0; iKer<capa->nombreKernels; iKer++){
        printf("Imprimint ker %i: Biaix: %.2lf\n", iKer,capa->biaixos[iKer]);
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
            
            if (elementResultant<0) elementResultant = elementResultant*0.1; //He implementat la funció d'activació directament aquí.
            matResultant[f][c] = elementResultant; //Hi ha una millora en quant al temps d'execusió. No caldria passar la funció com a paràmetre.
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
            capa->funcioActivacio);
    }
    return matriusProcessades;
}


double aplicarXarxa(double **matriu, XarxaNeuronal *xarxa){

    double ***matriuUnitaria = malloc(sizeof(double**));
    matriuUnitaria[0] = matriu;
    double ***resultatCapa = aplicarCapa(matriuUnitaria, xarxa->capes[0]);
    free(matriuUnitaria);
    
    for(int iCapa = 1; iCapa<xarxa->nCapes; iCapa++){
        
        double ***llistaAux = aplicarCapa(resultatCapa, xarxa->capes[iCapa]); //Per a fer això més eficient es pot pasar llistaAux com a parametre per a no have-la de crear/alliberar cada cop
        alliberarLlistaMatrius(resultatCapa, xarxa->capes[iCapa]->nMatrius, xarxa->capes[iCapa]->dimFil);
        resultatCapa = llistaAux;
    }


    CapaXarxa *ultimaCapa = xarxa->capes[xarxa->nCapes-1];
    double valorFinal = 0;
    for(int p=0; p<ultimaCapa->nombreKernels; p++){
        for(int f=0; f<ultimaCapa->dimFil - ultimaCapa->dimKer +1; f++){
            for(int c=0; c<ultimaCapa->dimCol - ultimaCapa->dimKer +1; c++){
                valorFinal = valorFinal + resultatCapa[p][f][c];
            }
            free(resultatCapa[p][f]);
        }
        free(resultatCapa[p]);
    }
    free(resultatCapa);

    return valorFinal;
}


CapaXarxa *crearCapaAleatoria(int dimKer, int nKer, int profunditatKer, int dimFilIn, int dimColIn){
    CapaXarxa *novaCapa = malloc(sizeof(CapaXarxa));
    novaCapa->nombreKernels = nKer;
    novaCapa->dimKer = dimKer;
    novaCapa->nMatrius = profunditatKer;
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
    novaXarxa->capes = malloc(sizeof(CapaXarxa*)*nCapes);

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



void alliberarXarxa(XarxaNeuronal *xarxa) {
    for (int i = 0; i < xarxa->nCapes; i++) {
        CapaXarxa *capa = xarxa->capes[i];

        for (int k = 0; k < capa->nombreKernels; k++) {
            for (int m = 0; m < capa->nMatrius; m++) {
                for (int f = 0; f < capa->dimKer; f++) {
                    free(capa->kerners[k][m][f]);
                }
                free(capa->kerners[k][m]);
            }
            free(capa->kerners[k]);
        }
        free(capa->kerners);
        free(capa->biaixos);
        free(capa);
    }
    free(xarxa->capes);
    free(xarxa);
}

void actualitzarXarxa(XarxaNeuronal *xarxaOriginal, XarxaNeuronal *xarxaClon, double sigma){
    for(int capa=0; capa<xarxaOriginal->nCapes; capa++){
        CapaXarxa *capaActual = xarxaClon->capes[capa];
        CapaXarxa *capaOriginal = xarxaOriginal->capes[capa];
        for(int iKer=0; iKer<capaActual->nombreKernels; iKer++){
            capaActual->biaixos[iKer] = capaOriginal->biaixos[iKer] + rand_normal_fast()*sigma; 
            for(int p=0; p<capaActual->nMatrius; p++){
                for(int f=0; f<capaActual->dimKer; f++) for(int c=0; c<capaActual->dimKer; c++){
                    //printf("capa:%i, iKer:%i, p:%i, f:%i, c:%i\n", capa, iKer, p, f, c);
                    capaActual->kerners[iKer][p][f][c] = capaOriginal->kerners[iKer][p][f][c] + rand_normal_fast()*sigma; 
                }
            }
        }
    }
}





void desarXarxa(XarxaNeuronal *xarxa, const char *filename) {
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



XarxaNeuronal *carregarXarxa(const char *filename){
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error abriendo archivo para leer");
        return NULL;
    }

    XarxaNeuronal *xarxa = malloc(sizeof(XarxaNeuronal));
    fread(&xarxa->nCapes, sizeof(int), 1, f);

    xarxa->capes = malloc(xarxa->nCapes * sizeof(CapaXarxa*));

    for (int i = 0; i < xarxa->nCapes; i++) {
        CapaXarxa *capa = malloc(sizeof(CapaXarxa));
        fread(&capa->nombreKernels, sizeof(int), 1, f);
        fread(&capa->nMatrius, sizeof(int), 1, f);
        fread(&capa->dimKer, sizeof(int), 1, f);
        fread(&capa->dimFil, sizeof(int), 1, f);
        fread(&capa->dimCol, sizeof(int), 1, f);

        capa->biaixos = malloc(capa->nombreKernels * sizeof(double));
        fread(capa->biaixos, sizeof(double), capa->nombreKernels, f);

        
        capa->kerners = malloc(capa->nombreKernels * sizeof(double***));
        for (int k = 0; k < capa->nombreKernels; k++) {
            capa->kerners[k] = malloc(capa->nMatrius * sizeof(double**));
            for (int m = 0; m < capa->nMatrius; m++) {
                capa->kerners[k][m] = malloc(capa->dimKer * sizeof(double*));
                for (int iFila = 0; iFila < capa->dimKer; iFila++) {
                    capa->kerners[k][m][iFila] = malloc(capa->dimKer * sizeof(double));
                    fread(capa->kerners[k][m][iFila], sizeof(double), capa->dimKer, f);
                }
            }
        }

        capa->funcioActivacio = NULL; 
        xarxa->capes[i] = capa;
    }

    fclose(f);
    return xarxa;
}





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
