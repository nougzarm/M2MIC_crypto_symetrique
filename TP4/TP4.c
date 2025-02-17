#include "TP4.h"

/*  @nougzarm
    Crypto symétrique
    TP4 - Attaques contre le chiffrement à flot
    M2 MIC 2024-2025    
    
    Sommaire:
        (0. Structures)
          */

/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                              1. Fonctions maths                                                |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
unsigned int puissance(int x, unsigned int n){
    int resultat = 1;
    for(int i = 0; i < n; i++){
        resultat *= x;
    }
    return resultat;
}

/*  1. Attaques contre le chiffrement de Geffe

    Le chiffrement de Geffe est un chiffrement à flot par registres combinés, proposé par Geffe en 1973. Il
    est composé de trois LFSR de longueurs distinctes combinés par la fonction booléenne suivante :
        f(x1, x2, x3) = x1x2 + x2x3 + x3. 
    Les trois LFSR sont initialisés par des clés de longueurs   */

/*  1.1 Implémentation du générateur de Geffe  

    Q1) la valeur de z(t) en fonction de s1(t), s2(t) et s3(t) est donnée par la définition de f et est
            z(t) = s1(t)s2(t) + s2(t)s3(t) + s3(t) 
            
    Q2) Programmons une fonction geffe(S1,S2,S3,N) qui prend en entrée les trois états initiaux et un
        entier N, et retourne les N premiers bits de la suite  */



int main(int argc, char *argv[]) {
    return 0;
}