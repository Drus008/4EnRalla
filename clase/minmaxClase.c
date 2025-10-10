#include"minmax.h"

void inicialitzarPartida(Node* direccioPartida){
    direccioPartida->fills = malloc(NCOLS*sizeof(Node*));
}


void tirada(char tauler[NFILES][NCOLS], int numDeFil){
    //Determinar columna
    //Determinar fila
    //Posar la fitxa
}

Node* crearNode(Node *pare, int numDeFill){
    Node *p=malloc(sizeof(Node));
    copiaTauler(p->tauler, pare->tauler);
    tirada(p->tauler, numDeFill);
    if(nivell<2){
        p->nFills = calcularNumFills(p->tauler);
        p->fills = malloc(p->nFills*sizeof(Node*));
    }
    else{
        p->nFills=0;
        p->fills=NULL;
    }
    return p;
}

void crearNivell(Node *pare){
    for(int i=0; i<pare->nFills; i++){
        pare->fills[i] = crearNode(pare, i);
    }
}

void crearArbre(Node *arrel){
    crearNivell(arrel);
}

int main(){
    Node *arrel = malloc(sizeof(Node));
}