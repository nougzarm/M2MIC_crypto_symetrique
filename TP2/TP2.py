""" @nougzarm
    sujet TP2 du cours de crypto symétrique
    Master2 MIC 2024-2025 """
"""  __________________________________________________________________________________________________
    |                      TP2 - Attaque intégrale contre 4 tours de l’AES-128                         |
    |__________________________________________________________________________________________________| """

""" But : Le but est d'implémenter l'attaque présentée dans le sujet TP2.pdf
        Pour pouvoir le faire, on commence par l'implémentation de l'AES-128 même.
    
    Rappel : On rappelle que AES-128 chiffre des mots de 16 octets (= 128 bits), qui seront donc représentés par
        des matrices de taille 4*4 octets. Cette représentation est utile car elle rend plus intuitives
        les opérations intermédiaires de l'AES  """
          
"""  __________________________________________________________________________________________________
    |                                               Constantes                                         |
    |__________________________________________________________________________________________________| 

    Début : Commençons par définir les différentes constantes nécessaires pour l'implémentation de l'AES :
        - Boîte-S de l'AES utilisée dans SubBytes (ainsi que son inverse dans invSubBytes)
        - Matrice mat_MC utilisée dans MixColumns (et son inverse inv_mat_MC dans invMixColumns)
        - Liste RCON utilisée dans la dérivation de clé  """

# Boite-S utilisée pour l'AES
Boite_S_AES =  [0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
                0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
                0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
                0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
                0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
                0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
                0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
                0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
                0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
                0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
                0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
                0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
                0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
                0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
                0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
                0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16]

# Inverse de la boîte-S de l'AES
Boite_S_inv_AES =  [0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
                    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
                    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
                    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
                    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
                    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
                    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
                    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
                    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
                    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
                    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
                    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
                    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
                    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
                    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
                    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D]

# Matrice mat_MC utilisée dans la multiplication de MixColumns
mat_MC =   [[2, 3, 1, 1],
            [1, 2, 3, 1],
            [1, 1, 2, 3],
            [3, 1, 1, 2]]

# Ainsi que son inverse :
inv_mat_MC =   [[14, 11, 13, 9],
                [9, 14, 11, 13], 
                [13, 9, 14, 11], 
                [11, 13, 9, 14]]

# La liste Rcon utilisée dans la dérivation de clé
rcon = [[0x01, 0x00, 0x00, 0x00], [0x02, 0x00, 0x00, 0x00],
        [0x04, 0x00, 0x00, 0x00], [0x08, 0x00, 0x00, 0x00],
        [0x10, 0x00, 0x00, 0x00], [0x20, 0x00, 0x00, 0x00],
        [0x40, 0x00, 0x00, 0x00], [0x80, 0x00, 0x00, 0x00],
        [0x1B, 0x00, 0x00, 0x00], [0x36, 0x00, 0x00, 0x00]]


"""  __________________________________________________________________________________________________
    |                      AddRoundKey (ARK), SubBytes (SB) et ShiftRows (SR)                          |
    |__________________________________________________________________________________________________| """
""" Définition des fonctions suivantes utilisée dans l'AES-128:
        - AddRoundKey (ARK) qui est simplement XOR_mots(x, cle_derivee_k)
        - SubBytes (SB) ainsi que son inverse
        - ShiftRows (SR) ainsi que son inverse (permutation 0x00010203050607040a0b08090f0c0d0e)    """

# Fonction addition (XOR) entre deux matrices 4*4 octets - AddRoundKey (ARK)
def XOR_mots(a, b):
    c = [[] for i in range(4)]
    for i in range(4):
        for j in range(4):
            c[i].append(a[i][j]^b[i][j])
    return c

# Fonction SubBytes (SB)
def SubBytes(x):
    y = [[0]*4 for i in range(4)]
    for i in range(4):
        for j in range(4):
            y[i][j] = Boite_S_AES[x[i][j]]
    return y

# Inverse de la fonction SubBytes (invSB)
def invSubBytes(x):
    y = [[0]*4 for i in range(4)]
    for i in range(4):
        for j in range(4):
            y[i][j] = Boite_S_inv_AES[x[i][j]]
    return y

# Fontion ShiftRows (SR)
def ShiftRows(x):
    y = [[] for i in range(4)]
    for i in range(4):
        for j in range(4):
            y[i].append(x[i][(j+i)%4])
    return y

# Inverse de la fonction ShiftRows (invSR)
def invShiftRows(x):
    y = [[] for i in range(4)]
    for i in range(4):
        for j in range(4):
            y[i].append(x[i][(j-i)%4])
    return y


"""  __________________________________________________________________________________________________
    |                             MixColumns (MC) - Multiplication d'octets                            |
    |__________________________________________________________________________________________________|

    Les octets sont vus comme des éléments du corps F_{2^8} = F_2[X]/(P) où P = X^7 + X^5 + X + 1
    Ce qui nous permet de définir la multiplication d'octets. Avant de pouvoir implémenter la fonction 
    multiplication nous avons besoin de fonctions intermédiaires qui sont les suivantes :
        - add_pol_deg_d(a, b, d)
        - mul_pol_monome(Q, d, m)
        - deg_pol_d(Q, d)
        - MultiplicationAvantReduction(a, b)
        - reduction_P(Q, d)     """

# Addition de deux polynomes de F_2[X] de degré au plus d (a et b doivent être de longueur d)
def add_pol_deg_d(a, b, d):
    resultat = [0]*(d+1)
    for i in range(d+1):
        resultat[i] = (a[i]+b[i])%2
    return resultat

# Multiplication d'un polynome Q de F_2[X] par X^m (Q doit être une liste de longueur d+1 mais de degré au plus d-m)
def mul_pol_monome(Q, d, m):
    Q_res = [0]*(d+1)
    for i in range(d-m+1):
        Q_res[d-i] = Q[d-i-m] 
    return Q_res

# Renvoie le degré d'un polynome inscrit sur une liste de longueur d+1 (nécessaire pour la réduction par un polynome)
def deg_pol_d(Q, d):
    for i in range(d):
        if(Q[d-i] != 0):
            return d-i
    return -1

# Multiplication d'octets sans réduction (nécessaire pour la multiplication d'octets)
def MultiplicationAvantReduction(a, b):
    polynome_produit = [0]*15   # Le polynome produit avant réduction
    for i in range(8):
        for j in range(8):
            polynome_produit[i+j] += (a>>i & 1)*(b>>j & 1)
    for i in range(len(polynome_produit)):
        polynome_produit[i] = polynome_produit[i]%2
    return polynome_produit

# Réduction d'un polynome de degre =< d par P = X^8 + X^4 + X^3 + X + 1 (nécessaire pour la multiplication d'octets)
def reduction_P(Q, d):
    P = [1, 1, 0, 1, 1, 0, 0, 0, 1]+[0, 0, 0, 0, 0, 0]  # Le polynome P = X^8 + X^4 + X^3 + X + 1
    deg_actuel = deg_pol_d(Q, d)
    while(deg_actuel >= 8):
        Quotient = mul_pol_monome(P, 14, deg_actuel-8)
        Q = add_pol_deg_d(Q, Quotient, 14)
        deg_actuel = deg_pol_d(Q, d)
    return Q

# Multiplication entre deux octets (nécessaire pour la multiplication de matrices)
def multiplication(a, b):
    pdt_pol = reduction_P(MultiplicationAvantReduction(a, b), 14)
    pdt = 0
    for i in range(8):
        pdt += pdt_pol[i]*(2**i)
    return pdt

# Multiplication matrices n*n octets (nécessaire pour MixColumns)
def multiplication_matrice(A, B, n):
    resultat = [[0]*n for i in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                resultat[i][j] ^= multiplication(A[i][k], B[k][j])
    return resultat

# Fonction MixColumns qui consiste simplement à multiplier mat_MC par x
def MixColumns(x):
    return multiplication_matrice(mat_MC, x, 4)

# Inverse de la fonction MixColumns (invMC)
def invMixColumns(x):
    return multiplication_matrice(inv_mat_MC, x, 4)


"""  __________________________________________________________________________________________________
    |                                         Dérivation de clé                                        |
    |__________________________________________________________________________________________________|

    Définition de la fonction de dérivation de clé : derivation_cle(cle)
        - Nk est la taille de la clé 'cle' en mots de 32 bits (4 pour l'AES-128)
        - Nr désigne le nombre de tour de l'AES (10 pour l'AES-128)  
        - Renvoie une liste de 11 matrices 4*4 octets qu'on peut directement utiliser pour AddRoundKey   
    Prérequis :
        - Fonction de rotation d'un mot de 32 bits vers la gauche de nb_bit bits : rot_mot(mot, nb_bit)
        - Fonction appliquant la boite-S octet par octet sur un mot : sbox_mot(mot)  """

def rot_mot(mot, nb_bit):
    mot_resultat = ((mot << nb_bit) | (mot >> (32 - nb_bit))) & 0xffffffff
    return mot_resultat

def sbox_mot(mot):
    resultat = 0
    for i in range(4):
        octet_temp = (mot >> 8*(4-i-1))%(2**8)
        resultat += Boite_S_AES[octet_temp]*(2**(8*(3-i)))
    return resultat

def derivation_cle(cle):
    Nk = 4
    Nr = Nk + 6
    W = [0]*(4*(Nr+1))   # On a Nr+1 clé dérivée de 128 bits
    # On recopie la clé maître dans les Nk premiers mots de W
    for i in range(Nk):
        W[i] = (cle >> (32*(Nk-i-1)))%(2**32)
    # Génération du reste :
    for i in range(Nk, 4*(Nr+1)):
        temporaire = W[i-1]
        # Calcul de W[i] en fonction des cas
        if i%Nk == 0:
            temporaire = rot_mot(temporaire, 8) # Rotation du mot de 8 bits vers la gauche
            temporaire = sbox_mot(temporaire)   # Application de la boite-S octet par octer
            temporaire = temporaire ^ (rcon[i//Nk - 1][0])*(2**(8*3))
        W[i] = W[i-Nk] ^ temporaire
    # Conversion en liste de clés qui sont des matrices de 4*4 octets 
    W_128 = []
    for i in range(Nr+1):
        texte_temp = 0
        for j in range(4):
            texte_temp = texte_temp + (W[4*i + j] << 32*(3-j))
        W_128.append(texte_vers_mat(texte_temp))
    return W_128


"""  __________________________________________________________________________________________________
    |                                 Chiffrement/Déchiffrement AES-128                                |
    |__________________________________________________________________________________________________|
    
    tour_AES : On peut à présent définir la fonction tour de l'AES qui prend une clé dérivé K_i 
        et qui retourne un chiffré. 
    
    Remarque :  
        Si option_MixColumns = 0 : l'opération MixColumns n'est pas appliquée
        Si option_MixColumns != 0 : l'opération MixColumns est appliquée      """

def tour_AES(x, cle_derivee, option_MixColumns):
    resultat = SubBytes(x)
    print(f"Après SubBytes : {hex(mat_vers_texte(resultat))}")
    resultat = ShiftRows(resultat)
    print(f"Après ShiftRows : {hex(mat_vers_texte(resultat))}")
    if option_MixColumns != 0:
        resultat = MixColumns(resultat)
        print(f"Après MixColumns : {hex(mat_vers_texte(resultat))}")
    resultat = XOR_mots(resultat, cle_derivee)
    print(f"Après AddRoundKey : {hex(mat_vers_texte(resultat))}")
    return resultat

""" Fonction de chiffrement AES :
        - x est un mot de 128 bits sous la forme d'une matrice 4*4 octets
        - cle_prive doit être une chaîne de 128 bits
        - nb_tour vaut au plus 10
        - Initialiser nb_tour = 10 pour avoir le chiffrement AES-128    
    
    Remarque : Pas de fonction MixColumns pour le dernier tour  """

def AES_chiffrement(x, cle_prive, nb_tour):
    print("Chiffrement : ")
    cle = derivation_cle(cle_prive)
    print(f"Utilisation de la première clé : {hex(mat_vers_texte(cle[0]))}")
    resultat = XOR_mots(x, cle[0])
    print(f"Après premier AddRoundKey : {hex(mat_vers_texte(resultat))}")
    for i in range(nb_tour):
        print(f"Tour numéro {i+1} : ")
        resultat = tour_AES(resultat, cle[i+1], nb_tour-1-i)
        print(f"Après tour {i+1} : {hex(mat_vers_texte(resultat))}")
    return resultat

""" Déchiffrement :
        On définit de même la fonction de tour inverse ainsi que le déchiffrement AES :     """

def inv_tour_AES(x, cle_derivee, option_MixColumns):
    resultat = XOR_mots(x, cle_derivee)
    print(f"après invAddRoundKey : {hex(mat_vers_texte(resultat))}")
    if option_MixColumns != 0:
        resultat = invMixColumns(resultat)
        print(f"après invMixColumns : {hex(mat_vers_texte(resultat))}")
    resultat = invShiftRows(resultat)
    print(f"après invShiftRows : {hex(mat_vers_texte(resultat))}")
    resultat = invSubBytes(resultat)
    print(f"après invSubBytes : {hex(mat_vers_texte(resultat))}")
    return resultat

def AES_dechiffrement(x, cle_prive, nb_tour):
    print(f"dechiffrement de {hex(mat_vers_texte(x))}")
    cle = derivation_cle(cle_prive)
    for i in range(nb_tour):
        print(f"dechiffrement tour {10-i}")
        x = inv_tour_AES(x, cle[nb_tour-i], i)
        print(f"apres tour {10-i} : {hex(mat_vers_texte(x))}")
    x = XOR_mots(x, cle[0])
    print(f"après dernier invAddRoundKey : {hex(mat_vers_texte(x))}")
    return x


"""  __________________________________________________________________________________________________
    |                                       Fonctions de conversion                                    |
    |__________________________________________________________________________________________________| 

    Définition des fonctions suivantes :
        - texte_vers_mat() : conversion d'une chaîne de 128 bits bruts en une matrice 4*4 octets
        - mat_vers_texte() : conversion d'une matrice 4*4 octets en une chaîne de 128 bits bruts    """

def texte_vers_mat(texte):
    mat = []
    for i in range(4):
        temp = [0]*4
        for j in range(4):
            temp[j] = (texte >> (32*(3-i) + 8*(3-j))) & 0xff
        mat.append(temp)
    return mat

def mat_vers_texte(mat):
    texte = 0
    for i in range(4):
        for j in range(4):
            texte = (texte << 8) | mat[i][j]
    return texte


# Tests et debug

A = [[2, 137, 62, 48],
     [128, 212, 98, 17],
     [91, 45, 2, 87],
     [0, 0, 92, 19]]

B = [[1, 0, 0, 0],
     [0, 1, 0, 0],
     [0, 2, 2, 20],
     [0, 4, 9, 0]]

cle_exemple = 0xf2c3a8b7d9e45f6a1d2e3c4b5a6f7c7e
texte_exemple = 0xf2c3a8b7d9e45f6a1d2e3c4b5a6f7c8e
texte_mat = texte_vers_mat(0xf2c3a8b7d9e45f6a1d2e3c4b5a6f7c8e)

chiffre_exemple_mat = AES_chiffrement(texte_mat, cle_exemple, 10)
dechiffre_exemple_mat = AES_dechiffrement(chiffre_exemple_mat, cle_exemple, 10)

dechiffre_exemple_texte = mat_vers_texte(dechiffre_exemple_mat)
print(f"Voici le déchiffrement : {hex(dechiffre_exemple_texte)}")

""" print(hex(texte_exemple))
print(hex(mat_vers_texte(XOR_mots(texte_mat, texte_mat)))) """
