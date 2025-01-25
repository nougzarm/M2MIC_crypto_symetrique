#ifndef TP3_H
#define TP3_H

#include <stdio.h>
#include <stdlib.h>


/*  Fonction puissance nécessaire pour divers calculs de tailles  */
int puissance(int x, int n);

/*  Fonction permettant de lire un nombre
    Remarque: Ici le curseur est supposé bien positionné sur le début d'un nombre */
unsigned int lectureNombre(FILE* fichier);

/*  Fonction permettant de lire un monome
    Remarque: Ici le curseur est supposé bien positionné sur le début d'un monome */
unsigned int lectureMonome(FILE* fichier, char nomVariable);

/*  Fonction permettant de lire une ANF:
    Remarque: Ici le curseur est supposé bien positionné sur le début d'une ANF
    P.ex: dans notre cas il s'agit du début de la 2e ligne de notre fichier */
unsigned short* lectureANF(unsigned int* n, FILE* fichier);

/*  Fonction permettant d'afficher une ANF (fonction test)  */
void affichageANF(unsigned short* tableau, unsigned int n);

/*  Fonction de test de supériorité selon l'ordre utilisée dans le calcul de ANF
    Plus précisément, a est supérieur à b si pour chaque bit, celui de a est supérieur ou égale à celui de b
    Remarque: ici n désigne la borne de taille  */
int supTest(int a, int b, int n);

/*  Fonction permettant de calculer la valeur de l'ANF pour une valeur un vecteur x donné  */
int valeurANF(unsigned short* tableau, unsigned int n, unsigned int x);

/*  Fonction qui retourne un tableau contenant les vecteurs de valeurs de l'ANF 
    (ainsi que le poids de Hamming qui est stocké en début de liste) */
unsigned long* vecteurValeurs(unsigned short* tableau, unsigned int n);

#endif // TP3_H