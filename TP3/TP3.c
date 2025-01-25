#include "TP3.h"

int puissance(int x, int n){
    int resultat = 1;
    for(int i = 0; i < n; i++){
        resultat *= x;
    }
    return resultat;
}

unsigned int lectureNombre(FILE* fichier){
    unsigned int nombre = 0;
    int ch = fgetc(fichier);
    while(ch >= '0' && ch <= '9'){
        nombre = nombre*10;
        nombre = nombre + (ch - '0');
        ch = fgetc(fichier);
    }
    fseek(fichier, -1, SEEK_CUR);
    return nombre;
}

unsigned int lectureMonome(FILE* fichier, char nomVariable){
    int ch = fgetc(fichier);
    if (ch == '1') {
        return 0;
    }
    unsigned int monome = 0;
    while(ch == nomVariable){
        ch = lectureNombre(fichier);
        monome = monome + puissance(2, ch - 1);
        ch = fgetc(fichier);
    }
    fseek(fichier, -1, SEEK_CUR);
    return monome;
}

unsigned short* lectureANF(unsigned int* n, FILE* fichier){
    unsigned int tailleTableau = puissance(2, *n);
    unsigned short* tableau = calloc(tailleTableau, sizeof(unsigned short));
    if(tableau == NULL){
        return NULL;
    }

    // Ignorer la première ligne qui contient le nombre de variables n
    char bufferCorbeille[256];
    fgets(bufferCorbeille, 256, fichier);

    // Lecture du symbole utilisé pour la variable (x ou t p.ex)
    int nomVariable = fgetc(fichier);
    fseek(fichier, -1, SEEK_CUR);

    int t = 0;
    int proCh;
    int monomeActuel;
    while(t == 0){
        monomeActuel = lectureMonome(fichier, nomVariable);
        printf("Monome actuel : %d\n", monomeActuel);
        tableau[monomeActuel] = 1;
        fseek(fichier, 3, SEEK_CUR);
        proCh = fgetc(fichier);
        if(proCh != nomVariable && proCh != '1'){
            t = 1;
        }
        else{
            fseek(fichier, -1, SEEK_CUR);
        }
    }
    return tableau;
}

void affichageANF(unsigned short* tableau, unsigned int n){
    for (int i = 0; i < puissance(2, n); i++) {
        if (tableau[i] == 0) {
            continue;
        }
        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) {
                printf("x%d", j + 1);
            }
        }
        printf(" = %d\n", tableau[i]);
    }
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

    // Lecture de n (le nombre de variables de notre fonction ANF)
    int n = lectureNombre(fichier);
    printf("\nNombre de variables de l'ANF : n = %d\n", n);

    // Lecture de l'ANF
    unsigned short* tableau = lectureANF(&n, fichier);
    if(tableau == NULL){
        printf("Erreur : Lecture de l'ANF impossible (revoir fichier contenant ANF)\n");
        return 4;
    }

    // Affichage de l'ANF
    /* affichageANF(tableau, n); */

    

    fclose(fichier);
    free(tableau);
    return 0;
}