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
void affichageANF(unsigned short* tableau, unsigned int n)

#endif // TP3_H