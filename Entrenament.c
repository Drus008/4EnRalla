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


void torneigEnfrentaments(Generacio *generacioXarxes){
    #pragma omp parallel for
    for(int iJ1=0; iJ1<generacioXarxes->midaPoblacio; iJ1++){
        for(int iJ2=0; iJ2<generacioXarxes->midaPoblacio; iJ2++) if(iJ1!=iJ2){
            int resultatEnfrentament = enfrentamentXarxes(generacioXarxes->poblacio[iJ1],generacioXarxes->poblacio[iJ2]);
            if (resultatEnfrentament==0) {
                #pragma omp atomic
                generacioXarxes->victories[iJ1]++;
                //printf("%ivs%i: %i\n", iJ1, iJ2, iJ1);
            }
            else if(resultatEnfrentament==1){
                #pragma omp atomic
                generacioXarxes->victories[iJ2]++;
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
            else moviment = triarMovimentBotAleatoriSenseText(partida, jugadors[i], &ctx);
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




void validarLlistaXarxes(XarxaNeuronal **llistaXarxes, int nXarxes, QuatreEnRatlla *partida){
    int movimentsTotalsJ1 = 0;
    int movimentsTotalsJ2 = 0;
    int victoriesTotalsJ1 = 0;
    int victoriesTotalsJ2 = 0;
    
    for(int iXarxa=0; iXarxa<nXarxes; iXarxa++){
        validarXarxa(llistaXarxes[iXarxa], 1, &movimentsTotalsJ1, &victoriesTotalsJ1, partida);
    }
    for(int iXarxa=0; iXarxa<nXarxes; iXarxa++){
        validarXarxa(llistaXarxes[iXarxa], -1, &movimentsTotalsJ2, &victoriesTotalsJ2, partida);
    }

    FILE *fp = fopen("Record.csv", "a"); 

    if (fp == NULL) {
        perror("Error abriendo el archivo");
        return;
    }

    fprintf(fp, "%d,%d,%d,%d\n", movimentsTotalsJ1, movimentsTotalsJ2, victoriesTotalsJ1, victoriesTotalsJ2);

    fclose(fp);
}




void iteracioEvolutiva(Generacio *generacioXarxes, QuatreEnRatlla *partida){
    torneigEnfrentaments(generacioXarxes);
    printf("VICTORIES: ");
    for(int i=0; i<generacioXarxes->midaPoblacio; i++) printf("v(X%i)=%i ", i, generacioXarxes->victories[i]);
    int *indexMillorsMatrius = trobarKMaxims(generacioXarxes->victories, generacioXarxes->midaPoblacio, generacioXarxes->nombreSupervivents);
    printf("MILLORS: ");
    for(int i=0; i<generacioXarxes->nombreSupervivents; i++) printf("%i ", indexMillorsMatrius[i]);
    printf("\n");
    int indexMillorXarxa=0;

    for(int i=1; i<generacioXarxes->nombreSupervivents; i++){
        if(generacioXarxes->victories[indexMillorsMatrius[i]]>generacioXarxes->victories[indexMillorXarxa]){
            indexMillorXarxa = i;
        }
    }
    desarXarxa(generacioXarxes->poblacio[indexMillorXarxa], "XarxaCalculada.DrusCNN");

    
    XarxaNeuronal **millorsMatrius = malloc(sizeof(XarxaNeuronal*)*generacioXarxes->nombreSupervivents);
    for(int i=0; i<generacioXarxes->nombreSupervivents; i++) millorsMatrius[i] = generacioXarxes->poblacio[indexMillorsMatrius[i]];
    validarLlistaXarxes(millorsMatrius,generacioXarxes->nombreSupervivents, partida);
    free(millorsMatrius);
    crearNovaGeneracio(generacioXarxes, indexMillorsMatrius);
    free(indexMillorsMatrius);
}



void entrenamentPerEnfrentaments(Generacio *generacioXarxes, QuatreEnRatlla *partida, int iteracions){

    int nombreCapes = 2;

    int *nDimKer = malloc(sizeof(int)*nombreCapes);
    nDimKer[0] = 5; nDimKer[1] = 3;
    
    int *LlistaNKer = malloc(sizeof(int)*nombreCapes);
    LlistaNKer[0] = 4; LlistaNKer[1] = 3;

    
    generacioXarxes->poblacio = malloc(generacioXarxes->midaPoblacio*sizeof(XarxaNeuronal*));
    generacioXarxes->victories = malloc(generacioXarxes->midaPoblacio*sizeof(int));

    for(int nXarxa=0; nXarxa<generacioXarxes->midaPoblacio; nXarxa++){
        generacioXarxes->victories[nXarxa] = 0;
        generacioXarxes->poblacio[nXarxa] = crearXarxaAleatoria(nombreCapes,nDimKer,LlistaNKer,partida->nfiles,partida->ncols);
    }

    free(nDimKer); free(LlistaNKer);
    for(int i=0; i<iteracions; i++){
        printf("Iteració %i.\n", i);
        iteracioEvolutiva(generacioXarxes, partida);


    }
    
    for(int iXarxa=0; iXarxa<generacioXarxes->midaPoblacio; iXarxa++) alliberarXarxa(generacioXarxes->poblacio[iXarxa]);
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


//gcc -fopenmp -ffast-math -funroll-loops -flto -march=native Entrenament.c 4enratlla.c  minmax.c Xarxa.c Utilitats.c partides.c funcioUtilitat.c -o entrenament -lm
/*
void main(){
    srand(9);
    
    Generacio generacioXarxes;
    generacioXarxes.nombreSupervivents = 4;
    generacioXarxes.nombreFills = 4;
    generacioXarxes.midaPoblacio = generacioXarxes.nombreSupervivents*generacioXarxes.nombreFills;
    generacioXarxes.learinngRate = 1;

    QuatreEnRatlla taulellEntrenament;
    inicialitzarQuatreEnRatlla(&taulellEntrenament, 8, 8, 4);

    entrenamentPerEnfrentaments(&generacioXarxes, &taulellEntrenament, 30);
}*/