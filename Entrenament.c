#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include"Xarxa.h"
#include"partides.h"
#include"4enratlla.h"
#include"minmax.h"
#include"Utilitats.h"

#define PROBABILITAT_ALEATORI 0.03

#define NOMBRE_CAPES 2

#define MIDA_POBLACIO 20


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
    inicialitzarQuatreEnRatlla(&partidaCompeticio, 8, 8, 4);
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


void entrenamentPerEnfrentaments(){
    //srand(time(NULL));
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
    for(int iJ1=0; iJ1<MIDA_POBLACIO; iJ1++) for(int iJ2=0; iJ2<MIDA_POBLACIO; iJ2++) if(iJ1!=iJ2){
        printf("Competint %ivs%i.\n", iJ1, iJ2);
        if (enfrentamentXarxes(generacioXarxes.poblacio[iJ1],generacioXarxes.poblacio[iJ2])==0) {
            generacioXarxes.victories[iJ1]++;
            printf("Ha guanyat la xarxa %i\n", iJ1);
        }

        else{
            generacioXarxes.victories[iJ2]++;
            printf("Ha guanyat la xarxa %i\n", iJ2);
        }
    }


    printf("VICTORIES:\n");
    for(int i=0; i<MIDA_POBLACIO; i++) printf("Xarxa %i: %i\n", i, generacioXarxes.victories[i]);

    int *millorsMatrius = trobarKMaxims(generacioXarxes.victories, MIDA_POBLACIO, 10);
    printf("MILLORS: ");
    for(int i=0; i<10; i++) printf("%i ", millorsMatrius[i]);

    for(int iXarxa=0; iXarxa<MIDA_POBLACIO; iXarxa++) alliberarXarxa(generacioXarxes.poblacio[iXarxa]);
}

void main(){
    entrenamentPerEnfrentaments();
}