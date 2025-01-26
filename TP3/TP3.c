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

int supTest(int a, int b, int n){
    int t = 1;
    for (int i = 0; i < n; i++) {
        if ((a & (1 << i)) < (b & (1 << i))) {
            t = 0;
            break;
        }
    }
    return t;
}

int valeurANF(unsigned short* tableau, unsigned int n, unsigned int x){
    int resultat = 0;
    for (int i = 0; i < puissance(2, n); i++) {
        if (tableau[i] == 0) {
            continue;
        }
        if (supTest(x, i, n)) {
            resultat = resultat ^ tableau[i];   // Equivaut à ajout puis réduction modulo 2
        }
    }
    return resultat;
}

unsigned long* vecteurValeurs(unsigned short* tableau, unsigned int n){
    unsigned long* vecteur = calloc(puissance(2, n) + 1, sizeof(unsigned long));
    long poidsHamming = 0;
    if(vecteur == NULL){
        return NULL;
    }
    int imageActuelle;
    int curseur = 1;
    for (int i = 0; i < puissance(2, n); i++) {
        imageActuelle = valeurANF(tableau, n, i);
        if (imageActuelle == 1) {
            vecteur[curseur] = i;
            curseur += 1;
            poidsHamming += 1;
        }
    }
    vecteur = realloc(vecteur, (poidsHamming + 1) * sizeof(unsigned long));
    vecteur[0] = poidsHamming;
    return vecteur;
}

unsigned int binomial(unsigned int n, unsigned int k){
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;

    // Calcul via la formule factorielle
    unsigned int resultat = 1;
    for (unsigned int i = 1; i <= k; ++i) {
        resultat *= n--;
        resultat /= i;
    }
    return resultat;
}

int degreMonome(unsigned int m, unsigned int n){
    unsigned int deg = 0;
    int t;
    for(unsigned int i = 0; i < n; i++){
        t = (m >> i) & 1;
        if(t == 1){
            deg++;
        }
    }
    return deg;
}

unsigned long* tableauMonomes(unsigned int n, unsigned int d){
    unsigned int tailleTableau = 0;
    for (unsigned int i = 0; i <= d; i++){
        tailleTableau = tailleTableau + binomial(n, i);
    }
    unsigned long* tableau = (unsigned long*)malloc((tailleTableau+1) * sizeof(unsigned long));
    unsigned int indice = 0;
    unsigned int nbTotal = puissance(2, n);
    for (unsigned long i = 0; i <= nbTotal; i++){
        if(degreMonome(i, n) <= d){
            tableau[indice] = i;
            indice++;
        }
    }
    return tableau;
}

unsigned long** constructMatrice(unsigned long* vecteurValeurs, int n, unsigned int nbMonomes, unsigned long* tableauMonomesC){
    int poids = (int)vecteurValeurs[0];
    unsigned long** matrice = (unsigned long**)malloc(nbMonomes*sizeof(unsigned long*));
    for (unsigned long i = 0; i < nbMonomes; i++){
        matrice[i] = (unsigned long*)malloc((poids) * sizeof(unsigned long));
        for (int j = 1; j <= poids; j++){
            if(supTest(vecteurValeurs[j], tableauMonomesC[i], n) == 1){
                matrice[i][j-1] = 1;
            }
            else{
                matrice[i][j-1] = 0;
            }
        }
    }
    return matrice;
}

void effacerMatrice(unsigned long** matrice, unsigned int nbMonomes){
    for(unsigned int i = 0; i < nbMonomes; i++){
        free(matrice[i]);
    }
    free(matrice);
}

void afficherMatrice(unsigned long** matrice, int nbLignes, int nbColonnes){
    printf("\n");
    for(int i = 0; i < nbLignes; i++){
        printf("[");
        for (int j = 0; j < nbColonnes; j++){
            printf(" %ld ", matrice[i][j]);
        }
        printf("]\n");
    }
}

void ajoutLignes(unsigned long** matrice, int l1, int l2, int nbLignes, int nbColonnes){
    for (int j = 0; j < nbColonnes; j++){
        matrice[l1][j] = matrice[l1][j] ^ matrice[l2][j] ;
    }
    return;
}

void echangeColonnes(unsigned long** matrice, int c1, int c2, int nbLignes, int nbColonnes){
    int t;
    for (int i = 0; i < nbLignes; i++){
        t = matrice[i][c1]; 
        matrice[i][c1] = matrice[i][c2];
        matrice[i][c2] = t;
    }
    return;
}

unsigned long** elimGauss(unsigned long** matrice, unsigned int nbLignes, int nbColonnes, int n){
    // Initialisation de la matrice identité
    unsigned long** T = (unsigned long**)calloc(nbLignes, sizeof(unsigned long*));
    for (int i = 0; i < nbLignes; i++){
        T[i] = (unsigned long*)calloc(nbLignes, sizeof(unsigned long));
        T[i][i] = 1;
    }

    int examined_rows = 0;
    int j;
    for(int i = 0; i < nbLignes-1; i++){
        j = -1;
        for (int j_prime = examined_rows; j < nbColonnes; j++){
            if(matrice[i][j_prime] != 0){
                j = j_prime;
                break;
            }
        }
        if(j == -1){
            // la ligne i est nulle (stocker cette info dans une liste...)
        }
        else{
            if(i != j){
                echangeColonnes(matrice, i, j, nbLignes, nbColonnes);
            }
            for (int l = i+1; l < nbLignes; l++){
                if(matrice[l][i] != 0){
                    ajoutLignes(matrice, l, i, nbLignes, nbColonnes);
                    ajoutLignes(T, l, i, nbLignes, nbLignes);
                }
            }
            examined_rows++;
        }
    }
    return T;
}



/*  Fonctions principale qui prend comme arguments :
        - Nom d'un fichier
        - Le degré souhaité */
int main(int argc, char *argv[]) {
    /* Vérification du nombre d'arguments */
    int nb_arg_attendu = 3;
    if (argc != nb_arg_attendu) {
        printf("Erreur : nombre d'arguments invalide\n");
        return 1;
    }

    /* Ouverture du fichier */
    FILE* fichier = fopen(argv[1], "r");
    if (fichier == NULL) {
        printf("Erreur : Ouverture impossible du fichier - (fichier introuvable)\n");
        return 2;
    }

    /* Lecture de n (le nombre de variables de notre fonction ANF) */
    int n = lectureNombre(fichier);
    printf("\nNombre de variables de l'ANF : n = %d\n", n);

    /* Lecture de l'ANF */
    unsigned short* tableau = lectureANF(&n, fichier);
    if(tableau == NULL){
        printf("Erreur : Lecture de l'ANF impossible (revoir fichier contenant ANF)\n");
        return 4;
    }

    /* Calcul du vecteur de valeurs (et le poids de Hamming qui se situe en début de liste) */
    unsigned long* vectValeurs = vecteurValeurs(tableau, n);
    int poids = (int)vectValeurs[0];

    // Affichage de l'ANF
    /* affichageANF(tableau, n); */

    /* Affichage du vecteur de valeurs (support de f, ainsi que son poids) */
    printf("\nPoids de Hamming de f = %d. \nVecteur de valeurs de l'ANF (support de f) : [", (int)vectValeurs[0]);
    for (int i = 1; i <= vectValeurs[0]; i++) {
        printf(" %ld ", vectValeurs[i]);
    }
    printf("]\n");

    /* Calcul de la matrice des monomes de degré au plus d  */
    unsigned int d = atoi(argv[2]);
    unsigned int nbMonomes = 0;
    for (unsigned int i = 0; i <= d; i++){
        nbMonomes = nbMonomes + binomial(n, i);
    }
    unsigned long* tableauMonomesC = tableauMonomes(n, d);

    /* Affichage des monomes de degré au plus d */
    printf("\nOn a d = %d et nb de monome de degre au plus d : %d\nLes voici : [", d, nbMonomes);
    for (int i = 0; i < nbMonomes; i++){
        printf(" %ld ", tableauMonomesC[i]);
    }
    printf("]\n");
    

    unsigned long** matrice = constructMatrice(vectValeurs, n, nbMonomes, tableauMonomesC);
    
    /* Affichage de la matrice de départ (avant pivot de Gauss) */
    afficherMatrice(matrice, nbMonomes, poids);

    unsigned long** T = elimGauss(matrice, nbMonomes, poids, n);

    afficherMatrice(matrice, nbMonomes, poids);
    afficherMatrice(T, nbMonomes, nbMonomes);

    /* Libération de la mémoire */
    fclose(fichier);
    effacerMatrice(matrice, nbMonomes);
    free(tableau);
    free(vectValeurs);
    free(tableauMonomesC);
    return 0;
}

/*  Modifier la fonction de construction de la première matrice, pour enlever la description des monomes en débuts de lignes
    puis continuer pivot de gauss */