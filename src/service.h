#ifndef SERVICE_H
#define SERVICE_H

typedef struct {
    int id;
    char nom[50];
    char description[100];
    int prix;
    char disponibilite[20];
} service;

// Fonction pour gérer la disponibilité
void ajouter_service_parking(int disponibilite, char msg[]);

int ajouter(char *filename, service s);
int modifier(char *filename, int id, service nouv);
int supprimer(char *filename, int id);
service chercher(char *filename, int id);
int afficher_services(char *filename);  // Corrected return type
int affecter_service_reservation(char *reservation_file, int reservation_id, int service_id);

#endif
