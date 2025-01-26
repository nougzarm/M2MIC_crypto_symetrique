#include "TP3.h"

/*  @nougzarm
    Crypto symétrique
    M2 MIC 2024-2025    
    
    Sommaire:
        (0. Structures)
        1. Fonctions maths
        2. Initialisation de ANF
        3. Affichage de ANF(/monome)
        4. Lecture d'ANF à partir d'un fichier
        5. Support d'une ANF
        6. (Initialisation/définition/gestion de) bases de monomes
        7. Initialisation/gestion de matrices
        8. Annihilateurs de ANF
        9. Résolution - fonction principal  */

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


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                           2. Initialisation de ANF                                             |
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
    |                                          3. Affichage de ANF(/monome)                                          |
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
    |                                   4. Lecture d'ANF à partir d'un fichier                                       |
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
    |                                              5. Support d'une ANF                                              |
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
    |                         6. (Initialisation/définition/gestion de) bases de monomes                             |
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
    |                                    7. Initialisation/gestion de matrices                                       |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
int init_matrice(matrice* M, unsigned int nbLignes, unsigned int nbColonnes){
    M->nbLignes = nbLignes;
    M->nbColonnes = nbColonnes;
    M->coefficient = (unsigned long**)calloc(nbLignes, sizeof(unsigned long*));
    for (unsigned int i = 0; i < nbLignes; i++){
        M->coefficient[i] = (unsigned long*)calloc(nbColonnes, sizeof(unsigned long));
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
    // Initialisation de la matrice identité
    for (int i = 0; i < T->nbLignes; i++){
        T->coefficient[i][i] = 1;
    }

    int nbLignesNulles = 0;
    int examined_rows = 0;
    int j;

    for(int i = 0; i < M->nbLignes-1; i++){
        j = nulliteLigne(M, i, examined_rows);
        while(j == -1 && i <= M->nbLignes-1-nbLignesNulles){
            echangeLignes(M, i, M->nbLignes-1-nbLignesNulles);
            echangeLignes(T, i, M->nbLignes-1-nbLignesNulles);
            nbLignesNulles++;
            j = nulliteLigne(M, i, examined_rows);
        }
        if (j != -1 && i < M->nbColonnes){
            if(i != j){
                echangeColonnes(M, i, j);
            }
            for (int l = i+1; l < M->nbLignes; l++){
                if(M->coefficient[l][i] != 0){
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


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                           8. Annihilateurs de ANF                                              |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
int vect_vers_ANF(ANF* h, unsigned long* vect, baseMonomes* B){
    for(int j = 0; j < B->nbMonomes; j++){
        if(vect[j] != 0){
            h->monome[B->monome[j]] = 1;
        }
    }
    return 0;
}


/*  |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
    |                                     9. Résolution - fonction principal                                         |
    |----------------------------------------------------------------------------------------------------------------|
    |----------------------------------------------------------------------------------------------------------------|
*/
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
    printf("CONFIGURATION :\n    Nombre total de variables n = %d\n", n);

    /* Initialisation des ANF f et g = f+1 */
    ANF f;
    ANF g;

    /* Lecture de l'ANF f à partir du fichier    */
    init_ANF(&f, n);
    lectureANF(&f, fichier);

    /* Définition de g = f + 1  */
    init_ANF_copie(&g, &f);
    g.monome[0] = g.monome[0] ^ 1;

    /*  Affichage de f  */
    printf("    f = "); afficherANF(&f);

    /* Calcul des support de f et g (et leurs poids) */
    supportANF(&f);
    supportANF(&g);
    int poids_f = f.poids;
    int poids_g = g.poids;

    /* Calcul de la base des monomes de degré au plus d  */
    unsigned int d = atoi(argv[2]);
    baseMonomes B;
    init_baseMonomes(&B, n, d);
    unsigned int nbMonomes = B.nbMonomes;
    
    /* Matrice de f (resp. g) dont le noyau est l'annihilateur de f (resp. g) */
    matrice M1, M2, T1, T2;
    init_matrice(&M1, nbMonomes, poids_f);
    init_matrice(&M2, nbMonomes, poids_g);
    matriceANF(&M1, &f, &B);
    matriceANF(&M2, &g, &B);

    /* Extraction de AN(f) et AN(f+1) (leurs bases respectives) */
    int dimANf;
    int dimANg;
    init_matrice(&T1, nbMonomes, nbMonomes);
    init_matrice(&T2, nbMonomes, nbMonomes);
    elimGauss(&T1, &M1, n, &dimANf);
    elimGauss(&T2, &M2, n, &dimANg);

    /* Affichade de AN(f) et AN(f+1) */
    printf("\n\nRESULTATS:");
    printf("\n*************************************************");
    if(dimANf == 0){
        printf("AN(f) = {}");
    }
    else{
        ANF baseANf[dimANf];
        for(int i = 0; i < dimANf; i++){
            init_ANF(&baseANf[i], n);
            vect_vers_ANF(&baseANf[i], T1.coefficient[B.nbMonomes-1-i], &B);
            printf("\n"); afficherANF(&baseANf[i]);
            vider_ANF(&baseANf[i]);
        }
    }
    printf("\n\nDimension de AN(f) = %d", dimANf);
    printf("\n*************************************************");
    if(dimANg == 0){
        printf("\nAN(f+1) = {}");
    }
    else{
        ANF baseANg[dimANg];
        for(int i = 0; i < dimANg; i++){
            init_ANF(&baseANg[i], n);
            vect_vers_ANF(&baseANg[i], T2.coefficient[B.nbMonomes-1-i], &B);
            printf("\n"); afficherANF(&baseANg[i]);
            vider_ANF(&baseANg[i]);
        }
    }
    printf("\n\nDimension de AN(f+1) = %d", dimANg);
    printf("\n*************************************************\n");

    /* Libération de la mémoire */
    fclose(fichier);
    vider_ANF(&f);
    vider_ANF(&g);
    vider_baseMonomes(&B);
    vider_matrice(&M1);
    vider_matrice(&M2);
    vider_matrice(&T1);
    vider_matrice(&T2);
    return 0;
}