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

int min(int a, int b){
    if(a < b){
        return a;
    }
    return b;
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
        entier N, et retourne les N premiers bits de la suite. Commençons par définir les trois LFSR
        que nous utilisons dans le sujet du TP  */

LFSR LFSR1, LFSR2, LFSR3;

/*  Fonction permettant d'initialiser les LFSR via leur polynomes Pi du TP */
void init__LFSR(){
    LFSR1.degre = 13;
    LFSR1.coeff = calloc(LFSR1.degre + 1, sizeof(int));
    LFSR1.coeff[0] = 1;
    LFSR1.coeff[3] = 1;
    LFSR1.coeff[4] = 1;
    LFSR1.coeff[13] = 1;

    LFSR2.degre = 11;
    LFSR2.coeff = calloc(LFSR2.degre + 1, sizeof(int));
    LFSR2.coeff[0] = 1;
    LFSR2.coeff[2] = 1;
    LFSR2.coeff[11] = 1;

    LFSR3.degre = 9;
    LFSR3.coeff = calloc(LFSR3.degre + 1, sizeof(int));
    LFSR3.coeff[0] = 1;
    LFSR3.coeff[4] = 1;
    LFSR3.coeff[9] = 1;
}

/*  Sans oublier la libération de mémoire en fin de programme */
void free__LFSR(){
    free(LFSR1.coeff);
    free(LFSR2.coeff);
    free(LFSR3.coeff);
}

/*  Première fonction qui retourne la liste des N premieres sorties d'un LFSR (en comptant l'état initial)
    Remarque: L'état initial doit contenir d éléments où d est le degré du polynome de rétroaction du LFSR  */
int lectureLFSR(liste* result, LFSR* LFSR, liste* etat_initial, int N){
    result->taille = N;
    result->valeur = calloc(N, sizeof(int));
    for(int i = 0; i < min(etat_initial->taille, N); i++){
        result->valeur[i] = etat_initial->valeur[i];
    }
    if(N > etat_initial->taille){
        for(int i = etat_initial->taille; i < N; i++){
            int somme = 0;
            for(int j = 0; j < LFSR->degre; j++){
                somme += result->valeur[i-j-1] * LFSR->coeff[j+1];
            }
            result->valeur[i] = somme % 2;
        }
    }
    return 0;
}

/*  Fonction geffe(S1, S2, S3, N) qui retourne la liste des N premiers bits de la suite de Geffe */
int geffe(liste* result, liste* S1, liste* S2, liste* S3, int N){
    /*  Calcul de la suite des sorties des LFSR */
    liste x1, x2, x3;
    lectureLFSR(&x1, &LFSR1, S1, N);
    lectureLFSR(&x2, &LFSR2, S2, N);
    lectureLFSR(&x3, &LFSR3, S3, N);
    /*  Calcul de la suite z(t) (définie comme dans la qst 1) */
    result->taille = N;
    result->valeur = calloc(N, sizeof(int));
    for(int i = 0; i < N; i++){
        result->valeur[i] = x1.valeur[i] * x2.valeur[i] + x2.valeur[i] * x3.valeur[i] + x3.valeur[i];
        result->valeur[i] %= 2;
    }
    /*  Libération de la mémoire occupée par les suites x  */
    free(x1.valeur);
    free(x2.valeur);
    free(x3.valeur);
    return 0;
}

/*  Pour finir cette partie, une dernière fonction permettant de tester l'implémentation 
    précedente (voir exemple du TP) */
void afficher_liste(liste* L){
    printf("[");
    for(int i = 0; i < L->taille; i++){
        printf("%d ", L->valeur[i]);
    }
    printf("]");
    printf("\n");
}

/*  Permet d'initialiser la liste S en mettant des 1 aux indices pairs et 0 ailleurs */
void init__S(liste* S, int taille){
    S->taille = taille;
    S->valeur = calloc(taille, sizeof(int));
    for(int i = 0; i < taille; i = i + 2){
        S->valeur[i] = 1;
    }
}

void test_implemGeffe(int N){
    /*  Suites d'états initiaux */
    liste S1, S2, S3, z;
    init__S(&S1, 13);
    init__S(&S2, 11);
    init__S(&S3, 9);
    /*  La suite résultat */
    geffe(&z, &S1, &S2, &S3, N);
    /*  Affichage des résultats */
    printf("Voici un test de l'implémentation de Geffe pour N = %d et les états initiaux suivants\n", N);
    printf("S1 = "); afficher_liste(&S1);
    printf("S2 = "); afficher_liste(&S2);
    printf("S3 = "); afficher_liste(&S3);
    printf("On obtient la suite z (s) suivante :\n");
    afficher_liste(&z);
    /*  Libération de la mémoire  */
    free(S1.valeur);
    free(S2.valeur);
    free(S3.valeur);
    free(z.valeur);
}

int main(int argc, char *argv[]) {
    init__LFSR();
    test_implemGeffe(20);
    free__LFSR();
    return 0;
}