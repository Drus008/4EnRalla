
#include"Xarxa.h"




/**
 * @brief Estructura que controla com es formen les generacions.
 */
typedef struct generacio{
    XarxaNeuronal **poblacio; /**<Array d'apuntadors a les xarxes neuronals que conforma la població */
    int *victories; /**<Array que desa a la posicií i la quantitat de victories que ha aconseguit la xarxa i */
    int nombreSupervivents; /**<El nombre de supervivents d'una iteració */
    int nombreFills; /**<El nombre de fills que té cada supervivent */
    int midaPoblacio; /**<La mida total de la població (ha de ser nombreSupervivents*midaPoblacio) */
    int learinngRate; /**<El ratio d'aprenentatge */
}Generacio;




/**
 * @brief Tria el moviment que fa una xarxa neuronal (aplican minmax) segons l'estat de la partida y el jugador al que li toqui tirar
 * 
 * @param partida és l'estat actual de la partida.
 * @param jugador és al jugador que li toca tirar actualment.
 * @param xarxa és la xarxa que farà de funció heurística
 * 
 * Compleix la classe selector de moviment.
 * 
 * 
 * @return El moviment que ha decidit la xarxa. Només realitza moviments válids
 */
int triarMovimentXarxa(QuatreEnRatlla *partida, char jugador, XarxaNeuronal *xarxa);


/**
 * @brief Enfrenta dues xarxes neuronals i retorna el guanyador.
 * 
 * @param J1 és un apuntador a la xarxa neuronal que començarà tirant.
 * @param J2 és un apuntador a la xarxa neuronal que tirarà al segón torn.
 * 
 * @return Retorna 0 si ha juanyat J1 i 1 si ha guanyat J2.
 * 
 */
int enfrentamentXarxes(XarxaNeuronal *J1, XarxaNeuronal *J2);


/**
 * @brief Crea una nova generació de xarxes a partir dels millors individus, modificant aquests lleugerament.
 * 
 * @param antigaGeneracio és la generació que es vol evolucionar.
 * @param millorsIndividus és un array amb els index del millors indiviuds de la generació.
 * 
 */
void crearNovaGeneracio(Generacio *antigaGeneracio, int *millorsIndividus);

/**
 * @brief Enfrenta a la generació de xarxes per a veure quina guanya més.
 * 
 * Enfrentes totes les xarxes en un format de lliga. Si la xarxa amb identifiacdor "i" guanya aleshores si li suma 1 a victories[i] de la seva generacio.
 * 
 * @param generacioXarxesJ1 és la generació de xarxes que es vol enfrentar i que començaran tirant.
 * @param generacioXarxesJ2 és la generació de xarxes que es vol enfrentar i que moura en segón lloc.
 * 
 */
void torneigEnfrentaments(Generacio *generacioXarxesJ1, Generacio *generacioXarxesJ2);




/**
 * 
 * @brief Simula una partida de la xarxa contra la IA de validació indicant el nombre de moviments i qui ha guanyat.
 * 
 * @param xarxa és la xarxa que es vol validar.
 * @param tornXarxa indica si la xarxa és el J1 o J2 (1 per a J1 i -1 per a J2)
 * @param nTorns variable on es registraran els torns fets. En acabar la funció a aquest paràmetre se li ha afegit el nombre de moviments fets en total.
 * @param nVictories variable on és registrarà si la xarxa ha guanyat. Se li sumarà 1 en cas de que guanyi i res en cas contrari.
 * @param partida és un apuntador a la partida en la que es validaran les xarxes. Es reinicia el taulell de la partida.
 * 
 */
void validarXarxa(XarxaNeuronal *xarxa, int tornXarxa, int *nTorns, int *nVictories, QuatreEnRatlla *partida);


/**
 * @brief Valida una llista de xarxes neuronals fent-les competir contra un algorisme predefinit i afegeix els reultats en un arxiu anomenat Record.csv
 * L'arxiu Record.csv conté 4 columnes:
 * - Columna 1: Nombre de moviments fets com a J1
 * - Columna 2: Nombre de moviments fets com a J2
 * - Columna 3: Nombre de victories com a J1
 * - Columna 4: Nombre de victories com a J2
 * 
 * Cada fila és una nova validació.
 * 
 * @param llistaXarxesJ1 és l'array de CNN que es volen avaluar en el primer torn de partida.
 * @param llistaXarxesJ1 és l'array de CNN que es volen avaluar en el segon torn de partida.
 * @param nXarxesJ1 és la longitut de l'array llista xarxes de primer torn.
 * @param nXarxesJ2 és la longitut de l'array llista xarxes de segon torn.
 * @param partida és el taulell on es validaran les xarxes
 * 
 */
void validarLlistaXarxes(XarxaNeuronal **llistaXarxesJ1, XarxaNeuronal **llistaXarxesJ2, int nXarxesJ1, int nXarxesJ2, QuatreEnRatlla *partida);


/**
 * @brief Realitza una iteració de l'algorisme evolutiu i guarda la xarxa.
 * 
 * Desa la xarxa neuronal del J1 a un arxiu anomenat "XarxaEntrenadaJ1.DrusCNN" i la J2 a un arxiu anomenat "XarxaEntrenadaJ2.DrusCNN".
 * 
 * @param generacioXarxesJ1 és la generació de xarxes que es vol fer evolucionar per a millorar en el primer torn. En acabar la iteració ja retorna la generació evolucionada.
 * @param generacioXarxesJ1 és la generació de xarxes que es vol fer evolucionar per a millorar en el segon torn. En acabar la iteració ja retorna la generació evolucionada.
 * @param partida és el taulell on competeixen les xarxes.
 */
void iteracioEvolutiva(Generacio *generacioXarxesJ1, Generacio *generacioXarxesJ2, QuatreEnRatlla *partida);


/**
 * @brief Crea una generació de xarxes y les va evolucionant per a arribar a una bona funció heurística.
 * 
 * @param generacioXarxesJ1 és una generació sense inicialitzar. Només ha de tenir creats els paràmtres ints, però no el *victories ni l'array de xarxes. Serà la que competirà iniciant la partida.
 * @param generacioXarxesJ2 és una generació sense inicialitzar. Només ha de tenir creats els paràmtres ints, però no el *victories ni l'array de xarxes. Serà la que competirà fent el segon moviemnt.
 * @param partida és un apuntador a un QuatreEnRatlla inicialitzada i buida.
 * @param iteracions és el nombre d'iteracions que fa l'entrenament.
 * 
 */
void entrenamentPerEnfrentaments(Generacio *generacioXarxesJ1, Generacio *generacioXarxesJ2, QuatreEnRatlla *partida, int iteracions);
