#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"Xarxa.h"
#include"partides.h"
#include"4enratlla.h"
#include"minmax.h"
#include"Utilitats.h"
#include"funcioUtilitat.h"

#define PROBABILITAT_ALEATORI 0.01

#define NOMBRE_CAPES 2


#define NOMBRE_SUPERVIVENTS 5
#define NOMBRE_FILLS 6
#define MIDA_POBLACIO (NOMBRE_FILLS*NOMBRE_SUPERVIVENTS)

#define DIM_TAULELL 8

#define LEARNING_RATE 0.001

typedef struct generacio{
    XarxaNeuronal *poblacio[MIDA_POBLACIO];
    int victories[MIDA_POBLACIO];
}Generacio;






int triarMovimentXarxa(QuatreEnRatlla *partida, char jugador, XarxaNeuronal *xarxa){
    int index;
    if (jugador==1) index = 0;
    else index = 1;

    double probabilitat = (double) rand() / RAND_MAX;
    int moviment;
    if (probabilitat<PROBABILITAT_ALEATORI){
        bool movimentValid = false;
        while (!movimentValid) {
            moviment = rand()%(partida->ncols);
            movimentValid = !comprovarColumnaPlena(partida, moviment);
        }
    } else moviment = minMax(partida, jugador, wrapperXarxa, xarxa);
    realitzarMoviment(partida,moviment,jugador);
    return moviment;
}



int enfrentamentXarxes(XarxaNeuronal *J1, XarxaNeuronal *J2){
    QuatreEnRatlla partidaCompeticio; //Es pot optmimizart treiant-ho.
    inicialitzarQuatreEnRatlla(&partidaCompeticio, DIM_TAULELL, DIM_TAULELL, 4);
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




//A partir d'una generació i saber quines son les millors xarxes genera la nova generació
void crearNovaGeneracio(Generacio *antigaGeneracio, int *millorsIndividus){
    for(int iSupervivent=0; iSupervivent<NOMBRE_SUPERVIVENTS; iSupervivent++){
        for(int iFamilia=0; iFamilia<NOMBRE_FILLS; iFamilia++){
            int iXarxa = iSupervivent*NOMBRE_FILLS + iFamilia;
            bool supervivent = false;
            for(int iSupervivent2=0; iSupervivent2<NOMBRE_SUPERVIVENTS;iSupervivent2++){
                if(millorsIndividus[iSupervivent2]==iXarxa){
                    supervivent = true;
                    break;   
                }
            }
            if(!supervivent){
                actualitzarXarxa(antigaGeneracio->poblacio[millorsIndividus[iSupervivent]], antigaGeneracio->poblacio[iXarxa], LEARNING_RATE);
            } 
        }
    }
}

//Enfrentes a la generació de xarxes entre elles i compta qui ha guanyat més
void torneigEnfrentaments(Generacio *generacioXarxes){
        for(int iJ1=0; iJ1<MIDA_POBLACIO; iJ1++) for(int iJ2=0; iJ2<MIDA_POBLACIO; iJ2++) if(iJ1!=iJ2){
        printf("Competint %ivs%i.\n", iJ1, iJ2);
        if (enfrentamentXarxes(generacioXarxes->poblacio[iJ1],generacioXarxes->poblacio[iJ2])==0) {
            generacioXarxes->victories[iJ1]++;
            printf("Ha guanyat la xarxa %i\n", iJ1);
        }

        else{
            generacioXarxes->victories[iJ2]++;
            printf("Ha guanyat la xarxa %i\n", iJ2);
        }
    }
}



//Validació


void validarXarxa(XarxaNeuronal *xarxa, int tornXarxa, int *nTorns, int *nVictories){
    QuatreEnRatlla partidaValidacio; //Es pot optmimizart treiant-ho.
    inicialitzarQuatreEnRatlla(&partidaValidacio, DIM_TAULELL, DIM_TAULELL, 4);

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
            if(jugadors[i]==tornXarxa) moviment = triarMovimentXarxa(&partidaValidacio, jugadors[i], xarxa);
            else moviment = triarMovimentBotAleatoriSenseText(&partidaValidacio, jugadors[i], &ctx);
            if(comprovarSolucio(&partidaValidacio, moviment)){
                if(jugadors[i]==tornXarxa) (*nVictories)++;
                alliberarQuatreEnRatlla(&partidaValidacio);
                return;
            }
            if(comprovarEmpat(&partidaValidacio)){
                alliberarQuatreEnRatlla(&partidaValidacio);
                return;
            }
        }
    }
}




void validarLlistaXarxes(XarxaNeuronal **llistaXarxes, int nXarxes){
    int movimentsTotalsJ1 = 0;
    int movimentsTotalsJ2 = 0;
    int victoriesTotalsJ1 = 0;
    int victoriesTotalsJ2 = 0;
    
    for(int iXarxa=0; iXarxa<nXarxes; iXarxa++){
        validarXarxa(llistaXarxes[iXarxa], 1, &movimentsTotalsJ1, &victoriesTotalsJ1);
    }
    for(int iXarxa=0; iXarxa<nXarxes; iXarxa++){
        validarXarxa(llistaXarxes[iXarxa], -1, &movimentsTotalsJ2, &victoriesTotalsJ2);
    }

    FILE *fp = fopen("Record.csv", "a");  // "a" = append al final

    if (fp == NULL) {
        perror("Error abriendo el archivo");
        return;
    }

    fprintf(fp, "%d,%d,%d,%d\n", movimentsTotalsJ1, movimentsTotalsJ2, victoriesTotalsJ1, victoriesTotalsJ2);

    fclose(fp);
}




//Una iteració (Fer torneix + seleccionar millors)
void iteracioEvolutiva(Generacio *generacioXarxes){

    torneigEnfrentaments(generacioXarxes);
    printf("VICTORIES:\n");
    for(int i=0; i<MIDA_POBLACIO; i++) printf("v(X%i)=%i ", i, generacioXarxes->victories[i]);
    int *indexMillorsMatrius = trobarKMaxims(generacioXarxes->victories, MIDA_POBLACIO, NOMBRE_SUPERVIVENTS);
    printf("MILLORS: ");
    for(int i=0; i<NOMBRE_SUPERVIVENTS; i++) printf("%i ", indexMillorsMatrius[i]);
    
    XarxaNeuronal **millorsMatrius = malloc(sizeof(XarxaNeuronal*)*NOMBRE_SUPERVIVENTS);
    for(int i=0; i<NOMBRE_SUPERVIVENTS; i++) millorsMatrius[i] = generacioXarxes->poblacio[indexMillorsMatrius[i]];
    validarLlistaXarxes(millorsMatrius,NOMBRE_SUPERVIVENTS);
    free(millorsMatrius);
    crearNovaGeneracio(generacioXarxes, indexMillorsMatrius);
    free(indexMillorsMatrius);
}



void entrenamentPerEnfrentaments(){
    int *nDimKer = malloc(sizeof(int)*NOMBRE_CAPES);
    nDimKer[0] = 3; nDimKer[1] = 3;
    
    int *LlistaNKer = malloc(sizeof(int)*NOMBRE_CAPES);
    LlistaNKer[0] = 3; LlistaNKer[1] = 3;

    Generacio generacioXarxes;
    for(int nXarxa=0; nXarxa<MIDA_POBLACIO; nXarxa++){
        generacioXarxes.victories[nXarxa] = 0;
        generacioXarxes.poblacio[nXarxa] = crearXarxaAleatoria(NOMBRE_CAPES,nDimKer,LlistaNKer,8,8);
    }

    free(nDimKer); free(LlistaNKer);
    for(int i=1; i<100000; i++){
        printf("Iteració %i.\n", i);
        iteracioEvolutiva(&generacioXarxes);
    }
    
    for(int iXarxa=0; iXarxa<MIDA_POBLACIO; iXarxa++) alliberarXarxa(generacioXarxes.poblacio[iXarxa]);
}



void validacioValidacio(){
    srand(9);
    int *nDimKer = malloc(sizeof(int)*NOMBRE_CAPES);
    nDimKer[0] = 3; nDimKer[1] = 3;
    
    int *LlistaNKer = malloc(sizeof(int)*NOMBRE_CAPES);
    LlistaNKer[0] = 3; LlistaNKer[1] = 3;

    Generacio generacioXarxes;
    for(int nXarxa=0; nXarxa<MIDA_POBLACIO; nXarxa++){
        generacioXarxes.victories[nXarxa] = 0;
        generacioXarxes.poblacio[nXarxa] = crearXarxaAleatoria(NOMBRE_CAPES,nDimKer,LlistaNKer,8,8);
    }
    printf("VAlidant:\n");
    validarLlistaXarxes(generacioXarxes.poblacio, 10);
}

void main(){
    entrenamentPerEnfrentaments();
}