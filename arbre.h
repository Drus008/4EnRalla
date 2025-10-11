#ifndef ARBRE_H
#define ARBRE_H

#define PROFUNDITAT 2

typedef struct arbre {
    int data;
    struct Node* left;
    struct arbre* fills;
} Arbre;



#endif // ARBRE_H