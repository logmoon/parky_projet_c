#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agent.h"
#include <gtk/gtk.h>

enum {
    ECIN,
    ENOM,
    EPRENOM,
    ESEXE,
    ENAISSANCE,
    EEMAIL,
    EADRESSE,
    ESERVICE,
    COLUMNS // Nombre total de colonnes
};
enum {
    EIDRES,
    EJOURS,
    EMOIS,
    EANNEE,
    EDUREE,
    ECOLUMNS
};

// Ajouter un agent
int ajouter(char *filename, Agent agent) {
    FILE *f = fopen(filename, "a");
    if (f == NULL) return 0;
    fprintf(f, "%d %s %s %s %d %d %d %s %s %s\n", 
            agent.CIN, agent.nom, agent.prenom, agent.sexe,
            agent.jour_naissance, agent.mois_naissance, agent.annee_naissance,
            agent.email, agent.adresse, agent.service);
    fclose(f);
    return 1;
}

// Modifier un agent
int modifier(char *filename, int CIN, Agent new_agent) {
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) return 0;

    Agent a;
    int found = 0;

    while (fscanf(f, "%d %49s %49s %9s %d %d %d %99s %99s %49s\n", 
                  &a.CIN, a.nom, a.prenom, a.sexe, 
                  &a.jour_naissance, &a.mois_naissance, &a.annee_naissance, 
                  a.email, a.adresse, a.service) != EOF) {
        if (a.CIN == CIN) {
            fprintf(temp, "%d %s %s %s %d %d %d %s %s %s\n", 
                    new_agent.CIN, new_agent.nom, new_agent.prenom, new_agent.sexe,
                    new_agent.jour_naissance, new_agent.mois_naissance, new_agent.annee_naissance,
                    new_agent.email, new_agent.adresse, new_agent.service);
            found = 1;
        } else {
            fprintf(temp, "%d %s %s %s %d %d %d %s %s %s\n", 
                    a.CIN, a.nom, a.prenom, a.sexe,
                    a.jour_naissance, a.mois_naissance, a.annee_naissance,
                    a.email, a.adresse, a.service);
        }
    }

    fclose(f);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("temp.txt", filename);
    } else {
        remove("temp.txt");
    }

    return found;
}

// Supprimer un agent
int supprimer(char *filename, int CIN) {
    FILE *f = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (f == NULL || temp == NULL) return 0;

    Agent a;
    int found = 0;

    while (fscanf(f, "%d %49s %49s %9s %d %d %d %99s %99s %49s\n", 
                  &a.CIN, a.nom, a.prenom, a.sexe, 
                  &a.jour_naissance, &a.mois_naissance, &a.annee_naissance, 
                  a.email, a.adresse, a.service) != EOF) {
        if (a.CIN != CIN) {
            fprintf(temp, "%d %s %s %s %d %d %d %s %s %s\n", 
                    a.CIN, a.nom, a.prenom, a.sexe,
                    a.jour_naissance, a.mois_naissance, a.annee_naissance,
                    a.email, a.adresse, a.service);
        } else {
            found = 1;
        }
    }

    fclose(f);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("temp.txt", filename);
    } else {
        remove("temp.txt");
    }

    return found;
}

// Rechercher un agent
Agent chercher(char *filename, int CIN) {
    FILE *f = fopen(filename, "r");
    Agent a;
    a.CIN = -1; // Valeur par défaut indiquant que l'agent n'est pas trouvé.

    if (f != NULL) {
        while (fscanf(f, "%d %49s %49s %9s %d %d %d %99s %99s %49s\n", 
                      &a.CIN, a.nom, a.prenom, a.sexe, 
                      &a.jour_naissance, &a.mois_naissance, &a.annee_naissance, 
                      a.email, a.adresse, a.service) != EOF) {
            if (a.CIN == CIN) {
                fclose(f);
                return a; // Retourne l'agent trouvé.
            }
        }
        fclose(f);
    }

    // Si aucun agent n'a été trouvé, la valeur par défaut est retournée.
    return a;
}
int pull( int recid,char tel[9]){
   FILE *f = fopen("add.txt", "r");
    if (f != NULL)
    {
        fscanf(f, "%d",&recid);
        fclose(f);
    }
	remove("add.txt");
	return recid;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

   void afficherListeTrieeParReservations(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier des réservations.\n");
        return;
    }

    reservation resList[MAX_RESERVATIONS];
    int count = 0;
    reservation res;

    // Lire les données des réservations
    while (fscanf(f, "%s %d %d %d %d\n", res.idres, &res.jours, &res.mois, &res.annee, &res.duree) != EOF) {
        resList[count++] = res;
    }
    fclose(f);

    // Trier les réservations par durée décroissante
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (resList[i].duree < resList[j].duree) {
                reservation temp = resList[i];
                resList[i] = resList[j];
                resList[j] = temp;
            }
        }
    }

    // Afficher ou écrire dans un fichier
    FILE *sortedFile = fopen("reservations_triees.txt", "w");
    if (sortedFile == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sortie.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(sortedFile, "%s %d/%d/%d %d jours\n",
                resList[i].idres, resList[i].jours, resList[i].mois, resList[i].annee, resList[i].duree);
    }

    fclose(sortedFile);
    printf("Liste des réservations triée enregistrée dans 'reservations_triees.txt'.\n");
}
void afficher_lsttree(GtkWidget *liste, char service[]) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;

    int CIN;
    char nom[MAX_NOM_LEN], prenom[MAX_PRENOM_LEN], sexe[MAX_SEXE_LEN];
    int jour_naissance, mois_naissance, annee_naissance;
    char email[MAX_EMAIL_LEN], adresse[MAX_ADRESSE_LEN], agent_service[MAX_SERVICE_LEN];
    store = NULL;

    FILE *f;
    store = gtk_tree_view_get_model(liste);

    if (store == NULL) {
        // Colonne CIN
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", ECIN, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Nom
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", ENOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Prénom
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", EPRENOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Sexe
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Sexe", renderer, "text", ESEXE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Date de naissance
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Naissance", renderer, "text", ENAISSANCE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Email
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Email", renderer, "text", EEMAIL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Adresse
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Adresse", renderer, "text", EADRESSE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        // Colonne Service
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Service", renderer, "text", ESERVICE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    store = gtk_list_store_new(COLUMNS,
                               G_TYPE_INT,    // CIN
                               G_TYPE_STRING, // Nom
                               G_TYPE_STRING, // Prénom
                               G_TYPE_STRING, // Sexe
                               G_TYPE_STRING, // Naissance (formatée)
                               G_TYPE_STRING, // Email
                               G_TYPE_STRING, // Adresse
                               G_TYPE_STRING  // Service
    );

    f = fopen("agents.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier des agents.\n");
        return;
    }

    while (fscanf(f, "%d %s %s %s %d %d %d %s %s %s\n", &CIN, nom, prenom, sexe,
                  &jour_naissance, &mois_naissance, &annee_naissance, email, adresse, agent_service) != EOF) {
        // Filtrer les agents par service si nécessaire
        if (strlen(service) == 0 || strcmp(service, agent_service) == 0) {
            char naissance[15];
            sprintf(naissance, "%02d/%02d/%04d", jour_naissance, mois_naissance, annee_naissance);

            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               ECIN, CIN,
                               ENOM, nom,
                               EPRENOM, prenom,
                               ESEXE, sexe,
                               ENAISSANCE, naissance,
                               EEMAIL, email,
                               EADRESSE, adresse,
                               ESERVICE, agent_service,
                               -1);
        }
    }

    fclose(f);

    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

void supprimer_agttree(Agent a, char service[]) {
    int CIN;
    char nom[MAX_NOM_LEN], prenom[MAX_PRENOM_LEN], sexe[MAX_SEXE_LEN];
    int jour_naissance, mois_naissance, annee_naissance;
    char email[MAX_EMAIL_LEN], adresse[MAX_ADRESSE_LEN], agent_service[MAX_SERVICE_LEN];

    Agent temp;
    FILE *f, *g;

    f = fopen("agents.txt", "r");
    g = fopen("temp.txt", "w");

    if (f == NULL || g == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return;
    }

    while (fscanf(f, "%d %s %s %s %d %d %d %s %s %s\n",
                  &temp.CIN, temp.nom, temp.prenom, temp.sexe,
                  &temp.jour_naissance, &temp.mois_naissance, &temp.annee_naissance,
                  temp.email, temp.adresse, temp.service) != EOF) {
        // Vérification des critères pour ne pas écrire l'agent à supprimer
        if (a.CIN != temp.CIN ||
            strcmp(a.nom, temp.nom) != 0 ||
            strcmp(a.prenom, temp.prenom) != 0 ||
            strcmp(a.sexe, temp.sexe) != 0 ||
            a.jour_naissance != temp.jour_naissance ||
            a.mois_naissance != temp.mois_naissance ||
            a.annee_naissance != temp.annee_naissance ||
            strcmp(a.email, temp.email) != 0 ||
            strcmp(a.adresse, temp.adresse) != 0 ||
            strcmp(a.service, temp.service) != 0) {
            fprintf(g, "%d %s %s %s %d %d %d %s %s %s\n",
                    temp.CIN, temp.nom, temp.prenom, temp.sexe,
                    temp.jour_naissance, temp.mois_naissance, temp.annee_naissance,
                    temp.email, temp.adresse, temp.service);
        }
    }

    fclose(f);
    fclose(g);

    // Remplacement du fichier original par le fichier temporaire
    remove("agents.txt");
    rename("temp.txt", "agents.txt");
}
void afficher_agttree(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;

    int CIN;
    char nom[MAX_NOM_LEN];
    char prenom[MAX_PRENOM_LEN];
    char service[MAX_SERVICE_LEN];

    store = NULL;

    FILE *f;
    store = gtk_tree_view_get_model(liste);

    // Si le modèle est NULL, initialisez les colonnes
    if (store == NULL) {
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", ECIN, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", ENOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", EPRENOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Service", renderer, "text", ESERVICE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Définir le modèle de données pour l'arbre
    store = gtk_list_store_new(ECOLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    f = fopen("agents.txt", "r");
    if (f == NULL) {
        return;
    } else {
        // Lire le fichier ligne par ligne et remplir le modèle GTK
        while (fscanf(f, "%d %s %s %*s %*d %*d %*d %*s %*s %s\n",
                      &CIN, nom, prenom, service) != EOF) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &it

}
void afficher_reservationtree(GtkWidget *liste) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkTreeIter iter;
    GtkListStore *store;

    char idres[20];
    int jours;
    int mois;
    int annee;
    int duree;

    store = NULL;

    FILE *f;
    store = gtk_tree_view_get_model(liste);

    // Initialiser les colonnes si elles n'existent pas
    if (store == NULL) {
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID Réservation", renderer, "text", EIDRES, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Jour", renderer, "text", EJOURS, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Mois", renderer, "text", EMOIS, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Année", renderer, "text", EANNEE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Durée (jours)", renderer, "text", EDUREE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
    }

    // Définir le modèle pour l'arbre
    store = gtk_list_store_new(ECOLUMNS, G_TYPE_STRING, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    f = fopen("reservations.txt", "r");
    if (f == NULL) {
        return;
    } else {
        // Lire le fichier et remplir le modèle GTK
        while (fscanf(f, "%s %d %d %d %d\n", idres, &jours, &mois, &annee, &duree) != EOF) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               EIDRES, idres,
                               EJOURS, jours,
                               EMOIS, mois,
                               EANNEE, annee,
                               EDUREE, duree,
                               -1);
        }
        fclose(f);

        // Appliquer le modèle à la vue
        gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
        g_object_unref(store);
    }
}
void supprimer_reservationtree(reservation r) {
    char idres[20];
    int jours, mois, annee, duree;

    reservation r2;

    FILE *f, *g;
    f = fopen("reservations.txt", "r");
    g = fopen("dumb.txt", "w");

    if (f == NULL || g == NULL) {
        return;
    } else {
        while (fscanf(f, "%s %d %d %d %d\n", r2.idres, &r2.jours, &r2.mois, &r2.annee, &r2.duree) != EOF) {
            // Conserver uniquement les lignes qui ne correspondent pas à la réservation à supprimer
            if (strcmp(r.idres, r2.idres) != 0 || r.jours != r2.jours || r.mois != r2.mois || r.annee != r2.annee || r.duree != r2.duree) {
                fprintf(g, "%s %d %d %d %d\n", r2.idres, r2.jours, r2.mois, r2.annee, r2.duree);
            }
        }
    }

    fclose(f);
    fclose(g);

    // Remplacer le fichier d'origine par le fichier temporaire
    remove("reservations.txt");
    rename("dumb.txt", "reservations.txt");
}

