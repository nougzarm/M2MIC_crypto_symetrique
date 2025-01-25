#include "TP3.h"

int puissance(int x, int n){
    int resultat = 1;
    for(int i = 0; i < n; i++){
        resultat *= x;
    }
    return resultat;
}

unsigned short* lectureANF(unsigned int* n, FILE* nomFichier){
    unsigned int tailleTableau = puissance(2, *n);
    unsigned short* tableau = calloc(tailleTableau, sizeof(unsigned short));
    if(tableau == NULL){
        return NULL;
    }

    return tableau;
}

/*  Fonctions principale qui prend comme arguments :
        - Nom d'un fichier
        - Le degré souhaité */

int main(int argc, char *argv[]) {
    // Vérification du nombre d'arguments
    int nb_arg_attendu = 3;
    if (argc != nb_arg_attendu) {
        printf("Erreur : nombre d'arguments invalide\n");
        return 1;
    }
    // Ouverture du fichier
    FILE* fichier = fopen(argv[1], "r");
    if (fichier == NULL) {
        printf("Erreur : Ouverture impossible du fichier - (fichier introuvable)\n");
        return 2;
    }
    // Ouverture du fichier réussie
    int n = 0;
    int ch = fgetc(fichier);

    if (ch == EOF) {
        printf("Erreur : Fichier vide\n");
        return 3;
    }
    // Lecture de n
    while(ch >= '0' && ch <= '9'){
        n = n*10;
        n = n + (ch - '0');
        ch = fgetc(fichier);
    }

    printf("\nVoici la valeur lue de n : %d\n", n);
    return 0;
}