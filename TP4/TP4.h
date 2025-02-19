#ifndef TP3_H
#define TP3_H

#include <stdio.h>
#include <stdlib.h>


/*  @nougzarm
    Crypto symétrique
    M2 MIC 2024-2025    
    
    Sommaire:
        0. Structures
        1. Fonctions maths  */

/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                                  0. Structures                                                 |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
typedef struct{
    int* valeur;
    int taille;
}liste;

typedef struct{
    int* coeff;
    int degre;
}LFSR;


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                              1. Fonctions maths                                                |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Fonction puissance nécessaire pour divers calculs de tailles  */
unsigned int puissance(int x, unsigned int n);
int min(int a, int b);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                  Implémentation du générateur de Geffe                                         |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Fonction permettant d'initialiser les trois LFSR du sujet de TP (et leur libération) */
void init__LFSR();
void free__LFSR();

/*  Fonction permettant de produire la suite des sorties d'un LFSR à partir de l'état initial
    Remarque: ici N désigne le nombre de bits souhaités en sortie    */
int lectureLFSR(liste* result, LFSR* LFSR, liste* etat_initial, int N);

/*  Fonction du générateur de Geffe comme demandée dans le TP    */
int geffe(liste* result, liste* S1, liste* S2, liste* S3, int N);

/*  Fonctions permettant de tester l'implémentation précedente 
    - Fonction d'affichage de listes
    - Fonction d'initialisation des états initiaux comme dans le sujet du TP
    - Fonction principale de test d'implémentation */
void afficher_liste(liste* L);
void init__S(liste* S, int taille);
void test_implemGeffe(int N);



#endif // TP3_H