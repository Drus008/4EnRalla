

typedef struct capaXarxa{
    int nombreKernels;
    double ***kerners;
}CapaXarxa;


typedef struct CNN{
    int capes;
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
double **convolucio(double **matriu, int dimFilMat, int dimColMat, double **kernel);