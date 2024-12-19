#include "reservation.h"
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "callbacks.h"


// Ajouter reservation

/*void ajouter_Reservation(Reservation R)
{
    FILE *f;
    f = fopen("Reservation.txt", "a+");
    if (f != NULL)
    {
  fprintf(f, "%d %s %d %d %d %d %d %d %s %d %d %d \n", R.id_client,R.type,R.id_parking,R.date_entree.jour,R.date_entree.mois,R.date_entree.annee,R.heure_entree.heure,R.heure_entree.minute,R.duree,R.entretient_c,R.services_c,R.priseencharge_c);
        
        fclose(f);
    }
}*/


void ajouter_Reservation(Reservation R) {
    // Exemple d'ajout d'une réservation dans un fichier
    FILE *f = fopen("reservations.txt", "a+");
    if (f != NULL) {
        fprintf(f, "%d %d/%d/%d %d:%d %s %s %s %d %d %d\n",
                R.id_client,
                R.date_entree.jour, R.date_entree.mois, R.date_entree.annee,
                R.heure_entree.heure, R.heure_entree.minute,
                R.duree, R.id_parking, R.type,
                R.entretient_c, R.services_c, R.priseencharge_c);
        fclose(f);
    }
}



// Fonction pour afficher les réservations dans le TreeView


void Afficher_Reservation(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeViewColumn *column;
    GtkCellRenderer *renderer;
    FILE *f;
    Reservation r;

    // Créer le modèle de données
    store = gtk_list_store_new(9, 
        G_TYPE_INT, // ID client
        G_TYPE_STRING, // Date entrée
        G_TYPE_STRING, // Heure entrée
        G_TYPE_STRING, // Duree
        G_TYPE_STRING, // Type
        G_TYPE_STRING, // Parking
        G_TYPE_INT,  //services
        G_TYPE_INT ,// Entretien
        G_TYPE_INT //priseencharge
    );
    


//test
//if (store == NULL)
    //{
        

if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == NULL) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        // Ajouter les colonnes
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID Client", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Date Entrée", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Heure Entrée", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Durée", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Type", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Parking", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Services", renderer, "text", 6, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Entretien", renderer, "text", 7, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Prise en charge", renderer, "text", 8, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }

//}
//test
    // Vérifier si le fichier de réservations existe
    f = fopen("reservations.txt", "r");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    // Lire et ajouter au GtkListStore
   /* while (fscanf(f, "%d %d/%d/%d %d:%d %s %s %s %s %s %d %d %d\n",
            &r.id_client, &r.date_entree.jour, &r.date_entree.mois, &r.date_entree.annee,
            &r.heure_entree.heure, &r.heure_entree.minute, r.duree, r.type, r.id_parking, 
           &r.services_c, &r.entretient_c, &r.priseencharge_c) != EOF) {*/
while (fscanf(f, "%d %d/%d/%d %d:%d %s %s %s %d %d %d\n",
            &r.id_client, &r.date_entree.jour, &r.date_entree.mois, &r.date_entree.annee,
            &r.heure_entree.heure, &r.heure_entree.minute, r.duree, r.type, r.id_parking, 
           &r.services_c, &r.entretient_c, &r.priseencharge_c) != EOF) {

        char date_entree[20], heure_entree[10];

        // Formater les dates et heures
        sprintf(date_entree, "%02d/%02d/%04d", r.date_entree.jour, r.date_entree.mois, r.date_entree.annee);
        sprintf(heure_entree, "%02d:%02d", r.heure_entree.heure, r.heure_entree.minute);

        // Ajouter une ligne au modèle
        GtkTreeIter iter;
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, r.id_client,
            1, date_entree,
            2, heure_entree,
            3, r.duree,
            4, r.type,
            5, r.id_parking,
            6, r.services_c,
            7, r.entretient_c,
            8, r.priseencharge_c,
            -1);
    }

    fclose(f);

    // Vérifier si le TreeView est valide
    if (GTK_IS_TREE_VIEW(treeview)) {
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
    } else {
        g_error("Le TreeView n'est pas valide.");
    }
}




