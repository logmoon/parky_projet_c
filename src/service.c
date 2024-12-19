#include <stdio.h>
#include <string.h>
#include "service.h"
#include <gtk/gtk.h>

void ajouter_service_parking(int disponibilite, char msg[]) {
    if (disponibilite == 0) { // Par défaut
        strcpy(msg, "Indéfini");
    } else if (disponibilite == 1) {
        strcpy(msg, "Disponible");
    } else if (disponibilite == 2) {
        strcpy(msg, "Non disponible");
    }
}

int ajouter(char *filename, service s) {
    FILE *f = fopen(filename, "a");
    if (f != NULL) {
        fprintf(f, "%d %s %s %d %s\n", s.id, s.nom, s.description, s.prix, s.disponibilite);
        fclose(f);
        return 1;
    }
    return 0;
}
int modifier(char *filename, int id, service nouv) {
    int tr = 0;
    service s;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &s.id, s.nom, s.description, &s.prix, s.disponibilite) != EOF) {
            if (s.id == id) {
                fprintf(f2, "%d %s %s %d %s\n", nouv.id, nouv.nom, nouv.description, nouv.prix, nouv.disponibilite);
                tr = 1;
            } else {
                fprintf(f2, "%d %s %s %d %s\n", s.id, s.nom, s.description, s.prix, s.disponibilite);
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp.txt", filename);
    }
    return tr;
}

int supprimer(char *filename, int id) {
    int tr = 0;
    service s;
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("temp.txt", "w");

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &s.id, s.nom, s.description, &s.prix, s.disponibilite) != EOF) {
            if (s.id != id) {
                fprintf(f2, "%d %s %s %d %s\n", s.id, s.nom, s.description, s.prix, s.disponibilite);
            } else {
                tr = 1;
            }
        }
        fclose(f);
        fclose(f2);
        remove(filename);
        rename("temp.txt", filename);
    }
    return tr;
}

service chercher(char *filename, int id) {
    service s;
    s.id = -1;
    FILE *f = fopen(filename, "r");

    if (f != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &s.id, s.nom, s.description, &s.prix, s.disponibilite) != EOF) {
            if (s.id == id) {
                fclose(f);
                return s;
            }
        }
        fclose(f);
    }
    return s;
}

int afficher_services(char *filename) {
    service s;
    FILE *f = fopen(filename, "r");

    if (f != NULL) {
        while (fscanf(f, "%d %s %s %d %s\n", &s.id, s.nom, s.description, &s.prix, s.disponibilite) != EOF) {
            printf("ID: %d\nNom: %s\nDescription: %s\nPrix: %d\nDisponibilité: %s\n\n", s.id, s.nom, s.description, s.prix, s.disponibilite);
        }
        fclose(f);
        return 1;  // success
    }
    return 0;  // failure if file could not be opened
}

int affecter_service_reservation(char *reservation_file, int reservation_id, int service_id) {
    FILE *f = fopen(reservation_file, "r");
    FILE *f2 = fopen("temp_reservations.txt", "w");
    int tr = 0;
    int res_id, srv_id;
    char citizen_name[50];

    if (f != NULL && f2 != NULL) {
        while (fscanf(f, "%d %s %d\n", &res_id, citizen_name, &srv_id) != EOF) {
            if (res_id == reservation_id) {
                srv_id = service_id; // Assign the new service to the reservation
                tr = 1;
            }
            fprintf(f2, "%d %s %d\n", res_id, citizen_name, srv_id);
        }
        fclose(f);
        fclose(f2);
        remove(reservation_file);
        rename("temp_reservations.txt", reservation_file);
    }
    return tr;
}
