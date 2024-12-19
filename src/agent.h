#ifndef AGENT_H_INCLUDED
#define AGENT_H_INCLUDED
#define MAX_NOM_LEN 50
#define MAX_PRENOM_LEN 50
#define MAX_CIN_LEN 8
#include <stdio.h>

typedef struct {
    int CIN;
    char nom[50];
    char prenom[50];
    char sexe[10];
    int jour_naissance;
    int mois_naissance;
    int annee_naissance;
    char email[100];
    char adresse[100];
    char service[50];
} Agent;
typedef struct {
    char idres[20];
    int jours;
    int mois;
    int annee;
    int duree;
} reservation;	

int ajouter(const char *filename, Agent agent);
int modifier(const char *filename, int CIN, Agent new_agent);
int supprimer(const char *filename, int CIN);
Agent chercher(const char *filename, int CIN);
void afficherlistedereservations(const char *filename);
void add( int ,char service[]);
int pull( int ,char service[]);
void supprimer_agttree(CIN,char service[]);
void afficher_lsttree(GtkWidget *liste,char ch[]);
void supprimer_agttree(agt);
void afficher_agttree (GtkWidget *liste);
#endif 
