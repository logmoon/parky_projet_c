#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED
#define MAX_LENGTH 100
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
typedef struct {
    char CIN[20];
    char nom[100];
    char prenom[100];
    char email[100];
    char sexe[50];
    char service[100];
    char ID_Parking[100];
    char num_tel[20];
    char adresse[100];
} agent;

typedef struct{
	char ID_Parkingr[100];
	char dater[50];
	char Depart[20];
	char duree[20];
	char service_choisi[20];
	char emplacement[20];
	char matricule[20];
}reservations;


void ajout_sexe(int sexe , char *msg);



void ajouter_agent(char*file_agent,agent agt);


int modifier_agent(char *file_agent, char *CIN, agent n_file_agent);

agent chercher_Agent(char *filename,  char *CIN);

int supprimer_agent(char *file_agent, char CIN[]);

void afficherAgents(GtkWidget *liste, char *filename);

void afficherReservations(GtkWidget *liste, char *filename);
void reservation(char *filename, char ID_Parking[]) ;

//int ajouter_utilisateur(char *file_utilisateur, utilisateur user);
//int verifier_connexion(char *file_utilisateur, char *Nom_utilisateur, char *mot_passe);

//int modifier_agent(char* file_agent, char CIN[], agent n_file_agent);
//int supprimer_agent(char *file_agent, char CIN[]);
//agent chercher(char * filename, char *CIN);
//void afficher_agent(char*file_agent,agent agt);
//void afficher_reservations(char *file_reservation, char date_recherche[]);

#endif

