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