#include "ajouter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>



void ajouterAvis(AvisReclamationDetails avis, const char *fichier) {
    FILE *fp = fopen(fichier, "a");
    if (fp != NULL) {
        fprintf(fp, "%d|%s|%s|%s|%.2f|%02d/%02d/%04d|%s|%d\n", 
                avis.id_client, avis.contenu, avis.type_demande,
                
                avis.categorie, avis.note_satisfaction, 
                avis.date_reclamation.jour, avis.date_reclamation.mois, 
                avis.date_reclamation.annee, avis.urgent, avis.id_parking);
        fclose(fp);
    }
}

void modifierAvis(int id_client, AvisReclamationDetails avis, const char *fichier) {
    FILE *fp = fopen(fichier, "r");
    FILE *temp = fopen("eya.txt", "w");
    AvisReclamationDetails avis_temp;

    if (fp != NULL && temp != NULL) {
        while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%s|%d\n", 
                      &avis_temp.id_client, avis_temp.contenu, avis_temp.type_demande,
                     
                      avis_temp.categorie, &avis_temp.note_satisfaction, 
                      &avis_temp.date_reclamation.jour, &avis_temp.date_reclamation.mois, 
                      &avis_temp.date_reclamation.annee, avis_temp.urgent, &avis_temp.id_parking) != EOF) {
            if (avis_temp.id_client == id_client) {
                
                avis_temp = avis; 
                strcpy(avis_temp.type_demande, avis.type_demande); 
            }
            fprintf(temp, "%d|%s|%s|%s|%.2f|%02d/%02d/%04d|%s|%d\n", 
                    avis_temp.id_client, avis_temp.contenu, avis_temp.type_demande,
                   
                    avis_temp.categorie, avis_temp.note_satisfaction, 
                    avis_temp.date_reclamation.jour, avis_temp.date_reclamation.mois, 
                    avis_temp.date_reclamation.annee, avis_temp.urgent, avis_temp.id_parking);
        }
        fclose(fp);
        fclose(temp);
        remove(fichier);
        rename("eya.txt", fichier);  
    }
}

void supprimerAvis(int id_client, int numero_reclamation, const char *fichier) {
    FILE *fp = fopen(fichier, "r");
    FILE *temp = fopen("temp.txt", "w");
    AvisReclamationDetails avis_temp;

    if (fp != NULL && temp != NULL) {
        while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%s|%d\n", 
                      &avis_temp.id_client, avis_temp.contenu, avis_temp.type_demande,
                      avis_temp.categorie, &avis_temp.note_satisfaction, 
                      &avis_temp.date_reclamation.jour, &avis_temp.date_reclamation.mois, 
                      &avis_temp.date_reclamation.annee, avis_temp.urgent, &avis_temp.id_parking) != EOF) {
            // Comparaison : ignorer l'entrée si l'ID client correspond
            if (!(avis_temp.id_client == id_client)) {
                fprintf(temp, "%d|%s|%s|%s|%.2f|%02d/%02d/%04d|%s|%d\n", 
                        avis_temp.id_client, avis_temp.contenu, avis_temp.type_demande,
                        avis_temp.categorie, avis_temp.note_satisfaction, 
                        avis_temp.date_reclamation.jour, avis_temp.date_reclamation.mois, 
                        avis_temp.date_reclamation.annee, avis_temp.urgent, avis_temp.id_parking);
            }
        }
        fclose(fp);
        fclose(temp);
        remove(fichier);
        rename("temp.txt", fichier); 
    }
}




void afficherAvis(const char *fichier_source) {
    FILE *fp = fopen(fichier_source, "r");
    AvisReclamationDetails avis_temp;
    int count = 0;

    if (fp != NULL) {
        printf("\n=== Liste des Avis et Réclamations ===\n");
        
        while (fscanf(fp, "%d|%[^|]|%[^|]|%d|%[^|]|%f|%d/%d/%d|%s|%d\n", 
                      &avis_temp.id_client, avis_temp.contenu, avis_temp.type_demande, 
                      &avis_temp.numero_reclamation, avis_temp.categorie, 
                      &avis_temp.note_satisfaction, &avis_temp.date_reclamation.jour, 
                      &avis_temp.date_reclamation.mois, &avis_temp.date_reclamation.annee, 
                      avis_temp.urgent, &avis_temp.id_parking) != EOF) {
            count++;
            printf("\nAvis ID Client : %d\n", avis_temp.id_client);
            printf("Contenu de la réclamation : %s\n", avis_temp.contenu);
            printf("Type de demande : %s\n", avis_temp.type_demande);
            printf("Numéro de réclamation : %d\n", avis_temp.numero_reclamation);
            printf("Catégorie : %s\n", avis_temp.categorie);
            printf("Note de satisfaction : %.2f\n", avis_temp.note_satisfaction);
            printf("Date de réclamation : %02d/%02d/%04d\n", avis_temp.date_reclamation.jour,
                   avis_temp.date_reclamation.mois, avis_temp.date_reclamation.annee);
            printf("Urgent : %s\n", avis_temp.urgent);
            printf("ID Parking : %d\n", avis_temp.id_parking);
            printf("--------------------------------------------\n");
        }
        fclose(fp);
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    } 
    if (count == 0) {
        printf("Aucun avis trouvé dans le fichier.\n");
        return;
    }
    printf("Avis affichés avec succés.\n"); 
}


int avisExiste(int id_client, const char *fichierAvis) {
    FILE *fichier = fopen(fichierAvis, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0; 
    }

    AvisReclamationDetails avis;
    while (fscanf(fichier, "%d|%[^|]|%[^|]|%d|%[^|]|%f|%d/%d/%d|%s|%d\n", 
                  &avis.id_client, avis.contenu, avis.type_demande,
                  &avis.numero_reclamation, avis.categorie, 
                  &avis.note_satisfaction, &avis.date_reclamation.jour, 
                  &avis.date_reclamation.mois, &avis.date_reclamation.annee, avis.urgent, &avis.id_parking) != EOF) {
        if (avis.id_client == id_client) {
            fclose(fichier);
            return 1; 
        }
    }

    fclose(fichier);
    return 0; 
}

int numeroAvisRecSuivante(const char *type, const char *fichierAvis) {
    FILE *fichier = fopen(fichierAvis, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1; 
    }

    int nbr = 0;
    AvisReclamationDetails avis;
    while (fscanf(fichier, "%d|%[^|]|%[^|]|%d|%[^|]|%f|%d/%d/%d|%s|%d\n", 
                  &avis.id_client, avis.contenu, avis.type_demande,
                  &avis.numero_reclamation, avis.categorie, 
                  &avis.note_satisfaction, &avis.date_reclamation.jour, 
                  &avis.date_reclamation.mois, &avis.date_reclamation.annee, avis.urgent, &avis.id_parking) != EOF) {       
        if (strcmp(avis.type_demande, type) == 0) { 
            nbr++;
        }
    }

    fclose(fichier);
    return nbr+1; 
}

 
AvisReclamationDetails cherche(char *filename, int id) {
    FILE *fp = fopen(filename, "r");
    AvisReclamationDetails avis;

    if (fp != NULL) {
        while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%s|%d\n",
                &avis.id_client, avis.contenu, avis.type_demande,
                avis.categorie,
                &avis.note_satisfaction, &avis.date_reclamation.jour,
                &avis.date_reclamation.mois, &avis.date_reclamation.annee, avis.urgent, &avis.id_parking) != EOF) {
           
            // Debug
            printf("ID lu: '%d'\n", avis.id_client);
            printf("Résultat de strcmp: %d\n", (avis.id_client == id) ? 0 : 1);
            if (avis.id_client == id) {
                fprintf(fp, "%d|%s|%s|%s|%.2f|%02d/%02d/%04d|%s|%d\n",
                avis.id_client, avis.contenu, avis.type_demande,
                avis.categorie, avis.note_satisfaction,
                avis.date_reclamation.jour, avis.date_reclamation.mois,
                avis.date_reclamation.annee, avis.urgent, avis.id_parking);
                fclose(fp);
                return avis; // Retour immédiat si l'ID est trouvé
            }
        }
        fclose(fp);
        printf("ID non trouvé dans le fichier.\n");
    } else {
        printf("Erreur d'ouverture du fichier : %s\n", filename);
    }

    return avis;  // Retourne un AvisReclamationDetails initialisé à des valeurs par défaut si non trouvé
}

void modifier_reserv(char *filename, int id, AvisReclamationDetails avis_temp) {
    FILE *fp = fopen(filename, "r");
    FILE *fp2 = fopen("eya.txt", "w");
    AvisReclamationDetails avis;
   
    int tr = 0;

    if (fp != NULL && fp2 != NULL) {
        while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%s|%d\n",
                &avis.id_client, avis.contenu, avis.type_demande,
                avis.categorie,
                &avis.note_satisfaction, &avis.date_reclamation.jour,
                &avis.date_reclamation.mois, &avis.date_reclamation.annee, avis.urgent, &avis.id_parking) != EOF) {
           
            // Debug
            printf("ID lu: '%d'\n", avis.id_client);
            printf("Résultat de comparaison: %d\n", avis.id_client - id);

            if (avis.id_client == id) {
                fprintf(fp2, "%d|%s|%s|%s|%.2f|%02d/%02d/%04d|%s|%d\n",
                 avis.id_client, avis_temp.contenu, avis_temp.type_demande,
                         
                        avis_temp.categorie, avis_temp.note_satisfaction, 
                        avis_temp.date_reclamation.jour, avis_temp.date_reclamation.mois, 
                        avis_temp.date_reclamation.annee, avis_temp.urgent, avis_temp.id_parking);
                tr = 1;
            } else {
                // Garder la réservation inchangée
                fprintf(fp2, "%d|%s|%s|%s|%.2f|%02d/%02d/%04d|%s|%d\n",
                avis.id_client, avis.contenu, avis.type_demande,
                avis.categorie, avis.note_satisfaction,
                avis.date_reclamation.jour, avis.date_reclamation.mois,
                avis.date_reclamation.annee, avis.urgent, avis.id_parking);
            }
        }
        fclose(fp);
        fclose(fp2);

        if (tr == 1) {
            if (remove(filename) != 0) {
                perror("Erreur lors de la suppression du fichier original");
            } else if (rename("eya.txt", filename) != 0) {
                perror("Erreur lors du renommage du fichier temporaire");
            } else {
                printf("Modification effectuée avec succès\n");
            }
        } else {
            remove("eya.txt");
            printf("ID non trouvé dans le fichier\n");
        }
    } else {
        printf("Erreur d'ouverture des fichiers\n");
        if (fp != NULL) fclose(fp);
        if (fp2 != NULL) fclose(fp2);
    }
}


void afficher_avis(GtkTreeView *treeview, const char *filename) {
   // Ouverture du fichier
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        g_print("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        return;
    }

    // Déclaration des variables
    AvisReclamationDetails avis; // Structure contenant toutes les données nécessaires
    GtkListStore *list_store;
    GtkTreeIter iter;

    // Création d'un nouveau modèle avec les colonnes nécessaires
    list_store = gtk_list_store_new(10,
                                    G_TYPE_INT,     // Colonne 0 : ID client
                                    G_TYPE_STRING,  // Colonne 1 : Contenu
                                    G_TYPE_STRING,  // Colonne 2 : Type demande
                                    G_TYPE_STRING,  // Colonne 3 : Catégorie
                                    G_TYPE_FLOAT,   // Colonne 4 : Note satisfaction
                                    G_TYPE_INT,     // Colonne 5 : Jour
                                    G_TYPE_INT,     // Colonne 6 : Mois
                                    G_TYPE_INT,     // Colonne 7 : Année
                                    G_TYPE_STRING,  // Colonne 8 : Urgent
                                    G_TYPE_INT);    // Colonne 9 : ID parking

    // Lecture du fichier ligne par ligne
    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%[^|]|%d\n",
                  &avis.id_client, avis.contenu, avis.type_demande, avis.categorie,
                  &avis.note_satisfaction,
                  &avis.date_reclamation.jour, &avis.date_reclamation.mois, &avis.date_reclamation.annee,
                  avis.urgent, &avis.id_parking) != EOF) {
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
                           0, avis.id_client,
                           1, avis.contenu,
                           2, avis.type_demande,
                           3, avis.categorie,
                           4, avis.note_satisfaction,
                           5, avis.date_reclamation.jour,
                           6, avis.date_reclamation.mois,
                           7, avis.date_reclamation.annee,
                           8, avis.urgent,
                           9, avis.id_parking,
                           -1);
    }

    fclose(fp); // Fermeture du fichier

    // Nettoyage de l'ancien modèle (s'il existe déjà)
    GtkTreeModel *old_model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (old_model) {
        g_object_unref(old_model);
    }

    // Associer le nouveau modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));

    // Ajouter les colonnes si elles n'existent pas déjà
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

        // Création et ajout des colonnes
        const char *column_titles[] = {
            "ID Client", "Contenu", "Type Demande", "Catégorie",
            "Note Satisfaction", "Jour", "Mois", "Année", "Urgent", "ID Parking"
        };

        for (int i = 0; i < 10; i++) {
            GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(column_titles[i], renderer, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
        }
    }

    // Libération de la mémoire associée au modèle
    g_object_unref(list_store);
}

void affichage_trie(GtkTreeView *treeview, const char *filename) {
    AvisReclamationDetails avis;

    // Ouverture du fichier
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        g_print("Erreur : Impossible d'ouvrir le fichier %s.\n", filename);
        return;
    }

    // Déclaration d'une liste pour stocker les avis
    GArray *avis_list = g_array_new(FALSE, FALSE, sizeof(AvisReclamationDetails));

    // Lecture du fichier ligne par ligne et stockage des données dans la liste
    while (fscanf(fp, "%d|%[^|]|%[^|]|%[^|]|%f|%d/%d/%d|%[^|]|%d\n",
                  &avis.id_client, avis.contenu, avis.type_demande, avis.categorie,
                  &avis.note_satisfaction,
                  &avis.date_reclamation.jour, &avis.date_reclamation.mois, &avis.date_reclamation.annee,
                  avis.urgent, &avis.id_parking) != EOF) {
        g_array_append_val(avis_list, avis);
    }

    fclose(fp); // Fermeture du fichier

    // Tri des avis par note de satisfaction (décroissant)
    g_array_sort(avis_list, (GCompareFunc)compare_avis);

    // Création d'un nouveau modèle avec les colonnes nécessaires
    GtkListStore *list_store = gtk_list_store_new(10,
                                    G_TYPE_INT,     // Colonne 0 : ID client
                                    G_TYPE_STRING,  // Colonne 1 : Contenu
                                    G_TYPE_STRING,  // Colonne 2 : Type demande
                                    G_TYPE_STRING,  // Colonne 3 : Catégorie
                                    G_TYPE_FLOAT,   // Colonne 4 : Note satisfaction
                                    G_TYPE_INT,     // Colonne 5 : Jour
                                    G_TYPE_INT,     // Colonne 6 : Mois
                                    G_TYPE_INT,     // Colonne 7 : Année
                                    G_TYPE_STRING,  // Colonne 8 : Urgent
                                    G_TYPE_INT);    // Colonne 9 : ID parking

    GtkTreeIter iter;

    // Ajout des données triées dans le modèle
    for (guint i = 0; i < avis_list->len; i++) {
        AvisReclamationDetails *current_avis = &g_array_index(avis_list, AvisReclamationDetails, i);
        gtk_list_store_append(list_store, &iter);
        gtk_list_store_set(list_store, &iter,
                           0, current_avis->id_client,
                           1, current_avis->contenu,
                           2, current_avis->type_demande,
                           3, current_avis->categorie,
                           4, current_avis->note_satisfaction,
                           5, current_avis->date_reclamation.jour,
                           6, current_avis->date_reclamation.mois,
                           7, current_avis->date_reclamation.annee,
                           8, current_avis->urgent,
                           9, current_avis->id_parking,
                           -1);
    }

    g_array_free(avis_list, TRUE); // Libération de la mémoire de la liste

    // Nettoyage de l'ancien modèle (s'il existe déjà)
    GtkTreeModel *old_model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (old_model) {
        g_object_unref(old_model);
    }

    // Associer le nouveau modèle au TreeView
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(list_store));

    // Ajouter les colonnes si elles n'existent pas déjà
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == 0) {
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();

        // Création et ajout des colonnes
        const char *column_titles[] = {
            "ID Client", "Contenu", "Type Demande", "Catégorie",
            "Note Satisfaction", "Jour", "Mois", "Année", "Urgent", "ID Parking"
        };

        for (int i = 0; i < 10; i++) {
            GtkTreeViewColumn *col = gtk_tree_view_column_new_with_attributes(column_titles[i], renderer, "text", i, NULL);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
        }
    }

    // Libération de la mémoire associée au modèle
    g_object_unref(list_store);
}

gint compare_avis(const void *a, const void *b) {
    const AvisReclamationDetails *avis_a = (const AvisReclamationDetails *)a;
    const AvisReclamationDetails *avis_b = (const AvisReclamationDetails *)b;

    // Comparer les notes de satisfaction
    if (avis_a->note_satisfaction < avis_b->note_satisfaction) {
        return 1; // Tri décroissant
    } else if (avis_a->note_satisfaction > avis_b->note_satisfaction) {
        return -1;
    } else {
        return 0; // Égalité
    }
}

