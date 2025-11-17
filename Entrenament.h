
#include"Xarxa.h"


#define PROBABILITAT_ALEATORI 0.01

#define NOMBRE_CAPES 2


#define NOMBRE_SUPERVIVENTS 10
#define NOMBRE_FILLS 6
#define MIDA_POBLACIO (NOMBRE_FILLS*NOMBRE_SUPERVIVENTS)

#define DIM_TAULELL 8

#define LEARNING_RATE 0.001



typedef struct generacio{
    XarxaNeuronal *poblacio[MIDA_POBLACIO];
    int victories[MIDA_POBLACIO];
}Generacio;




/**
 * @brief Tria el moviment que fa una xarxa neuronal segons l'estat de la partida y el jugador al que li toqui tirar
 * 
 * @param partida és l'estat actual de la partida.
 * @param jugador és al jugador que li toca tirar actualment.
 * @param xarxa és la xarxa que farà de funció heurística
 * 
 * Compleix la classe selector de moviment.
 * 
 * Realitza amb una petita provabilitat un moviment aleatori.
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
 * @brief Crea una nova generació de xarxes a partir dels millors individus.
 * 
 * @param antigaGeneracio és la generació que es vol evolucionar.
 * @param millorsIndividus és un array amb els index del millors indiviuds de la generació.
 * 
 */
void crearNovaGeneracio(Generacio *antigaGeneracio, int *millorsIndividus);

/**
 * @brief Enfrenta a la generació de xarxes per a veure quina guanya més.
 * 
 * Enfrentes totes les xarxes en un format de lliga. Si la xarxa amb id i guanya aleshores si li afegeix 1 a victories[i] de generacioXarxes.
 * 
 * @param generacioXarxes és la generació de xarxes que es vol enfrentar
 * 
 */
void torneigEnfrentaments(Generacio *generacioXarxes);




/**
 * 
 * @brief Simula una partida de la xarxa contra la IA de validació indicant el nombre de moviments i qui ha guanyat.
 * 
 * @param xarxa és la xarxa que es vol validar.
 * @param tornXarxa indica si la xarxa és el J1 o J2 (1 per a J1 i -1 per a J2)
 * @param nTorns variable on es registraran els torns fets. En acabar la funció a aquest paràmetre se li ha afegit el nombre de moviments fets en total.
 * @param nVictories variable on és registrarà si la xarxa ha guanyat. Se li sumarà 1 en cas de que guanyi i res en cas contrari.
 * 
 */
void validarXarxa(XarxaNeuronal *xarxa, int tornXarxa, int *nTorns, int *nVictories);


/**
 * @brief Valida una llista de xarxes neuronals fent-les competir contra un algorisme predefinit i afegeix els reultats en un arxiu anomenat Record.csv
 * L'arxiu Record.csv conté 4 columnes:
 * - Columna 1: Nombre de moviments fets com a J1
 * - Columna 2: Nombre de moviments fets com a J2
 * - Columna 3: Nombre de victories com a J1
 * - Columna 4: Nombre de victories com a J2
 * 
 * Cada fila és una nova validació.
 */
void validarLlistaXarxes(XarxaNeuronal **llistaXarxes, int nXarxes);


/**
 * @brief Realitza una iteració de l'algorisme evolutiu.
 * 
 * @param generacioXarxes és la generació de xarxes que es vol fer evolucionar. En acabar la iteració ja retorna la generació evolucionada.
 * 
 */
void iteracioEvolutiva(Generacio *generacioXarxes);


/**
 * @brief Crea una generació de xarxes y les va evolucionant per a arribar a una bona funció heurística.
 */
void entrenamentPerEnfrentaments();
