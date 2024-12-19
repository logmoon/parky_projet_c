#ifndef CITOYEN_H_INCLUDED
#define CITOYEN_H_INCLUDED

typedef struct {
    char nom[50];
    char prenom[50];
    int cin;
    int numero;
    char sexe[10];     // "homme" or "femme"
} Citoyen;

int ajouter_citoyen(char *filename, Citoyen c);
int modifier_citoyen(char *filename, int cin, Citoyen nouv);
int supprimer_citoyen(char *filename, int cin);
Citoyen chercher_citoyen(char *filename, int cin);
// Function to generate the invoice and write it to a file
void generer_facture(Citoyen c, int duree_parking, float tarif, const char *facture_filename);

#endif // CITOYEN_H_INCLUDED
