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


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                  Implémentation du générateur de Geffe                                         |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
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


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                                Attaque par corrélation                                         |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
/*  1.2 Attaque par corrélation  

    Q3) Notons l1, l2, et l3 les tailles des états initiaux des trois LFSR que nous considérons 
        Ici on a l1 = 13, l2 = 11 et l3 = 9
        De plus, une recherche exhaustive pour déterminer l'état initial du générateur de Geffe
        revient à faire une recherche exhaustive sur les états initiaux des trois LFSR. Ainsi 
        la compléxité est 2^{l1+l2+l3} = 2^33   
        
    Q4) On rappelle que z(t) = s1(t)s2(t) + s2(t)s3(t) + s3(t) 
        Commençons par remarquer que :
            - z(t) = s3(t)  si s2(t) = 0 
            - z(t) = s1(t)  si s2(t) = 1
        Puis, d'autre part:
        P(z(t)=s1(t))   = P(z(t)=s1(t) inter s2(t)=1) + P(z(t)=s1(t) inter s2(t)=0)
                        = P(z(t)=s1(t) | s2(t)=1)*P(s2(t)=1) + P(z(t)=s1(t) | s2(t)=0)*P(s2(t)=0)
                        = P(s1(t)=s1(t))*(1/2) + P(s3(t)=s1(t))*(1/2)
                        = 1/2 + 1/4 = 3/4   
        Et on calcule de la même manière P(z(t)=s3(t))
        
    Q5) On a: p = P(z(t)=x)
                = P(x=0)*P(z(t)=x | x=0) + P(x=1)*P(z(t)=x | x=1)
                = (1/2)*P(z(t)=0) + (1/2)*P(z(t)=1)
                = (1/2)*(P(z(t)=0) + 1 - P(z(t)=0))
                = 1/2   
            
    Q6) D'après la dépendance qu'on a vu, z(t) est égale à s3(t) pour 3/4 des valeurs de t 
        (et pareil pour z(t) = s1(t)) ainsi on commence par faire une recherche exhaustive sur 
        l'état initial de s3. Ainsi pour les 2^l3 initialisations possibles, on compare les 
        valeurs de z(t) et s3(t) et si on a environ 3/4 des valeurs qui coincident alors on tient la
        bonne initialisation. Pareil pour s1. Et pour finir, une fois qu'on a s3 et s1 on peut faire
        une recherche exhaustive classique sur s2. Ce qui nous ramène à une compléxité de :
            2^l1 + 2^l2 + 2^l3 ~= 2^13
        qui est nettement mieux que 2^33 qu'on avait trouvé pour la recherche exhaustive classique
        
    Q7) Implémentons l'attaque précedente. Ici les résultats des états initiaux seront stockés
        dans L1, L2, et L3. On donnera à la fonction z, qui sera la suite des valeurs de z(t) 
        et l'entier b=borne sera la précision de l'attaque souhaité. Ie on considérera qu'on a la bonne 
        initialisation de s3 qui |C-3N/4| < 1/b 
        avec C le nombre de t où s3 et z coincident, et N est la longueur de la liste z fournie  */

int attaque_corr(liste* L1, liste* L2, liste* L3, liste* z, int borne){

}

int main(int argc, char *argv[]) {
    init__LFSR();
    test_implemGeffe(20);
    free__LFSR();
    return 0;
}