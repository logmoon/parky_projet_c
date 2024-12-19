#include <gtk/gtk.h>

typedef struct {
    int jour;
    int mois;
    int annee;
} DateReclamation;

typedef struct {
    int id_client;
    char contenu[500];
    char type_demande[100];
    int numero_reclamation;
    char categorie[100];
    float note_satisfaction;
    DateReclamation date_reclamation; 
    char urgent[20];  
    int id_parking;  
} AvisReclamationDetails;

typedef struct {
    int id_parking;
    float note;
    int nombre_avis;
} Parking; 
 
void ajouterAvis(AvisReclamationDetails avis, const char *fichier);
void modifierAvis(int id_client, AvisReclamationDetails avis, const char *fichier);
void supprimerAvis(int id_client, int numero_reclamation, const char *fichier);
int compareParking(const void *a, const void *b);
void afficherParkings(const char *fichier);
void afficherAvis(const char *fichier_source);
int avisExiste(int id_client, const char *fichierAvis); 
int numeroAvisRecSuivante(const char *type, const char *fichierAvis); 
AvisReclamationDetails cherche(char *filename, int id);
void modifier_reserv(char *filename, int id, AvisReclamationDetails avis_temp);
void afficher_avis(GtkTreeView *treeview, const char *filename);
void affichage_trie(GtkTreeView *treeview, const char *filename);
gint compare_avis(const void *a, const void *b);

