#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"Xarxa.h"
#include"partides.h"
#include"4enratlla.h"
#include"minmax.h"
#include"Utilitats.h"
#include"funcioUtilitat.h"
#include"Entrenament.h"





int triarMovimentXarxa(QuatreEnRatlla *partida, char jugador, XarxaNeuronal *xarxa){
    int moviment = minMax(partida, jugador, wrapperXarxa, xarxa);
    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}



int enfrentamentXarxes(XarxaNeuronal *J1, XarxaNeuronal *J2){
    QuatreEnRatlla partidaCompeticio;
    int dimTaulellFil = J1->capes[0]->dimFil;
    int dimTaulellCol = J1->capes[0]->dimCol;
    inicialitzarQuatreEnRatlla(&partidaCompeticio, dimTaulellFil, dimTaulellCol, 4);
    bool partidaEnCurs = true;
    char jugadors[2] = {1, -1};
    
    while (partidaEnCurs){
        for(int i=0; i<2; i++){

            int moviment;
            if(jugadors[i]==1) moviment = triarMovimentXarxa(&partidaCompeticio, jugadors[i], J1);
            else moviment = triarMovimentXarxa(&partidaCompeticio, jugadors[i], J2);

            if(comprovarSolucio(&partidaCompeticio, moviment)){
                alliberarQuatreEnRatlla(&partidaCompeticio);
                return i;
            }
            if(comprovarEmpat(&partidaCompeticio)){
                alliberarQuatreEnRatlla(&partidaCompeticio);
                return -1;
            }
        }
    }
}



void crearNovaGeneracio(Generacio *antigaGeneracio, int *millorsIndividus){
    for(int iSupervivent=0; iSupervivent<antigaGeneracio->nombreSupervivents; iSupervivent++){
        for(int iFamilia=0; iFamilia<antigaGeneracio->nombreFills; iFamilia++){
            int iXarxa = iSupervivent*antigaGeneracio->nombreFills + iFamilia;
            antigaGeneracio->victories[iXarxa] = 0;
            bool supervivent = false;
            for(int iSupervivent2=0; iSupervivent2<antigaGeneracio->nombreSupervivents;iSupervivent2++){
                if(millorsIndividus[iSupervivent2]==iXarxa){
                    supervivent = true;
                    break;   
                }
            }
            if(!supervivent){
                actualitzarXarxa(antigaGeneracio->poblacio[millorsIndividus[iSupervivent]], antigaGeneracio->poblacio[iXarxa], antigaGeneracio->learinngRate);
            } 
        }
    }
}


void torneigEnfrentaments(Generacio *generacioXarxesJ1, Generacio *generacioXarxesJ2){
    #pragma omp parallel for
    for(int iJ1=0; iJ1<generacioXarxesJ1->midaPoblacio; iJ1++){
        for(int iJ2=0; iJ2<generacioXarxesJ2->midaPoblacio; iJ2++) {
            int resultatEnfrentament = enfrentamentXarxes(generacioXarxesJ1->poblacio[iJ1],generacioXarxesJ2->poblacio[iJ2]);
            if (resultatEnfrentament==0) {
                #pragma omp atomic
                generacioXarxesJ1->victories[iJ1]++;
                //printf("%ivs%i: %i\n", iJ1, iJ2, iJ1);
            }
            else if(resultatEnfrentament==1){
                #pragma omp atomic
                generacioXarxesJ2->victories[iJ2]++;
                //printf("%ivs%i: %i\n", iJ1, iJ2, iJ2);
            }
        }
        
    }
}



//Validació


void validarXarxa(XarxaNeuronal *xarxa, int tornXarxa, int *nTorns, int *nVictories, QuatreEnRatlla *partida){

    reiniciarQuatreEnRatlla(partida);
    ContextHeuristica ctx;
    ctx.funcio[0] = puntuacioPerAdjacencia;
    ctx.funcio[1] = puntuacioPerAdjacencia;
    ctx.altres[0] = NULL;
    ctx.altres[1] = NULL;

    bool partidaEnCurs = true;
    char jugadors[2] = {1, -1};
    while (partidaEnCurs){
        (*nTorns)++;
        for(int i=0; i<2; i++){
            
            int moviment;
            if(jugadors[i]==tornXarxa) moviment = triarMovimentXarxa(partida, jugadors[i], xarxa);
            else moviment = triarMovimentBotSenseText(partida, jugadors[i], &ctx);
            if(comprovarSolucio(partida, moviment)){
                if(jugadors[i]==tornXarxa) (*nVictories)++;
                return;
            }
            if(comprovarEmpat(partida)){
                return;
            }
        }
    }
}




void validarLlistaXarxes(XarxaNeuronal **llistaXarxesJ1, XarxaNeuronal **llistaXarxesJ2, int nXarxesJ1, int nXarxesJ2, QuatreEnRatlla *partida){
    int movimentsTotalsJ1 = 0;
    int movimentsTotalsJ2 = 0;
    int victoriesTotalsJ1 = 0;
    int victoriesTotalsJ2 = 0;
    
    for(int iXarxa=0; iXarxa<nXarxesJ1; iXarxa++){
        validarXarxa(llistaXarxesJ1[iXarxa], 1, &movimentsTotalsJ1, &victoriesTotalsJ1, partida);
    }
    for(int iXarxa=0; iXarxa<nXarxesJ1; iXarxa++){
        validarXarxa(llistaXarxesJ2[iXarxa], -1, &movimentsTotalsJ2, &victoriesTotalsJ2, partida);
    }

    FILE *fp = fopen("Record.csv", "a"); 

    if (fp == NULL) {
        perror("Error abriendo el archivo");
        return;
    }

    fprintf(fp, "%d,%d,%d,%d\n", movimentsTotalsJ1, movimentsTotalsJ2, victoriesTotalsJ1, victoriesTotalsJ2);

    fclose(fp);
}




void iteracioEvolutiva(Generacio *generacioXarxesJ1, Generacio *generacioXarxesJ2, QuatreEnRatlla *partida){
    torneigEnfrentaments(generacioXarxesJ1, generacioXarxesJ2);

    printf("VICTORIES:\nJ1: ");
    for(int i=0; i<generacioXarxesJ1->midaPoblacio; i++) printf("v(X%i)=%i ", i, generacioXarxesJ1->victories[i]);
    printf("J2: ");
    for(int i=0; i<generacioXarxesJ2->midaPoblacio; i++) printf("v(X%i)=%i ", i, generacioXarxesJ2->victories[i]);


    int *indexMillorsMatriusJ1 = trobarKMaxims(generacioXarxesJ1->victories, generacioXarxesJ1->midaPoblacio, generacioXarxesJ1->nombreSupervivents);
    printf("\nMILLORS:\nJ1: ");
    for(int i=0; i<generacioXarxesJ1->nombreSupervivents; i++) printf("%i ", indexMillorsMatriusJ1[i]);
    
    int *indexMillorsMatriusJ2 = trobarKMaxims(generacioXarxesJ2->victories, generacioXarxesJ2->midaPoblacio, generacioXarxesJ2->nombreSupervivents);
    printf("\nJ2: ");
    for(int i=0; i<generacioXarxesJ2->nombreSupervivents; i++) printf("%i ", indexMillorsMatriusJ2[i]);
    printf("\n");

    printf("Desant millor xarxa.");
    int indexMillorXarxaJ1=0;
    for(int i=1; i<generacioXarxesJ1->nombreSupervivents; i++){
        if(generacioXarxesJ1->victories[indexMillorsMatriusJ1[i]]>generacioXarxesJ1->victories[indexMillorXarxaJ1]){
            indexMillorXarxaJ1 = i;
        }
    }
    desarXarxa(generacioXarxesJ1->poblacio[indexMillorXarxaJ1], "XarxaEntrenadaJ1.DrusCNN");
    int indexMillorXarxaJ2=0;
    for(int i=1; i<generacioXarxesJ2->nombreSupervivents; i++){
        if(generacioXarxesJ2->victories[indexMillorsMatriusJ2[i]]>generacioXarxesJ2->victories[indexMillorXarxaJ2]){
            indexMillorXarxaJ2 = i;
        }
    }
    desarXarxa(generacioXarxesJ2->poblacio[indexMillorXarxaJ2], "XarxaEntrenadaJ2.DrusCNN");
    printf(" Xarxes desades.\n");
    printf(" Validant iteració.\n");
    XarxaNeuronal **millorsMatriusJ1 = malloc(sizeof(XarxaNeuronal*)*generacioXarxesJ1->nombreSupervivents);
    XarxaNeuronal **millorsMatriusJ2 = malloc(sizeof(XarxaNeuronal*)*generacioXarxesJ2->nombreSupervivents);
    for(int i=0; i<generacioXarxesJ1->nombreSupervivents; i++) millorsMatriusJ1[i] = generacioXarxesJ1->poblacio[indexMillorsMatriusJ1[i]];
    for(int i=0; i<generacioXarxesJ2->nombreSupervivents; i++) millorsMatriusJ2[i] = generacioXarxesJ2->poblacio[indexMillorsMatriusJ2[i]];
    validarLlistaXarxes(millorsMatriusJ1, millorsMatriusJ2, generacioXarxesJ1->nombreSupervivents, generacioXarxesJ2->nombreSupervivents, partida);
    free(millorsMatriusJ1);
    free(millorsMatriusJ2);
    crearNovaGeneracio(generacioXarxesJ1, indexMillorsMatriusJ1);
    crearNovaGeneracio(generacioXarxesJ2, indexMillorsMatriusJ2);
    free(indexMillorsMatriusJ1);
    free(indexMillorsMatriusJ2);
}



void entrenamentPerEnfrentaments(Generacio *generacioXarxesJ1, Generacio *generacioXarxesJ2, QuatreEnRatlla *partida, int iteracions){

    printf("Incialitzant xarxes aleatoriament.\n");
    int nombreCapes = 2;

    int *nDimKer = malloc(sizeof(int)*nombreCapes);
    nDimKer[0] = 5; nDimKer[1] = 3;
    
    int *LlistaNKer = malloc(sizeof(int)*nombreCapes);
    LlistaNKer[0] = 4; LlistaNKer[1] = 3;

    
    generacioXarxesJ1->poblacio = malloc(generacioXarxesJ1->midaPoblacio*sizeof(XarxaNeuronal*));
    generacioXarxesJ1->victories = malloc(generacioXarxesJ1->midaPoblacio*sizeof(int));
    generacioXarxesJ2->poblacio = malloc(generacioXarxesJ2->midaPoblacio*sizeof(XarxaNeuronal*));
    generacioXarxesJ2->victories = malloc(generacioXarxesJ2->midaPoblacio*sizeof(int));


    for(int nXarxa=0; nXarxa<generacioXarxesJ1->midaPoblacio; nXarxa++){
        generacioXarxesJ1->victories[nXarxa] = 0;
        generacioXarxesJ1->poblacio[nXarxa] = crearXarxaAleatoria(nombreCapes,nDimKer,LlistaNKer,partida->nfiles,partida->ncols);
    }
    for(int nXarxa=0; nXarxa<generacioXarxesJ1->midaPoblacio; nXarxa++){
        generacioXarxesJ2->victories[nXarxa] = 0;
        generacioXarxesJ2->poblacio[nXarxa] = crearXarxaAleatoria(nombreCapes,nDimKer,LlistaNKer,partida->nfiles,partida->ncols);
    }


    free(nDimKer); free(LlistaNKer);
    printf("Iniciant entrenament");
    for(int i=0; i<iteracions; i++){
        printf("Iteració %i.\n", i);
        iteracioEvolutiva(generacioXarxesJ1, generacioXarxesJ2, partida);
    }
    
    for(int iXarxa=0; iXarxa<generacioXarxesJ1->midaPoblacio; iXarxa++) alliberarXarxa(generacioXarxesJ1->poblacio[iXarxa]);
    for(int iXarxa=0; iXarxa<generacioXarxesJ2->midaPoblacio; iXarxa++) alliberarXarxa(generacioXarxesJ2->poblacio[iXarxa]);
}


/*
void validacioValidacio(){
    srand(9);
    int *nDimKer = malloc(sizeof(int)*NOMBRE_CAPES);
    nDimKer[0] = 3; nDimKer[1] = 3;
    
    int *LlistaNKer = malloc(sizeof(int)*NOMBRE_CAPES);
    LlistaNKer[0] = 3; LlistaNKer[1] = 3;

    Generacio generacioXarxes;
    generacioXarxes.nombreSupervivents = 10;
    generacioXarxes.nombreFills = 5;
    generacioXarxes.midaPoblacio = generacioXarxes.nombreFills*generacioXarxes.nombreFills;
    generacioXarxes.poblacio = malloc(generacioXarxes.midaPoblacio*sizeof(XarxaNeuronal*));
    generacioXarxes.victories = malloc(generacioXarxes.midaPoblacio*sizeof(int));

    for(int nXarxa=0; nXarxa<generacioXarxes.midaPoblacio; nXarxa++){
        generacioXarxes.victories[nXarxa] = 0;
        generacioXarxes.poblacio[nXarxa] = crearXarxaAleatoria(NOMBRE_CAPES,nDimKer,LlistaNKer,8,8);
    }
    printf("VAlidant:\n");
    validarLlistaXarxes(generacioXarxes.poblacio, 10, 8, 8);
}*/


//gcc -fopenmp -funroll-loops -flto -march=native Entrenament.c 4enratlla.c  minmax.c Xarxa.c Utilitats.c partides.c funcioUtilitat.c -O3 -o entrenament -lm

void main(){
    srand(time(NULL));


    
    Generacio generacioXarxesJ1;Generacio generacioXarxesJ2;
    generacioXarxesJ1.nombreSupervivents = 50; generacioXarxesJ2.nombreSupervivents = 50;
    generacioXarxesJ1.nombreFills = 10; generacioXarxesJ2.nombreFills = 10;
    generacioXarxesJ1.midaPoblacio = generacioXarxesJ1.nombreSupervivents*generacioXarxesJ1.nombreFills;
    generacioXarxesJ2.midaPoblacio = generacioXarxesJ2.nombreSupervivents*generacioXarxesJ2.nombreFills;
    generacioXarxesJ1.learinngRate = 1; generacioXarxesJ2.learinngRate = 1;

    QuatreEnRatlla taulellEntrenament;
    inicialitzarQuatreEnRatlla(&taulellEntrenament, 8, 8, 4);

    entrenamentPerEnfrentaments(&generacioXarxesJ1, &generacioXarxesJ2, &taulellEntrenament, 300);
}