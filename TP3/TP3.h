#ifndef TP3_H
#define TP3_H

#include <stdio.h>
#include <stdlib.h>


/*  @nougzarm
    Crypto symétrique
    M2 MIC 2024-2025    
    
    Sommaire:
        0. Structures
        1. Fonctions maths
        2. Initialisation de ANF
        3. Affichage de ANF(/monome)
        4. Lecture d'ANF à partir d'un fichier
        5. Support d'une ANF
        6. (Initialisation/définition/gestion de) bases de monomes
        7. Initialisation/gestion de matrices
        8. Annihilateurs de ANF
        (9. Résolution - fonction principal)  */

/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                                  0. Structures                                                 |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Structure de ANF : permet de représenter une fonctions booléenne f  sous sa forme algébrique normale    
    Remarque: la liste ANF.monome est de taille 2^n où n = ANF.nbVar, et elle est définie comme dans TP3.pdf   */
typedef struct{
    unsigned short* monome;
    unsigned int nbVar;
    unsigned long* support;
    unsigned long poids;
}ANF;

typedef struct{
    unsigned int nbVar;
    unsigned int degMax;
    unsigned int nbMonomes;
    unsigned long* monome;
}baseMonomes;

typedef struct{
    unsigned long** coefficient;
    unsigned int nbLignes;
    unsigned int nbColonnes;
}matrice;


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                              1. Fonctions maths                                                |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Fonction puissance nécessaire pour divers calculs de tailles  */
unsigned int puissance(int x, unsigned int n);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                           2. Initialisation de ANF                                             |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
 */
/*  Fonction permettant d'initialiser un ANF f (de n variables) 0  */
int init_ANF(ANF* f, unsigned int n);

/*  Fonction permettant d'initialiser dans g une copie de f (sans copier le support et le poids)  */
int init_ANF_copie(ANF* g, ANF* f);

/*  Permet la libération de la mémoire occupée par une ANF   */
int vider_ANF(ANF* f);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                          3. Affichage de ANF(/monome)                                          |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
 */
/*  Fonction d'affichage de monome à partir de sa décompo en base 2 (en n variables)  */
int afficher_monome(int monome, unsigned int n);

/*  Fonction permettant d'afficher une ANF f comme somme de ses monomes  */
int afficherANF(ANF* f);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                   4. Lecture d'ANF à partir d'un fichier                                       |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
 */
/*  Fonction permettant de lire un nombre
    Remarque: Ici le curseur est supposé bien positionné sur le début d'un nombre */
unsigned int lectureNombre(FILE* fichier);

/*  Fonction permettant de lire un monome
    Remarque: Ici le curseur est supposé bien positionné sur le début d'un monome */
unsigned int lectureMonome(FILE* fichier, char nomVariable);

/*  Fonction permettant de lire une ANF et de stocker dans f (qui est deja initialisée):
    Remarque: Ici le curseur est supposé bien positionné sur le début d'une ANF
    P.ex: dans notre cas il s'agit du début de la 2e ligne de notre fichier */
int lectureANF(ANF* f, FILE* fichier);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                              5. Support d'une ANF                                              |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Fonction de test de supériorité selon l'ordre utilisée dans le calcul de ANF
    Plus précisément, a est supérieur à b si pour chaque bit, celui de a est supérieur ou égale à celui de b
    Remarque: ici n désigne la borne de taille  */
int supTest(int a, int b, int n);

/*  Fonction permettant d'évaluer une ANF f en un point x  */
int imageANF(ANF* f, unsigned int x);

/*  Fonction qui calcule le support de f ainsi que son poids */
int supportANF(ANF* f);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                         6. (Initialisation/définition/gestion de) bases de monomes                             |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
unsigned int binomial(unsigned int n, unsigned int k);

int degreMonome(unsigned int m, unsigned int n);

/*  Permet d'initialiser la base B des monomes de degré au plus de d et de n variables */
int init_baseMonomes(baseMonomes* B, unsigned int n, unsigned int d);

/*  Libération de la mémoire occupée par une base de monomes */
int vider_baseMonomes(baseMonomes* B);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                    7. Initialisation/gestion de matrices                                       |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Permet d'initialiser la matrice M à 0    */
int init_matrice(matrice* M, unsigned int nbLignes, unsigned int nbColonnes);

/*  Permet la libération de la mémoire occupée par une matrice   */
int vider_matrice(matrice* M);

/*  Affiche une matrice M     */
void afficher_matrice(matrice* M);

/*  Stocke la matrice de f (évaluations des monomes de B sur le support de f) */
int matriceANF(matrice* M, ANF* f, baseMonomes* B);

/*  Opérations sur les matrices */
void ajoutLignes(matrice* M, int l1, int l2);
void echangeLignes(matrice* M, int l1, int l2);
int nulliteLigne(matrice* M, int l, int colonneDepart);
void echangeColonnes(matrice* M, int c1, int c2);

/*  Pivot de Gauss, qui enregistre les changements de lignes dans T, qui à l'initialisation
    est la matrice identité/
    Remarque : T doit être une matrice carré nulle dont le nombre de lignes coincide avec celui de M */
int elimGauss(matrice* T, matrice* M, int n, int* nbLNulles);


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                           8. Annihilateurs de ANF                                              |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  Convertir un vecteur exprimant une fonction dans la base B en une ANF (extraction de la base de AN(f))     */
int vect_vers_ANF(ANF* h, unsigned long* vect, baseMonomes* B);

#endif // TP3_H