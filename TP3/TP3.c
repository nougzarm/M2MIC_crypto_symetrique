#include "TP3.h"

unsigned int puissance(int x, unsigned int n){
    int resultat = 1;
    for(int i = 0; i < n; i++){
        resultat *= x;
    }
    return resultat;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                            Initialisation de ANF                                               |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
 */
int init_ANF(ANF* f, unsigned int n){
    f->nbVar = n;
    f->monome = (unsigned short*)calloc(puissance(2, f->nbVar), sizeof(unsigned short));
    f->support = NULL;
    f->poids = -1;
    return 0;
}

int init_ANF_copie(ANF* g, ANF* f){
    g->nbVar = f->nbVar;
    unsigned int borne = puissance(2, g->nbVar);
    g->monome = (unsigned short*)calloc(puissance(2, borne), sizeof(unsigned short));
    for (int i = 0; i < borne; i++){
        g->monome[i] = f->monome[i];
    }
    g->support = NULL;
    g->poids = -1;
    return 0;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                           Affichage de ANF (et monome)                                         |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
 */
int afficher_monome(int monome, unsigned int n){
    if(monome == 0){
        printf("1");
    }
    for(int i = 0; i < n; i++){
        if((monome >> i) & 1 == 1){
            printf("x%d", i+1);
        }
    }
    return 0;
}

int afficherANF(ANF* f){
    unsigned int nbMax = puissance(2, f->nbVar);
    for (int i = 0; i < nbMax; i++) {
        if (f->monome[i] != 0) {
            afficher_monome(i, f->nbVar);
            printf(" + ");
        }
    }
    return 0;
}

int vider_ANF(ANF* f){
    if(f->monome != NULL){
        free(f->monome);
    }
    if(f->support != NULL){
        free(f->support);
    }
    return 0;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                    Lecture de ANF à partir d'un fichier                                        |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
 */
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

int lectureANF(ANF* f, FILE* fichier){
    unsigned int tailleTableau = puissance(2, f->nbVar);

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
        f->monome[monomeActuel] = 1;
        fseek(fichier, 3, SEEK_CUR);
        proCh = fgetc(fichier);
        if(proCh != nomVariable && proCh != '1'){
            t = 1;
        }
        else{
            fseek(fichier, -1, SEEK_CUR);
        }
    }
    return 0;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                               Support d'une ANF                                                |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
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

int imageANF(ANF* f, unsigned int x){
    int resultat = 0;
    unsigned int n = f->nbVar;
    unsigned int nbMax = puissance(2, n);
    for (int i = 0; i < nbMax; i++) {
        if (f->monome[i] == 0) {
            continue;
        }
        if (supTest(x, i, n)) {
            resultat = resultat ^ f->monome[i];   // Equivaut à ajout puis réduction modulo 2
        }
    }
    return resultat;
}

int supportANF(ANF* f){
    unsigned int n = f->nbVar;
    unsigned int nbMax = puissance(2, n);
    f->support = calloc(nbMax + 1, sizeof(unsigned long));
    long poidsHamming = 0;
    if(f->support == NULL){
        1;
    }
    int imageActuelle;
    int curseur = 0;
    for (int i = 0; i < nbMax; i++) {
        imageActuelle = imageANF(f, i);
        if (imageActuelle == 1) {
            f->support[curseur] = i;
            curseur += 1;
            poidsHamming += 1;
        }
    }
    f->support = realloc(f->support, (poidsHamming + 1) * sizeof(unsigned long));
    f->poids = poidsHamming;
    return 0;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                           Initialisation/définition/gestion de bases de monome                                 |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
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

int init_baseMonomes(baseMonomes* B, unsigned int n, unsigned int d){
    unsigned int tailleTableau = 0;
    for (unsigned int i = 0; i <= d; i++){
        tailleTableau = tailleTableau + binomial(n, i);
    }
    B->degMax = d;
    B->nbVar = n;
    B->nbMonomes = tailleTableau;
    B->monome = (unsigned long*)malloc((tailleTableau+1) * sizeof(unsigned long));

    unsigned int indice = 0;
    unsigned int nbTotal = puissance(2, n);
    for (unsigned long i = 0; i <= nbTotal; i++){
        if(degreMonome(i, n) <= d){
            B->monome[indice] = i;
            indice++;
        }
    }
    return 0;
}

int vider_baseMonomes(baseMonomes* B){
    if(B->monome != NULL){
        free(B->monome);
    }
    return 0;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                     Initialisation/gestion de matrices                                         |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
int init_matrice(matrice* M, unsigned int nbLignes, unsigned int nbColonnes){
    M->nbLignes = nbLignes;
    M->nbColonnes = nbColonnes;
    M->coefficient = (unsigned long**)malloc(nbLignes*sizeof(unsigned long*));
    for (unsigned int i = 0; i < nbLignes; i++){
        M->coefficient[i] = (unsigned long*)malloc((nbColonnes) * sizeof(unsigned long));
    }
    return 0;
}

int vider_matrice(matrice* M){
    for(unsigned int i = 0; i < M->nbLignes; i++){
        free(M->coefficient[i]);
    }
    free(M->coefficient);
}

void afficher_matrice(matrice* M){
    printf("\n");
    for(int i = 0; i < M->nbLignes; i++){
        printf("[");
        for (int j = 0; j < M->nbColonnes; j++){
            printf(" %ld ", M->coefficient[i][j]);
        }
        printf("]\n");
    }
}

int matriceANF(matrice* M, ANF* f, baseMonomes* B){
    unsigned int n = f->nbVar;
    for (unsigned int i = 0; i < M->nbLignes; i++){
        for (int j = 0; j < M->nbColonnes; j++){
            if(supTest(f->support[j], B->monome[i], n) == 1){
                M->coefficient[i][j] = 1;
            }
            else{
                M->coefficient[i][j] = 0;
            }
        }
    }
    return 0;
}

void ajoutLignes(matrice* M, int l1, int l2){
    for (int j = 0; j < M->nbColonnes; j++){
        M->coefficient[l1][j] = M->coefficient[l1][j] ^ M->coefficient[l2][j] ;
    }
    return;
}

void echangeLignes(matrice* M, int l1, int l2){
    int t;
    for (int j = 0; j < M->nbColonnes; j++){
        t = M->coefficient[l1][j];
        M->coefficient[l1][j] = M->coefficient[l2][j];
        M->coefficient[l2][j] = t;
    }
    return;
}

int nulliteLigne(matrice* M, int l, int colonneDepart){
    for(int j = colonneDepart; j < M->nbColonnes; j++){
        if (M->coefficient[l][j] == 1){
            return j;
        }
    }
    return -1;
}

void echangeColonnes(matrice* M, int c1, int c2){
    int t;
    for (int i = 0; i < M->nbLignes; i++){
        t = M->coefficient[i][c1]; 
        M->coefficient[i][c1] = M->coefficient[i][c2];
        M->coefficient[i][c2] = t;
    }
    return;
}

int elimGauss(matrice* T, matrice* M, int n, int* nbLNulles){
    printf("\nPivot de Gauss : nbLignes = %d, nbColonnes = %d", M->nbLignes, M->nbColonnes);
    // Initialisation de la matrice identité
    for (int i = 0; i < T->nbLignes; i++){
        T->coefficient[i][i] = 1;
    }

    int nbLignesNulles = 0;
    int examined_rows = 0;
    int j;

    for(int i = 0; i < M->nbLignes-1; i++){
        j = nulliteLigne(M, i, examined_rows);
        printf("\n j=%d", j);
        while(j == -1 && i <= M->nbLignes-1-nbLignesNulles){
            printf("\n ligne nulle : i=%d", i);
            echangeLignes(M, i, M->nbLignes-1-nbLignesNulles);
            echangeLignes(T, i, M->nbLignes-1-nbLignesNulles);
            nbLignesNulles++;
            j = nulliteLigne(M, i, examined_rows);
        }
        if (j != -1 && i < M->nbColonnes){
            if(i != j){
                printf("\non fait l'échange de colonnes pour i = %d, j = %d", i, j);
                echangeColonnes(M, i, j);
            }
            for (int l = i+1; l < nbLignes; l++){
                if(M->coefficient[l][i] != 0){
                    printf("\non fait l'échange de lignes pour i = %d, l = %d", i, l);
                    ajoutLignes(M, l, i);
                    ajoutLignes(T, l, i);
                }
            }
            examined_rows++;
        }
    }
    *nbLNulles = nbLignesNulles;
    return 0;
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

    /* Initialisation des ANF f et g = f+1 */
    ANF f;
    ANF g;

    /* Lecture de l'ANF f à partir du fichier    */
    init_ANF(&f, n);
    lectureANF(&f, fichier);

    /* Définition de g = f + 1  */
    init_ANF_copie(&g, &f);
    g.monome[0] = g.monome[0] ^ 1;

    /*  Affichage de f et g  */
    printf("f et g sous leurs formes ANF:\n - f = "); afficherANF(&f);
    printf("\n - g = "); afficherANF(&g);

    /* Calcul des support de f et g (et leurs poids) */
    supportANF(&f);
    supportANF(&g);
    int poids_f = f.poids;
    int poids_g = g.poids;

    /* Affichage du vecteur de valeurs (support de f, ainsi que son poids) */
    /* printf("\nPoids de Hamming de f = %d. \nVecteur de valeurs de l'ANF (support de f) : [", poids_f);
    for (int i = 0; i < poids_f; i++) {
        printf(" %ld ", f.support[i]);
    }
    printf("]\n"); */

    /* Calcul de la base des monomes de degré au plus d  */
    unsigned int d = atoi(argv[2]);
    baseMonomes B;
    init_baseMonomes(&B, n, d);
    unsigned int nbMonomes = B.nbMonomes;

    /* Affichage de la base B */
    
    /* Matrice de f (resp. g) dont le noyau est l'annihilateur de f (resp. g) */
    unsigned long** matrice_f = matriceANF(&f, &B);
    unsigned long** matrice_g = matriceANF(&g, &B);
    
    /* Affichage de la matrice de départ (avant pivot de Gauss) */
    afficherMatrice(matrice_f, nbMonomes, poids_f);

    int dimANf;
    int dimANg;

    unsigned long** T_f = elimGauss(matrice_f, nbMonomes, poids_f, n, &dimANf);
    unsigned long** T_g = elimGauss(matrice_g, nbMonomes, poids_g, n, &dimANg);

    printf("\n Dim IN(f) = %d", dimANf);
    printf("\n Dim IN(f) = %d", dimANg);

    afficherMatrice(matrice_f, nbMonomes, poids_f);
    afficherMatrice(T_f, nbMonomes, nbMonomes);

    afficher_monome(6, n);

    /* Libération de la mémoire */
    fclose(fichier);
    vider_ANF(&f);
    vider_ANF(&g);
    vider_baseMonomes(&B);
    effacerMatrice(matrice_f, nbMonomes);
    return 0;
}

/*  Modifier la fonction de construction de la première matrice, pour enlever la description des monomes en débuts de lignes
    puis continuer pivot de gauss */