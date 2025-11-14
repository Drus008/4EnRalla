#ifndef XARXA_H
#define XARXA_H


#define LEARNING_RATE


#define ACTIVACIO leakyReLU

/**
 * @brief Tipus de dada que representa la funció d'activació
 */
typedef double (*funcioReal)(double);


/**
 * @brief Estructura que conté una capa de la xarxa
 */
typedef struct capaXarxa{
    int nombreKernels; /**<Nombre de kernels que té la capa */
    double ****kerners; /**<Nuclis de la capa */
    double *biaixos; /**<Biaixos de cada kernel */
    funcioReal funcioActivacio;
    int dimKer; /**<Dimensió dels kernels */
    int nMatrius; /**<Nombre de matrius a processar que coincideix amb la profunditat dels kernels */
    int dimFil; /**<Nombre de files que tenen les matrius a processar */
    int dimCol; /**<Nombre de columnes que tenen les matrius a processar */
}CapaXarxa;


typedef struct CNN{
    int nCapes;
    CapaXarxa **capes;
} XarxaNeuronal;





/**
 * @brief Realitza una convolució matricial
 * 
 * Per a la gestió dels bordes s'usa valid (ignora els bordes i la matriu resultatn és més petita).
 * 
 * 
 * @param matriu és la matriu a la que se li realitza la convolució.
 * @param dimFilMat nombre de files de la matriu.
 * @param dimColMat nombre de columnes de la matriu.
 * 
 * @param kernel és el kernel que s'aplica a la matriu. Ha der ser de mida 5x5.
 * 
 * 
 * @return Retorna la convolució entre la matriu y el kernel.
 */
double **convolucio(double **matriu, int dimFilMat, int dimColMat, double **kernel, double biaix, funcioReal activacio);




/**
 * @brief Imprimeix els kernels d'una capa
 * 
 * @param capa és la capa que es vol imprimir
 */
void imprimirCapa(CapaXarxa *capa);

/**
 * @brief Imprimeix els kernels de les capes d'una xarxa
 * 
 * @param xarxa és la xarxa que es vol imprimir
 */
void imprimirXarxa(XarxaNeuronal *xarxa);

/**
 * @brief Donades una llista de matrius de longitut igual a la profunditat del kernels d'una llista, aplica la convolució dels kernels.
 * 
 * 
 * @param llistaMatrius és l'array de matrius al que s'els hi vol aplicar la convolució dels kernels. Totes han de tenir les mateixes dimensions. L'array ha de tenir la mateixa longitud que la profunditat del kernel.
 * @param dimFilMat nombre de files que tenen les matrius.
 * @param dimColMat nombre de columnes que tenen les matrius
 * @param profunditat profunditat que té el kernel (que coincideix amb el nombre de matrius de l'array llistaMatrius).
 * @param kernel és el kernel que es vol aplicar.
 * @param dimKer és el nombre de files/columnes que té el kernel.
 * @param biaix és el biaix que s'aplica a la convolució.
 * @param activacio és la funció d'activació que s'aplica un cop afegit el viaix.
 * 
 * @return Retorna la matriu resultant de l'aplicació del kernel a la llista de matrius.
 */
double **aplicarKer(double ***llistaMatrius, int dimFilMat, int dimColMat, int profunditat, double ***kernel, int dimKer, double biaix, funcioReal activacio);

/**
 * @brief Donades una llista de matrius els hi aplica una capa d'una CNN.
 * 
 * 
 * @param llistaMatrius és l'array de matrius al que s'els hi vol aplicar la capa. Totes han de tenir les mateixes dimensions. L'array ha de tenir la mateixa longitud que la profunditat dels kernels.
 * @param capa és la capa que es vol aplicar.
 * 
 * @return Retorna l'array de matrius resultant de l'aplicació dels kernels de la capa a la llista de matrius.
 */
double ***aplicarCapa(double ***llistaMatrius, CapaXarxa *capa);

/**
 * @brief Donada una matrius li aplica una xarxa neuronal.
 * 
 * 
 * @param matriu és la matriu inicial.
 * @param xarxa és la xarxa que es vol aplicar a la matriu.
 * 
 * @return Retorna el valor resultant d'aplicar la xarxa a la matriu.
 */
double aplicarXarxa(double **matriu, XarxaNeuronal *xarxa);


/**
 * @brief Crea una capa amb núclis amb valors aleatoris.
 * 
 * 
 * @param dimKer és el nombre de files/columnes que tindran els kernels.
 * @param nKer és el nombre de kernels que tindrà la capa.
 * @param profunditatKer és la profunditat que tindrán els kernels.
 * @param dimFilIn dimensió de les matrius d'entrada.
 * @param dimColIn dimensió de les columnes d'entrada.
 * 
 * @return Retorna l'apuntador a la capa creada.
 */
CapaXarxa *crearCapaAleatoria(int dimKer, int nKer, int profunditatKer, int dimFilIn, int dimColIn);


/**
 * @brief Crea una xarxa amb valors aleatoris
 * 
 * Està ajustada per a que totes les dimensions quadrin.
 * 
 * Això vol dir que cada capa està preparada per a rebre tantes matrius com kernels té la capa anterior.
 * I aquestes son de dimensió igual que l'anterior però reduïdes en un nombre igual que la meitat de la mida dels kernels més 1.
 * 
 * @param nCapes és el nombre de capes de la xarxa
 * @param llistaDimKernels és un array on l'element i és la dimensió que tenen els kernels de la capa i.
 * @param llistaNKernels és un array on l'element i és el nombre de kernels de la capa i.
 * @param dimFilIn és el nombre de files de la matriu que reb com a entrada la xarxa.
 * @param dimColIn és el nombre de columnes de la matriu que reb com a entrada la xarxa.
 * 
 * @return Retorna l'apuntador a la xarxa creada.
 */
XarxaNeuronal *crearXarxaAleatoria(int nCapes, int *llistaDimKernels, int *llistaNKernels, int dimFilIn, int dimColIn);


/**
 * @brief allibera una Xarxa
 * 
 * 
 * @param xarxa és la xarxa que es vol alliberar.
 */
void alliberarXarxa(XarxaNeuronal *xarxa);


/**
 * @brief Fa una copia d'una xarxa neuronar canviant els valors lleugerament seguint una distribució normal
 * 
 * Les dues xarxes han de tenir les mateixes dimensions.
 * 
 * @param xarxaOriginal és de la que és copien els valors
 * @param xarxaClon és on s'aplicaran els valors modificats
 * @param sigma és la desviació de la distribució normal (Marsaglia polar) que s'utilitza per a variar els valors
 */
void actualitzarXarxa(XarxaNeuronal *xarxaOriginal, XarxaNeuronal *xarxaClon, double sigma);





/**
 * @brief Desa una xarxa neuronal a un arxiu.
 * 
 * Fet per la IA.
 * 
 * @param xarxa és la xarxa que es vol desar.
 * @param filename és el nom del fitxer en el que es vol desar la xarxa.
 */
void desarXarxa(XarxaNeuronal *xarxa, const char *filename);

/**
 * @brief Carrega una xarxa neuronal des d'un arxiu.
 * 
 * Fet per la IA.
 * 
 * @param filename és el nom del fitxer des del que es vol carregar la xarxa.
 * 
 * @return un apuntador a la xarxa carregada.
 */
XarxaNeuronal *carregarXarxa(const char *filename);


#endif