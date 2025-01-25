#include "TP3.h"

/*  Fonctions principale qui prend comme arguments :
        - Nom d'un fichier
        - Le degré souhaité */

int main(int argc, char *argv[]) {
    // Vérification du nombre d'arguments
    int nb_arg_attendu = 2;
    if (argc != nb_arg_attendu) {
        printf("Erreur : nombre d'arguments invalide\n");
        return 1;
    }
    // Ouverture du fichier
    File* fichier = fopen(argv[1], "r");
    if (fichier == NULL) {
        printf("Erreur : Ouverture impossible du fichier - (fichier introuvable)\n");
        return 2;
    }
    
    return 0;
}