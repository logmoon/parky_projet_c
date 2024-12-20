#include <stdio.h>
#include <stdlib.h>
#include "agent.h"
#include <string.h>

#define MAX_HORAIRE_LEN 200
enum A {
    CIN_AGENT,
    NOM,
    PRENOM,
    EMAIL,
    SEXE,
    SERVICE,
    ID_PARKING,
    NUM_TEL,
    ADRESSE,
    COLUMNS
};
enum B {
    ID_PARKINGR,
    DATER,
    DEPART,
    DUREE,
    SERVICE_CHOISI,
    EMPLACEMENT,
    MATRICULE,
    RCOLUMNS
};
// Fonction pour définir le sexe

// Ajout du sexe dans le fichier agent.txt
//void ajout_sexe(int sexe, char *msg) {
   // char sexe_msg[50];
    
    //if (sexe == 1) 
       // strcpy(sexe_msg, "Homme");
    // else if (sexe == 2) 
      //  strcpy(sexe_msg, "Femme");
    

   
//}




//void ajout_sexe(int sexe , char *msg) {
 //   if (sexe == 1) 
      //  strcpy(msg, "Homme");
  //  else if (sexe == 2) 
      //  strcpy(msg, "Femme");
//}

// Fonction pour ajouter un agent dans un fichier
void ajouter_agent(char *file_agent, agent agt) {
    // Ouvrir le fichier en mode ajout
    FILE *f = fopen(file_agent, "a");
    if (f!=NULL)
{
   
        // Écrire les informations de l'agent dans le fichier
        fprintf(f, "%s %s %s %s %s %s %s %s %s\n", 
                agt.CIN, 
                agt.nom, 
                agt.prenom, 
                agt.email, 
                agt.sexe, 
                agt.service, 
                agt.ID_Parking, 
                agt.num_tel, 
                agt.adresse);
        fclose(f); // Fermer le fichier
    } else {
        // Si le fichier ne peut pas être ouvert, afficher un message d'erreur
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", file_agent);
    }
}


int modifier_agent(char *file_agent, char *CIN, agent n_file_agent) {
    int found = 0;
    agent agt;
    
    
    FILE *f = fopen(file_agent, "r");
    
    FILE *f2 = fopen("n_file_agent.txt", "w");

    if (f != NULL && f2 != NULL) {
      
        while (fscanf(f, "%s %s %s %s %s %s %s %s %s\n", agt.CIN, agt.nom, agt.prenom, agt.email, agt.sexe, agt.service, agt.ID_Parking, agt.num_tel, agt.adresse) != EOF) {
            
            if (strcmp(agt.CIN, CIN) == 0) {
                fprintf(f2, "%s %s %s %s %s %s %s %s %s\n", n_file_agent.CIN, n_file_agent.nom, n_file_agent.prenom, n_file_agent.email, n_file_agent.sexe, n_file_agent.service, n_file_agent.ID_Parking, n_file_agent.num_tel, n_file_agent.adresse);
                found = 1;
            } else {
                
                fprintf(f2, "%s %s %s %s %s %s %s %s %s\n", agt.CIN, agt.nom, agt.prenom, agt.email, agt.sexe, agt.service, agt.ID_Parking, agt.num_tel, agt.adresse);
            }
        }
        
        fclose(f);
        fclose(f2);

       
        if (found) {
            remove(file_agent);
            rename("n_file_agent.txt", file_agent);   
        } else {
            
            remove("n_file_agent.txt");
        }
    }
    
    return found;  
}

//Fonction chercher :
agent chercher_Agent(char *filename,  char *CIN){	

    agent agt;
    FILE  *f=fopen(filename, "r");
    if(f!=NULL)
    {
        while(fscanf(f,"%s %s %s %s %s %s %s %s %s\n", agt.CIN, agt.nom, agt.prenom, agt.email, agt.sexe, agt.service, agt.ID_Parking,agt.num_tel,agt.adresse)!=EOF)
        {
            if( strcmp(agt.CIN,CIN)==0){
                fclose(f);
		return agt;
        }
    }
    fclose(f);
}
        strcpy(agt.CIN,"-1");
    
	return agt;
}

//Fonction reservation : 

void reservation(char *filename, char ID_Parking[]) {
char ID_Parkingr[100], dater[50], Depart[20], duree[20], service_choisi[20], emplacement[20], matricule[20];


	FILE *f = fopen(filename,"r");
	FILE *f2 = fopen("reservation.txt","w");

	if (f!= NULL &&f2!= NULL) {
		while (fscanf (f,"%s %s %s %s %s %s %s\n ",ID_Parkingr,dater,Depart,duree,service_choisi,emplacement,matricule)!=EOF){
			if(strcmp(dater,dater)==0) {
				fprintf(f2,"%s %s %s %s %s %s %s\n ",ID_Parkingr,dater,Depart,duree,service_choisi,emplacement,matricule);
}
}
		fclose(f);
		fclose(f2);
}


}

//Fonction afficher :
void afficherAgents(GtkWidget *liste, char *filename) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;
    GtkTreeIter iter;
    agent ag;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(liste)));

    if (store == NULL) {
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", CIN_AGENT, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("nom", renderer, "text", NOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("prenom", renderer, "text", PRENOM, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("email", renderer, "text", EMAIL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("sexe", renderer, "text", SEXE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("service", renderer, "text", SERVICE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID_Parking", renderer, "text", ID_PARKING, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("num_tel", renderer, "text", NUM_TEL, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("adresse", renderer, "text", ADRESSE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

       
        store = gtk_list_store_new(COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

       
        FILE *f = fopen(filename, "r");
        if (f != NULL) {

            while (fscanf(f, "%s %s %s %s %s %s %s %s %s\n", ag.CIN, ag.nom, ag.prenom, ag.email, ag.sexe, ag.service, ag.ID_Parking, ag.num_tel, ag.adresse) != EOF) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                    CIN_AGENT, ag.CIN,
                    NOM, ag.nom,
                    PRENOM, ag.prenom,
                    EMAIL, ag.email,
                    SEXE, ag.sexe,
                    SERVICE, ag.service,
                    ID_PARKING, ag.ID_Parking,
                    NUM_TEL, ag.num_tel,
                    ADRESSE, ag.adresse,
                    -1);
            }
            fclose(f);
        }
    }

   
    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store);
}

/////////////////////
int supprimer_agent(char *file_agent, char CIN[]) {
    int tr = 0;
    agent agt;
    
    FILE *f = fopen(file_agent, "r");
    FILE *f2 = fopen("n_agent.txt", "w");
    
    if (f == NULL || f2 == NULL) {
        printf("Erreur d'ouverture des fichiers.\n");
        return tr;
    }
    
    while (fscanf(f, "%s %s %s %s %s %s %s %s %s\n", agt.CIN, agt.nom, agt.prenom, agt.email, agt.sexe, agt.service, agt.ID_Parking,agt.num_tel,agt.adresse) != EOF) {
        if (strcmp(agt.CIN, CIN) == 0) {
            
            tr = 1;  
        } else {
            
            fprintf(f2, "%s %s %s %s %s %s %s %s %s\n", agt.CIN, agt.nom, agt.prenom, agt.email, agt.sexe, agt.service, agt.ID_Parking,agt.num_tel,agt.adresse);
        }
    }
    
    fclose(f);
    fclose(f2);
    
    if (tr) {
        remove(file_agent);  
        rename("n_agent.txt", file_agent);  
    } else {
        remove("n_agent.txt");
        printf("Agent avec CIN %s non trouvé.\n", CIN);
    }
return tr;
}
//////////////////////////////////


// Fonction pour vérifier et nettoyer les chaînes non-UTF-8
void afficherReservations(GtkWidget *liste, char *filename) {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;
    GtkTreeIter iter;
    reservations res;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(liste)));

    if (store == NULL) {
        
        

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("dater", renderer, "text", DATER, NULL);

        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);
renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID_Parkingr", renderer, "text", ID_PARKINGR, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Depart", renderer, "text", DEPART, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("duree", renderer, "text", DUREE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("service_choisi", renderer, "text", SERVICE_CHOISI, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("emplacement", renderer, "text", EMPLACEMENT, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("matricule", renderer, "text", MATRICULE, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(liste), column);

       
        store = gtk_list_store_new(RCOLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

        
        FILE *f = fopen(filename, "r");
        if (f != NULL) {
           
            while (fscanf(f, "%s %s %s %s %s %s %s\n", res.ID_Parkingr, res.dater, res.Depart, res.duree, res.service_choisi, res.emplacement, res.matricule) != EOF) {
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                    ID_PARKINGR, res.ID_Parkingr,
                    DATER, res.dater,
                    DEPART, res.Depart,
                    DUREE, res.duree,
                    SERVICE_CHOISI, res.service_choisi,
                    EMPLACEMENT, res.emplacement,
                    MATRICULE, res.matricule,
                    -1);
            }
            fclose(f);
        }
    }

    
    gtk_tree_view_set_model(GTK_TREE_VIEW(liste), GTK_TREE_MODEL(store));
    g_object_unref(store);
}
