""" @nougzarm
    sujet TP2 du cours de crypto symétrique
    Master2 MIC 2024-2025 """
"""  __________________________________________________________________________________________________
    |                        TP2 - Attaque intégrale contre 4 tours de l’AES                           |
    |__________________________________________________________________________________________________| """

""" But : Le but est d'implémenter l'attaque présentée dans le sujet TP2.pdf
        Pour pouvoir le faire, on commence par l'implémentation de l'AES même.
    
    Rappel : On rappelle que AES chiffre des mots de 16 octets (= 128 bits), qui seront donc représentés par
        des matrices de taille 4*4 octets. Cette représentation est utile car elle rend plus intuitives
        les opérations intermédiaires de l'AES  """

# Mots tests
a = [[1, 3, 0, 3],
     [1, 0, 5, 3],
     [1, 2, 5, 3],
     [1, 2, 5, 3]]

b = [[1, 2, 1, 3],
     [0, 2, 0, 3],
     [1, 2, 4, 3],
     [1, 2, 5, 5]]

# Fonction addition (XOR) entre deux matrices 4*4 octets - AddRoundKey (ARK)
def XOR_mots(a, b):
    c = [[] for i in range(4)]
    for i in range(4):
        for j in range(4):
            c[i].append(a[i][j]^b[i][j])
    return c

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

# Fonction SubBytes (SB)
def SubBytes(x):
    for i in range(4):
        for j in range(4):
            x[i][j] = Boite_S_AES[x[i][j]]
    return x

# Fontion ShiftRows (SR)
def ShiftRows(x):
    y = [[] for i in range(4)]
    for i in range(4):
        for j in range(4):
            y[i].append(x[i][(j+i)%4])
    return y

""" Multiplication d'octets : les octets sont vus comme des éléments du corps F_{2^8} = F_2[X]/(P) où P = X^7 + X^5 + X + 1)
        Ce qui nous permet de définir la multiplication d'octets. Avant de pouvoir implémenter la fonction 
        multiplication nous avons besoin de fonctions intermédiaires    """
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

# Renvoie le degré d'un polynome inscrit sur une liste de longueur d+1 
def deg_pol_d(Q, d):
    for i in range(d):
        if(Q[d-i] != 0):
            return d-i
    return -1

# Multiplication d'octets (fonction finale)
def MultiplicationAvantReduction(a, b):
    polynome_produit = [0]*15   # Le polynome produit avant réduction
    for i in range(8):
        for j in range(8):
            polynome_produit[i+j] += (a>>i & 1)*(b>>j & 1)
    for i in range(len(polynome_produit)):
        polynome_produit[i] = polynome_produit[i]%2
    return polynome_produit

# Réduction d'un polynome de degre (au plus) d par P = X^8 + X^4 + X^3 + X + 1
def reduction_P(Q, d):
    P = [1, 1, 0, 1, 1, 0, 0, 0, 1]+[0, 0, 0, 0, 0, 0]  # Le polynome P = X^8 + X^4 + X^3 + X + 1
    deg_actuel = deg_pol_d(Q, d)
    while(deg_actuel >= 8):
        Quotient = mul_pol_monome(P, 14, deg_actuel-8)
        Q = add_pol_deg_d(Q, Quotient, 14)
        deg_actuel = deg_pol_d(Q, d)
    return Q

# Fonction finale - multiplication entre deux octets
def multiplication(a, b):
    pdt_pol = reduction_P(MultiplicationAvantReduction(a, b), 14)
    pdt = 0
    for i in range(8):
        pdt += pdt_pol[i]*(2**i)
    return pdt

""" MixColums : maintenant qu'on peut multiplier deux octets, on peut implémenter l'étape MixColums de l'AES
        Pour cela on commence par définir la matrice constante M utilisée dans cette opération ainsi que la
        multiplication de deux matrice 4*4 d'octets    """
# Matrice constante M dans l'opération MixColumns
M = [[2, 3, 1, 1],
     [1, 2, 3, 1],
     [1, 1, 2, 3],
     [3, 1, 1, 2]]

# Multiplication matrices n*n octets
def multiplication_matrice(A, B, n):
    resultat = [[0]*n for i in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                resultat[i][j] += multiplication(A[i][k], B[k][j])
    return resultat

# Fonction principale MixColums qui consiste simplement à multiplier M par x
def MixColums(x):
    return multiplication_matrice(M, x, 4)

""" tour_AES : On peut à présent définir la fonction tour de l'AES qui prend une clé dérivé K_i 
    et qui retourne un chiffré    """

""" def tour_AES(x, cle_derivee):
    return(XOR_mots) """




# Tests et debug
""" print(XOR_mots(a, b))
print(SubBytes(XOR_mots(a, b)))
print(ShiftRows(XOR_mots(a, b)))
print(Boite_S_AES[0x01]) # -> 0x7c = 124
print(Boite_S_AES[0x2f]) # -> 0x15 = 21 """

A = [[2, 0, 0, 0],
     [0, 2, 0, 0],
     [0, 0, 2, 0],
     [0, 0, 0, 2]]

B = [[1, 0, 0, 0],
     [0, 0, 0, 0],
     [0, 0, 2, 0],
     [0, 0, 0, 0]]

print(multiplication_matrice(A, B, 4))