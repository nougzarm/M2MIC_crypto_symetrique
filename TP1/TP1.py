"""  __________________________________________________________________________________________________
    |                                    Partie 1 - Boîte-S de l'AES                                   |
    |__________________________________________________________________________________________________| """

""" QUESTION 1:
    Rappel : Une Boite-S est une application : F_{2^n} -> F_{2^m} 
    
    AES : Dans le cas de l'AES, on a n=m=8, il s'agit donc d'une fonction qui envoit un octet
        sur un autre. Voici le tableau des images de la boîte S utilisée dans le chiffrement AES, 
        ainsi que la fonction S_AES qui définit directement cette boîte-S :     """

Boite_S_AES =  [[0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76],
                [0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0],
                [0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15],
                [0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75],
                [0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84],
                [0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF],
                [0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8],
                [0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2],
                [0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73],
                [0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB],
                [0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79],
                [0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08],
                [0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A],
                [0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E],
                [0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF],
                [0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16]]

def S_AES(x):
    ligne = x//16
    colonne = x%16
    return Boite_S_AES[x//16][x%16]

# Quelques tests (dé-commenter si nécessaire) :
""" print(S_AES(0x01)) # -> 0x7c = 124
print(S_AES(0x2f)) # -> 0x15 = 21 """

""" Rappel: Table de distribution des différences (DDT)
        Soit S : F_{2^n} -> F_{2^n}  une boite-S. La DDT de S est une table de dimensions 2^n*2^n,
        où le coeff situé à l'intersection de la ligne a et de la colonne b est donné par :
                        d_S(a,b) := #{x \\in F_{2^n} | S(x)^S(x^a) = b}  
        où ^ est l'addition dans le corps F_{2^n}   
        
        Voici le calcul de la DDT de la boîte-S de l'AES :   """

# Initialisation de la table
DDT = [[0 for a in range(1<<8)] for b in range(1<<8)]

# Calcul des coefficients de la table
for a in range(1<<8):
    for x in range(1<<8):
        DDT[a][S_AES(x)^S_AES(x^a)] += 1

# Affichage de la DDT de la boîte-S de l'AES
""" for a in range(1<<8):
    print(DDT[a]) """


""" QUESTION 2:
    Fonctions coordonnées : Mathématiquement, une Boite-S peut être vue comme une fonction 
        booléenne vectorielle  S : F_{2^n} -> F_{2^m}, qui se décompose en m coordonnée
        s_0, s_1, ..., s_{m-1} où chaque coordonnée est une fonction booléenne.
        Ie s_i : F_{2^n} -> F_2. Cela s'exprime sous la forme :
                        S(x) = (s_0(x), s_1(x), ..., s_{m-1}(x))    
        où x \\in F_{2^n}.
    
    Remarque : Il est parfois préferable de représenter une boîte-S en utilisant la forme normale 
        algébrique (ANF) de ses fonctions coordonnées.
    
    Rappel/déf : Soit f une fonction booléenne en n variables. La forme algébrique normale de f est :
                        f(x) = \\bigoplus_{u \\in F_{2^n}} c_f(u)x^u
        où x^u = x_0^{u_0} x_1^{u_1} ... x_{n-1}^{u_{n-1}}
        et le coefficient c_f(u) peut être calculé à l'aide de la transformée de Möbius comme suit :
                        c_f(u) = \\bigoplus_{v \\in F_{2^n}, v<u} f(v) 
        où v<u ssi (u_i=0 => v_i=0) pour tout i     """

# Fonction permettant d'extraire des fonctions coordonnées de la Boîte-S de l'AES
def S_AES_i(i, x):
    # Décalage de l'octet de i bit vers la droite et applic. du masque
    return (S_AES(x) >> i) & 1  

""" Application : Le module sage.crypto.sbox.SBox de SageMath est très pratique pour calculer  de
        diverses propriétés (cryptographiques) des boîtes-S. On utilise ce module pour calculer la 
        forme algébrique normale de chacune des fonctions coordonnées d'une boîte-S.  """

# from sage.all import *
from sage.crypto.boolean_function import BooleanFunction

""" Premier exemple : Commençons par l'exemple de la feuille de TP : 
        On se donne la Boite-S S : F_{2^3} -> F_{2^3} suivante :    """

S_exemple = [3, 1, 6, 7, 0, 5, 2, 4]

def traitement_exemple1(Boite_S_ex):
    # Calcul des fonctions coordonnées de la boite-S exemple
    s_ex_i = [[] for i in range(3)]
    for i in range(3):
        for x in range(1<<3):
            s_ex_i[i].append(Boite_S_ex[x]>>i & 1)

    # Affichage des fonctions coordonnées (dé-commenter si nécessaire)
    for i in range(3):
        print(s_ex_i[i])

    # Conversion dans l'env sagemath et forme algébrique normale
    B_ex_i = []
    P_ex_i = []
    for i in range(3):
        B_ex_i.append(BooleanFunction(s_ex_i[i]))
        P_ex_i.append(B_ex_i[i].algebraic_normal_form())
        # Affichage du polynome (dé-commenter si nécessaire) :
        print(P_ex_i[i])

# Traitement exemple (dé-commenter):
# traitement_exemple1(S_exemple)

""" Remarque :
        Etant donnée une fonction booléenne f : F_{2^n} -> F_2 et F sa forme normale
        algébrique générée par la librairie sagemath, pour calculer f(x), on compose
        F(x_0, x_1, ..., x_{n-1}) où x_0 est le bit de poid FAIBLE (tout à droite) de x 
        
    Cas de l'AES : Passons à présent au cas de l'AES    """

# Calculs fonctions coordonnées de la boîte-S de l'AES
s_i = [[] for i in range(8)]
for i in range(8):
    for x in range(1<<8):
        s_i[i].append(S_AES_i(i, x))

# Affichage des fonctions coordonnées (dé-commenter si nécessaire)
""" for i in range(8):
    print(s_i[i]) """

# Conversion des fonctions coordonnées vers l'environnement sagemath
B_i = []    # Liste des BooleanFunction's
P_i = []    # Liste des ANF correspondantes
for i in range(8):
    B_i.append(BooleanFunction(s_i[i]))
    P_i.append(B_i[i].algebraic_normal_form())

# Affichage des ANF (dé-commenter si nécessaire)
""" for i in range(8):
    print(P_i[i]) """

# Tests pour 1, 2 et 3 (dé-commenter si nécessaire)
""" print([P_i[i](1, 0, 0, 0, 0, 0, 0, 0) for i in range(8)])
print([P_i[i](0, 1, 0, 0, 0, 0, 0, 0) for i in range(8)])
print([P_i[i](1, 1, 0, 0, 0, 0, 0, 0) for i in range(8)]) """


"""  __________________________________________________________________________________________________
    |                  Partie 2 - Résistance de l’AES à la cryptanalyse différentielle                 |
    |__________________________________________________________________________________________________| """

""" QUESTION 2.2: Une approche par programmation linéaire mixte par contrainte (MILP)   """

def minimisation_MILP(A, r);
    p = MixedIntegerLinearProgram(maximization=False)
    s = p.new_variable(binary=True)
    M = p.new_variable(binary=True)
    for tour in range(r):
        for j in range(4):
            # Corriger contrainte
            p.add_constraint(5*M[tour][j] <= s[tour][(j)%4]+s[tour][(1+j)%4]+s[tour][(2+j)%4]+s[tour][(3+j)%4] <= 8*M[tour][j])
    p.set_objective(sum(s[i][j] for i in range(4) for j in range(4))) # Revoir la syntaxte pour la somme
    p.show