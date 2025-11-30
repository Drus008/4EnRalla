# 4 en Ratlla amb IA Evolutiva (CNN + MinMax)

## Descripcio
Implementacio completa del joc Quatre en Ratlla en llenguatge C. El projecte inclou una Xarxa Neuronal Convolucional (CNN) implementada des de zero (sense llibreries externes de ML) que apren a jugar mitjancant un algorisme genetic i evolucio competitiva.

El bot utilitza l'algorisme MinMax prenent com a funció heurística la xarxa neuronal per decidir el millor moviment.

## Caracteristiques Principals
* Motor de Joc: Implementacio del 4 en Ratlla amb deteccio de victories.
* Intel.ligencia Artificial:
  * Algorisme MinMax per a la presa de decisions.
  * CNN propia amb capes convolucionals i activacio Leaky ReLU.
* Entrenament Evolutiu:
  * Sistema de generacio de poblacions i torneigs (Lliga tots contra tots).
  * Mutacio genetica (soroll gaussia) sobre els pesos de la xarxa.
  * Paral.lelisme amb OpenMP per accelerar l'entrenament.
* Documentacio: Codi font completament documentat amb Doxygen.

## Requisits
* Compilador C: GCC o Clang.
* OpenMP: Necessari per a la paral.lelitzacio de l'entrenament.
* Llibreries Estandard: math.h, stdlib.h, stdio.h, omp.h.

## Compilacio

#### Entrenament
Abans de compilar assegura't d'haver variat els paràmetres d'entrenament del main:
- El nombre de supervivents de cada iteració de cada xarxa.
- El nombre de fills que té cada supervivent.
- El learningRate de cada xarxa.
- Les dimensions del taulell i la longitud de k-en ratlla.
A més és recomanable reduïr el PROFUNDITAT de l'arxiu minmax.h a 3.

Per compilar l'entrenament, utilitza la seguent comanda:

```
gcc -fopenmp -funroll-loops -flto -march=native Entrenament.c 4enratlla.c  minmax.c Xarxa.c Utilitats.c funcioUtilitat.c conexioXarxa4EnRatlla.c -O3 -o entrenament -lm
```

#### Partida

Abans de compilar assegura't d'haver variat els paràmetres de la funció validacioXarxa:
- El nom de la xarxa que vols carregar (en cas de voler fer servir una xarxa).
- Quina funció heurística fa servir cada jugador (en cas de voler fer servir heurística). Aquestes son wrapperXarxa (si has carregat una xarxa neuronal) i puntuacioPerAjacencia si vols ver servir la bàsica.  
- la funció de decisió que fa servir el Jugador 1/2. Les disponibles son triarMovimentJugador (si vols que el jugador trii), triarMovimentBot (si vols que trii la IA especificada anteriorment) i triarMovimentBotAleatori (si vols que sigui IA, però amb possibilitat de tirar aleatoriament).

A més es pot variar la profunditat de l'algorisme minmax amb la constant PROFUNDITAT de l'arxiu minmax.c (recomanat 7) y les dimensions/longitud de la ratlla del taulell a la funció inicialitzar partida (compte amb que si fas servir una xarxa neuronal aquesta sigui compatible amb les dimensions del taullel).


Per a compilar la partida, utilitza la seguent comanda: 

```
gcc -g -fopenmp -funroll-loops -flto -march=native Entrenament.c 4enratlla.c  minmax.c Xarxa.c Utilitats.c partides.c funcioUtilitat.c conexioXarxa4EnRatlla.c -O3 -o partida -lm
```

## Com s'utilitza

### 1. Entrenament de la Xarxa
Per iniciar el proces evolutiu, executa el programa compilat:

./entrenament

El programa:
1. Generara una poblacio inicial aleatoria.
2. Realitzara tornejos entre les xarxes.
3. Seleccionara les millors i creara noves generacions amb mutacions.
4. Desara els millors models (.DrusCNN) automaticament. (A cada iteració sobreescriu la anterior i desa una de J1 i una de J2).

### 2. Validacio i Joc
Per jugar contra la IA executa ./entrenament. Assegura't de que en compilar has introduït les opcions correctes.

## Detalls Tecnics de la IA

### Estructura de la CNN
* Entrada: Matriu del tauler (files x columnes).
* Capes: Sistema flexible definit per l'usuari (actualment configurat amb capes convolucionals de kernels 3x3 i 5x5).
* Funcio d'Activacio: Leaky ReLU.
* Sortida: Un unic valor (double) que representa l'avaluacio heuristica del tauler.

### Algorisme Genetic
* Metode de Seleccio: Torneig per parelles o lliga, jugen xarxes J1 vs J2.
* Reproduccio: Els guanyadors es clonen i els perdedors es substitueixen per versions mutades dels guanyadors.
* Mutacio: S'aplica soroll gaussia als biaixos i pesos dels kernels.

## Documentacio
Aquest projecte inclou documentacio generada automaticament. Per consultar-la obrint la carpeta html o el pdf.

## Autor
Drus Sentís Cahué.

Desenvolupat com a part d'un projecte d'implementacio d'IA en C pur.

## Llicencia
Codi obert.