

/**
 * @brief Tipus de dada que representa la funció d'activació
 */
typedef double (*funcioActivacio)(double);

/**
 * @brief Estructura que conté una capa de la xarxa
 */
typedef struct capaXarxa{
    int nombreKernels; /**<Nombre de kernels que té la capa */
    double ***kerners; /**<Nuclis de la capa */
    double *biaixos; //**<Biaixos de cada kernel */
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
double **convolucio(double **matriu, int dimFilMat, int dimColMat, double **kernel, double biaix, funcioActivacio activacio);




