#ifndef RESERVATION_H
#define RESERVATION_H

#include <gtk/gtk.h>
/*
// Structure pour stocker la date
typedef struct {
    int jour;
    int mois;
    int annee;
} Date;
// Structure pour stocker l'heure
typedef struct {
    int heure;
    int minute;
} Heure;

typedef struct {
    int id_client;
    char type[20];
    int id_parking;
    Date date_entree;
    Heure heure_entree;
    char duree[10];
    int entretient_c;
    int services_c;
    int priseencharge_c;
}Reservation;*/


typedef struct {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct {
    int heure;
    int minute;
} Heure;

typedef struct {
    int id_client;
    Date date_entree;
    Heure heure_entree;
    char duree[50];
    char id_parking[50];
    char type[50];
    int entretient_c;
    int services_c;
    int priseencharge_c;
} Reservation;


// Énumération pour les colonnes du GtkTreeView
 /*enum { COL_ID_CLIENT,
 COL_DATE_ENTREE,
 COL_HEURE_ENTREE,
 COL_DUREE,
 COL_TYPE,
 COL_PARKING,
 COL_SERVICES,
 COL_ENTRETIEN,
 COL_PRISEENCHARGE,
 NUM_COLS };*/
typedef struct {
    int heures;
    int minutes;
} Temps;
// Structure pour stocker les informations de parking
typedef struct {
    int Id_agent;
    int id_parking;
    int c_postalp;
    char villep[20];
    char ruep[20];
    char nature[20];     // chaîne de caractères
    int PaiementTPE;     // 1 oui, 0 non
    int PaiementCheques; // 1 oui, 0 non
    int nb_places_std;
    int nb_places_std_dispo;
    int nb_places_elec;
    int nb_places_elec_dispo;
    Temps lundi_o;
    Temps lundi_f;
    Temps mardi_o;
    Temps mardi_f;
    Temps mercredi_o;
    Temps mercredi_f;
    Temps jeudi_o;
    Temps jeudi_f;
    Temps vendredi_o;
    Temps vendredi_f;
    Temps samedi_o;
    Temps samedi_f;
    Temps dimanche_o;
    Temps dimanche_f;
    int o_dimanche;           // 1 --> ouvert dimanche
    int f_dimanche;           // 1 --> fermé dimanche
    int abo_mensuel_std_dispo; // 1 oui, 0 non
    int abo_mensuel_elec_dispo; // 1 oui, 0 non
    float tarif_h_std;
    float tarif_j_std;
    float tarif_abo_std;
    float tarif_h_elec;
    float tarif_j_elec;
    float tarif_abo_elec;
} Parking;


// Déclarations des fonctions
void ajouter_Reservation(Reservation R);
void Afficher_Reservation(GtkWidget *treeview);

#endif // RESERVATION_H

