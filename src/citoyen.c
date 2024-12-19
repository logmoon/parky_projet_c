#include <stdio.h>
#include <string.h>
#include "citoyen.h"

int ajouter_citoyen(char *filename, Citoyen c) {
    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "%d %s %s %d %s\n", c.cin, c.nom, c.prenom, c.numero, c.sexe);
        fclose(f);
        return 1;  // Success
    }
    return 0;  // Failure
}

int modifier_citoyen(char *filename, int cin, Citoyen nouv) {
    int found = 0;
    Citoyen c;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &c.cin, c.nom, c.prenom, &c.numero, c.sexe) != EOF) {
            if (c.cin == cin) {
                fprintf(f2, "%d %s %s %d %s\n", nouv.cin, nouv.nom, nouv.prenom, nouv.numero, nouv.sexe);
                found = 1;
            } else {
                fprintf(f2, "%d %s %s %d %s\n", c.cin, c.nom, c.prenom, c.numero, c.sexe);
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp.txt", filename);
    } else {
        return 0;  // Failure due to file handling issue
    }

    return found;
}

int supprimer_citoyen(char *filename, int cin) {
    int found = 0;
    Citoyen c;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &c.cin, c.nom, c.prenom, &c.numero, c.sexe) != EOF) {
            if (c.cin == cin) {
                found = 1;
            } else {
                fprintf(f2, "%d %s %s %d %s\n", c.cin, c.nom, c.prenom, c.numero, c.sexe);
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp.txt", filename);
    } else {
        return 0;  // Failure due to file handling issue
    }

    return found;
}

Citoyen chercher_citoyen(char *filename, int cin) {
    Citoyen c;
    c.cin = -1;  // Initialize as not found
    FILE *f = fopen(filename, "r");

    if (f != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &c.cin, c.nom, c.prenom, &c.numero, c.sexe) != EOF) {
            if (c.cin == cin) {
                fclose(f);
                return c;  // Found
            }
        }
        fclose(f);
    }

    c.cin = -1;  // Not found
    return c;
}


// Function to generate an invoice and write it to a file
void generer_facture(Citoyen c, int duree_parking, float tarif, const char *facture_filename) {
    // Calculate the total amount
    float montant = duree_parking * tarif;

    // Open the file to write the facture
    FILE *f = fopen(facture_filename, "a");
    if (f != NULL) {
	fprintf(f, "--- Facture ---\n");
        fprintf(f, "Nom: %s %s\n", c.prenom, c.nom);
        fprintf(f, "CIN: %d\n", c.cin);
        fprintf(f, "Durée de stationnement: %d heures\n", duree_parking);
        fprintf(f, "Tarif par heure: %.2f TND\n", tarif);
        fprintf(f, "Montant total à payer: %.2f TND\n\n", montant);
        fclose(f);
    } else {
        printf("Erreur lors de l'écriture du fichier facture.\n");
    }
}
