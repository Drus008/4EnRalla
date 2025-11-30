#ifndef UTILITATS_H
#define UTILITATS_H



/**
 * @file Utilitats.h
 */


/**
 * @brief Donat un array de matrius amb el mateix nombre de files, les allibera totes.
 * 
 * @param llistaMatrius és la llista de matrius que vols alliberar
 * @param nMatrius és el nombre de matrius que vols alliberar
 * @param dimFil és el nombre de files que tenen les matrius
 */
void alliberarLlistaMatrius(double ***llistaMatrius, int nMatrius, int dimFil);

/**
 * @brief Imprimeix una matriu
 * 
 * @param matriu és la matriu que vols imprimir
 * @param dimFil és el nombre de files que té la matriu
 * @param dimCol és el nombre de columnes que té la matriu
 */
void imprimirMatriu(double **matriu, int dimFil, int dimCol);

/**
 * @brief Fa una copia profunda d'una matriu de doubles.
 * 
 * @param matriu és la matriu que vols copiar.
 * @param dimFil és el nombre de files que té la matriu.
 * @param dimCol és el nombre de columnes que té la matriu.
 * 
 * @return Retorna un apuntador a la nova matriu.
 */
double **deepCopyMatriu(double **matriu, int dimFil, int dimCol);


/**
 * @brief Trova els k índexos amb valors més grans d'una llista.
 * 
 * @param llista és la llista de la que es volen trobar els màxims.
 * @param midaLlista és la longitud de la llista.
 * @param k és el nombre de valors més grans que vols trobar.
 * 
 * @return una llista de mida k amb els índexos dels valors més grans.
 */
int *trobarKMaxims(int *llista, int midaLlista, int k);


/**
 * @brief Genera un nombre aleatori amb una distribució aproximada de la normal estandar
 * 
 * @warning S'ha de vigilar perque aquesta funció no es pot fer servir en paral·le degut a que fa servir variables static.
 * 
 * 
 * @return el nombre aleatori
 */
double rand_normal_fast();

/**
 * @brief Rectificador Lineal Unitari Leaky.
 * 
 * @param x és la variable de la funció.
 * 
 * @return x si x>=0 y a*x si x<0.
 * 
 */
double leakyReLU(double x);

#endif