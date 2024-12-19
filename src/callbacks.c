#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "windows.h"
#include "utils.h"

// ---------------------------------------------------------------------------------
// AMEN ALLAH BEN AISSA:
// ---------------------------------------------------------------------------------
#include "gerer_park.h"
int selected_parking = -1;
void
on_AABAbuttonGPAjouter_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    windows_show_window(AABAwindowAjouter);
}
void
on_AABAbuttonAjouterEnregistrer_clicked
                                        (GtkWidget       *widget,
                                        gpointer         user_data)
{
    Parking parking = {0};

    GtkWidget* name_entry = lookup_widget(widget, "AABAentryAjouterNom");
    GtkWidget* location_entry = lookup_widget(widget, "AABAtextviewAjouterLocalization");
    GtkWidget* type_combo = lookup_widget(widget, "AABAcomboboxAjouterType");
    GtkWidget* capacity_spin = lookup_widget(widget, "AABAspinbuttonNombreDePlaces");
    GtkWidget* is_24_hours_check = lookup_widget(widget, "AABAcheckbuttonAjouter247");
    GtkWidget* open_hour_combo = lookup_widget(widget, "AABAcomboboxAjouterOuv");
    GtkWidget* close_hour_combo = lookup_widget(widget, "AABAcomboboxAjouterFerm");
    GtkWidget* hourly_rate_spin = lookup_widget(widget, "AABAspinbuttonAjouterTarif");

    GtkWidget* error_label = lookup_widget(widget, "AABAlabelAjouterError");

    const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
    if (name == NULL || strcmp(name, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(error_label), "Nom ne peut pas etre vide.");
        return;
    }
    char* location = get_text_view_text(location_entry);
    if (location == NULL || strcmp(location, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(error_label), "La localisation ne peut pas etre vide.");
        return;
    }
    char *type = get_combo_box_text(type_combo);
    if (type == NULL || strcmp(type, "") == 0)
    {
        gtk_label_set_text(GTK_LABEL(error_label), "Le type ne peut pas etre vide.");
        return;
    }
    int capacity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacity_spin));
    int is_24_hours = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(is_24_hours_check));
    int open_hour = gtk_combo_box_get_active(GTK_COMBO_BOX(open_hour_combo));
    int close_hour = gtk_combo_box_get_active(GTK_COMBO_BOX(close_hour_combo));
    if (is_24_hours == 0)
    {
        if (open_hour == -1)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "L'heure d'ouverture ne peut pas etre vide.");
            return;
        }
        else if (close_hour == -1)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "L'heure de fermeture ne peut pas etre vide.");
            return;
        }
    }
    float hourly_rate = gtk_spin_button_get_value(GTK_SPIN_BUTTON(hourly_rate_spin));

    strncpy(parking.name, name, MAX_NAME_LEN - 1);
    strncpy(parking.location, location, MAX_LOCATION_LEN - 1);
    g_free(location);
    strncpy(parking.type, type, sizeof(parking.type) - 1);
    g_free(type);
    parking.capacity = capacity;
    parking.is_24_hours = is_24_hours;
    parking.open_hour = open_hour;
    parking.close_hour = close_hour;
    parking.hourly_rate = hourly_rate;
    parking.agent = -1;

    if (ajouter_park("/home/amen/Projects/AABAParky/parkings.txt", parking) == 1)
    {
        gtk_label_set_text(GTK_LABEL(error_label), "Park Ajouter!");
        windows_show_window(AABAwindowGererParking);
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(error_label), "Erreur lors de l'ajout du parking...");
    }
}
void
on_AABAbuttonAnnuler_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    windows_show_window(AABAwindowGererParking);
}


void
on_AABAcheckbuttonAjouter247_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget* open_hour_combo = lookup_widget(GTK_WIDGET(togglebutton), "AABAcomboboxAjouterOuv");
    GtkWidget* close_hour_combo = lookup_widget(GTK_WIDGET(togglebutton), "AABAcomboboxAjouterFerm");

    if (gtk_toggle_button_get_active(togglebutton) == 0)
    {
        gtk_widget_set_sensitive(open_hour_combo, TRUE);
        gtk_widget_set_sensitive(close_hour_combo, TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(open_hour_combo, FALSE);
        gtk_widget_set_sensitive(close_hour_combo, FALSE);

        gtk_combo_box_set_active(GTK_COMBO_BOX(open_hour_combo), -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(close_hour_combo), -1);
    }
}
void
on_AABAbuttonGPSupprimer_clicked       (GtkWidget       *widget,
                                        gpointer         user_data)
{
    GtkWidget* treeview = lookup_widget(widget, "AABAtreeviewGPList");
    selected_parking = get_selected_tree_view_row(treeview);
    if (selected_parking >= 0)
    {
        windows_show_window(AABAwindowSupprimer);
    }
}


void
on_AABAbuttonSupprimerOui_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
    if (selected_parking >= 0)
    {
        supprimer_park("/home/amen/Projects/AABAParky/parkings.txt", selected_parking);
        windows_show_window(AABAwindowGererParking);
    }
    else
    {
        windows_show_window(AABAwindowGererParking);
    }
}


void
on_AABAbuttonSupprimerNon_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
    windows_show_window(AABAwindowGererParking);
}


void
on_AABAwindowSupprimer_map             (GtkWidget       *widget,
                                        gpointer         user_data)
{
    Parking p = chercher_park("/home/amen/Projects/AABAParky/parkings.txt", selected_parking);
    
    if (p.id != -1)
    {
        GtkWidget* parking_name = lookup_widget(widget, "AABAlabelSupprimerParkingName");
        gtk_label_set_text(GTK_LABEL(parking_name), p.name);
    }
    else
    {
        windows_show_window(AABAwindowGererParking);
    }
}

void
on_AABAwindowGererParking_map          (GtkWidget       *widget,
                                        gpointer         user_data)
{
    GtkWidget* treeview = lookup_widget(widget, "AABAtreeviewGPList");
    populate_parking_treeview("/home/amen/Projects/AABAParky/parkings.txt", treeview);
}


void
on_AABAwindowModifier_map              (GtkWidget       *widget,
                                        gpointer         user_data)
{
    Parking p = chercher_park("/home/amen/Projects/AABAParky/parkings.txt", selected_parking);
    
    if (p.id != -1)
    {
        GtkWidget* name_entry = lookup_widget(widget, "AABAentryModifierNom");
        GtkWidget* location_entry = lookup_widget(widget, "AABAtextviewModifierLoc");
        GtkWidget* type_combo = lookup_widget(widget, "AABAcomboboxModifierType");
        GtkWidget* capacity_spin = lookup_widget(widget, "AABAspinbuttonModifierNumDePlaces");
        GtkWidget* is_24_hours_check = lookup_widget(widget, "AABAcheckbuttonModifier247");
        GtkWidget* open_hour_combo = lookup_widget(widget, "AABAcomboboxModifierOuv");
        GtkWidget* close_hour_combo = lookup_widget(widget, "AABAcomboboxModifierFerm");
        GtkWidget* hourly_rate_spin = lookup_widget(widget, "AABAspinbuttonModifierTarif");


        // Set name entry
        gtk_entry_set_text(GTK_ENTRY(name_entry), p.name);
        //gtk_widget_set_sensitive(name_entry, FALSE);
        gtk_entry_set_editable(GTK_ENTRY(name_entry), FALSE);

        // Set location
        GtkTextBuffer* buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(location_entry));
        gtk_text_buffer_set_text(buffer, p.location, -1);

        // Set type combo box
        if (strcmp(p.type, "Interieur") == 0)
            gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo), 0);
        else if (strcmp(p.type, "Exterieur") == 0)
            gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo), 1);
        else if (strcmp(p.type, "Sous-terrain") == 0)
            gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo), 2);

        // Set capacity spin button
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(capacity_spin), p.capacity);

        // Set 24/7 checkbox
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(is_24_hours_check), p.is_24_hours);

        // Set open and close hour combos
        if (p.is_24_hours) {
            // Disable these if 24/7
            gtk_widget_set_sensitive(open_hour_combo, FALSE);
            gtk_widget_set_sensitive(close_hour_combo, FALSE);
            gtk_combo_box_set_active(GTK_COMBO_BOX(open_hour_combo), -1);
            gtk_combo_box_set_active(GTK_COMBO_BOX(close_hour_combo), -1);
        } else {
            gtk_widget_set_sensitive(open_hour_combo, TRUE);
            gtk_widget_set_sensitive(close_hour_combo, TRUE);
            
            // Set open hour
            gtk_combo_box_set_active(GTK_COMBO_BOX(open_hour_combo), p.open_hour);
            
            // Set close hour
            gtk_combo_box_set_active(GTK_COMBO_BOX(close_hour_combo), p.close_hour);
        }

        // Set hourly rate spin button (assuming it accepts float)
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(hourly_rate_spin), p.hourly_rate);
    }
    else
    {
        windows_show_window(AABAwindowGererParking);
    }
}


void
on_AABAbuttonModifierEnregistrer_clicked
                                        (GtkWidget       *widget,
                                        gpointer         user_data)
{
    Parking p = chercher_park("/home/amen/Projects/AABAParky/parkings.txt", selected_parking);
    if (p.id != -1)
    {
        Parking parking = {0};

        GtkWidget* name_entry = lookup_widget(widget, "AABAentryModifierNom");
        GtkWidget* location_entry = lookup_widget(widget, "AABAtextviewModifierLoc");
        GtkWidget* type_combo = lookup_widget(widget, "AABAcomboboxModifierType");
        GtkWidget* capacity_spin = lookup_widget(widget, "AABAspinbuttonModifierNumDePlaces");
        GtkWidget* is_24_hours_check = lookup_widget(widget, "AABAcheckbuttonModifier247");
        GtkWidget* open_hour_combo = lookup_widget(widget, "AABAcomboboxModifierOuv");
        GtkWidget* close_hour_combo = lookup_widget(widget, "AABAcomboboxModifierFerm");
        GtkWidget* hourly_rate_spin = lookup_widget(widget, "AABAspinbuttonModifierTarif");

        GtkWidget* error_label = lookup_widget(widget, "AABAlabelModifierError");

        const char *name = gtk_entry_get_text(GTK_ENTRY(name_entry));
        if (name == NULL || strcmp(name, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "Nom ne peut pas etre vide.");
            return;
        }
        char* location = get_text_view_text(location_entry);
        if (location == NULL || strcmp(location, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "La localisation ne peut pas etre vide.");
            return;
        }
        char *type = get_combo_box_text(type_combo);
        if (type == NULL || strcmp(type, "") == 0)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "Le type ne peut pas etre vide.");
            return;
        }
        int capacity = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacity_spin));
        int is_24_hours = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(is_24_hours_check));
        int open_hour = gtk_combo_box_get_active(GTK_COMBO_BOX(open_hour_combo));
        int close_hour = gtk_combo_box_get_active(GTK_COMBO_BOX(close_hour_combo));
        if (is_24_hours == 0)
        {
            if (open_hour == -1)
            {
                gtk_label_set_text(GTK_LABEL(error_label), "L'heure d'ouverture ne peut pas etre vide.");
                return;
            }
            else if (close_hour == -1)
            {
                gtk_label_set_text(GTK_LABEL(error_label), "L'heure de fermeture ne peut pas etre vide.");
                return;
            }
        }
        float hourly_rate = gtk_spin_button_get_value(GTK_SPIN_BUTTON(hourly_rate_spin));

        strncpy(parking.name, name, MAX_NAME_LEN - 1);
        strncpy(parking.location, location, MAX_LOCATION_LEN - 1);
        g_free(location);
        strncpy(parking.type, type, sizeof(parking.type) - 1);
        g_free(type);
        parking.capacity = capacity;
        parking.is_24_hours = is_24_hours;
        parking.open_hour = open_hour;
        parking.close_hour = close_hour;
        parking.hourly_rate = hourly_rate;
        parking.agent = p.agent;

        if (modifier_park("/home/amen/Projects/AABAParky/parkings.txt", p.id, parking) == 1)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "Park Modifier!");
        }
        else
        {
            gtk_label_set_text(GTK_LABEL(error_label), "Erreur lors de la modification du parking...");
        }
    }
    else
    {
        windows_show_window(AABAwindowGererParking);
    }
}


void
on_AABAbuttonModifierAnnuler_clicked   (GtkButton       *button,
                                        gpointer         user_data)
{
    windows_show_window(AABAwindowGererParking);
}


void
on_AABAbuttonGPModifier_clicked        (GtkWidget       *widget,
                                        gpointer         user_data)
{
    GtkWidget* treeview = lookup_widget(widget, "AABAtreeviewGPList");
    selected_parking = get_selected_tree_view_row(treeview);
    if (selected_parking >= 0)
    {
        windows_show_window(AABAwindowModifier);
    }
}


void
on_AABAcheckbuttonModifier247_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget* open_hour_combo = lookup_widget(GTK_WIDGET(togglebutton), "AABAcomboboxModifierOuv");
    GtkWidget* close_hour_combo = lookup_widget(GTK_WIDGET(togglebutton), "AABAcomboboxModifierFerm");

    if (gtk_toggle_button_get_active(togglebutton) == 0)
    {
        gtk_widget_set_sensitive(open_hour_combo, TRUE);
        gtk_widget_set_sensitive(close_hour_combo, TRUE);
    }
    else
    {
        gtk_widget_set_sensitive(open_hour_combo, FALSE);
        gtk_widget_set_sensitive(close_hour_combo, FALSE);

        gtk_combo_box_set_active(GTK_COMBO_BOX(open_hour_combo), -1);
        gtk_combo_box_set_active(GTK_COMBO_BOX(close_hour_combo), -1);
    }
}


void
on_AABAbuttonGPAgent_clicked           (GtkWidget       *widget,
                                        gpointer         user_data)
{
    GtkWidget* treeview = lookup_widget(widget, "AABAtreeviewGPList");
    selected_parking = get_selected_tree_view_row(treeview);
    if (selected_parking >= 0)
    {
        windows_show_window(AABAwindowAttribuerAgent);
    }
}


void
on_AABAwindowAttribuerAgent_map        (GtkWidget       *widget,
                                        gpointer         user_data)
{
    Parking p = chercher_park("/home/amen/Projects/AABAParky/parkings.txt", selected_parking);
    
    if (p.id != -1)
    {
        GtkWidget* parking_name = lookup_widget(widget, "AABAlabelAGParkingName");
        gtk_label_set_text(GTK_LABEL(parking_name), p.name);
        GtkWidget* agent_combo = lookup_widget(widget, "AABAcomboboxentryAGAgentId");
        gtk_combo_box_set_active(GTK_COMBO_BOX(agent_combo), p.agent);
    }
    else
    {
        windows_show_window(AABAwindowGererParking);
    }
}


void
on_AABAbuttonAGAttribuer_clicked       (GtkWidget       *widget,
                                        gpointer         user_data)
{
    Parking p = chercher_park("/home/amen/Projects/AABAParky/parkings.txt", selected_parking);
    
    if (p.id != -1)
    {
        GtkWidget* agent_combo = lookup_widget(widget, "AABAcomboboxentryAGAgentId");
        int agent_id = gtk_combo_box_get_active(GTK_COMBO_BOX(agent_combo));
        g_print("%d\n", agent_id);

        GtkWidget* error_label = lookup_widget(widget, "AABAlabelAGError");

        if (attribuer_agent("/home/amen/Projects/AABAParky/parkings.txt", agent_id, p.id) == 1)
        {
            gtk_label_set_text(GTK_LABEL(error_label), "Agent Attribuee!");
            windows_show_window(AABAwindowGererParking);
        }
        else
        {
            gtk_label_set_text(GTK_LABEL(error_label), "Erreur lors de l'ajout de l'agent...");
        }
    }
    else
    {
        windows_show_window(AABAwindowGererParking);
    }
}


void
on_AABAbuttonAGAnnuler_clicked         (GtkWidget       *widget,
                                        gpointer         user_data)
{
    windows_show_window(AABAwindowGererParking);
}


// ---------------------------------------------------------------------------------
// NIDHAL GHANMI:
// ---------------------------------------------------------------------------------
/*
#include"agent.h"

int supp=1;
int choix[4]={0,0};
int choix2[4]={0,0};
char ch[500]="",cha[500]="";
void
on_button_GR_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *EspaceAgent,*Listedesagents ;


	EspaceAgent=lookup_widget(button,"EspaceAgent");
	gtk_widget_destroy(EspaceAgent);
	Listedesagents=create_Listedesagents();
	gtk_widget_show(Listedesagents); 

}


void
on_APT_clicked                         (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *Affichagelistedereservation,*EspaceAgent ;


	EspaceAgent=lookup_widget(button,"EspaceAgent");
	gtk_widget_destroy(EspaceAgent);
	Affichagelistedereservation=create_Affichagelistedereservation();
	gtk_widget_show(Affichagelistedereservation);
}


void
on_Liste_A_row_activated          (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
GtkTreeIter iter;
	gint* CIN;
	gint* nom;
	gint* prenom
	gint* jour;
	gint* mois;
	gint* annee;
	gchar* tel;
	gchar* sexe;	
	gchar* adresse;
	gchar* service;

	
	
	rec r;
	GtkTreeModel *model = gtk_tree_view_get_model (treeview) ;

	if (gtk_tree_model_get_iter(model, &iter, path) ){

		gtk_tree_model_get(GTK_LIST_STORE(model), &iter, 0, &CIN, 1, &jour, 2,&mois,3,&annee,4,&tel,5,&adresse,6,&email,7,&service,-1);
		
}
}


void
on_button_M_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *ModificationA,*Listedesagents;


	Listedesagents=lookup_widget(button,"Listedesagents");
	gtk_widget_destroy(Listedesagents);
	ModificationA=create_ModificationA();
	gtk_widget_show(ModificationA); 
}


void
on_button_Supp_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *Suppagt,*Listedesagents;


	Listedesagents=lookup_widget(button,"Listedesagents");
	gtk_widget_destroy(ListeA);
	SuppAgt=create_SuppAgt();
	gtk_widget_show(SuppAgt); 
}

void
on_button1_Liste_a_reser_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Listesdesagents,*Liste_A;
	Liste_A = lookup_widget(button,"Listesdesagents");
	Listesdesagents=lookup_widget(Listesdesagents,"Liste_A");
	 afficher_agent (Liste_A,ch);
}
void
on_button_Ajouter_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *AjouterAgent,*Listedesagents ;


	Listedesagents=lookup_widget(button,"Listedesagents");
	gtk_widget_destroy(Listedesagents);
	AjouterAgent=create_Ajouteragent();
	gtk_widget_show(AjouterAgent); 

}
void
on_home1_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *EspaceAgents,*Listedesagents ;
	Listedesagents=lookup_widget(button,"Listedesagents");
	gtk_widget_destroy(Listedesagents);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent);
}


void
on_return1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

	GtkWidget *EspaceAgent,*Listedesagents ;
	GestionR=lookup_widget(button,"Listedesagents");
	gtk_widget_destroy(Listedesagents);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent); 
}
void
on_return2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *Listedesagents,*ModificationA;
	ModificationA=lookup_widget(button,"ModificationA");
	gtk_widget_destroy(ModificationA);
	Listedesagents=create_Listedesagents();
	gtk_widget_show(Listedesagents);
}


void
on_home2_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *EspaceAgent,*ModificationA;
	ModificationA=lookup_widget(button,"ModificationA");
	gtk_widget_destroy(ModificationA);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent); 
}
void
on_button_amod_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *ModificationA ,*Modificationagt;
	GtkWidget *input1, *input2;
	GtkWidget *output;
	char vide[2] = "";
	char mot1[50] = "introuvable";

	
	char service[10];int CIN;
	agt a;

	ModificationA=lookup_widget(button,"ModificationA");
	input1 = lookup_widget(button, "entry5_nM");
        input2 = lookup_widget(button, "spinbutton9_idMM");
	output=lookup_widget(button,"label257_mo");

	strcpy(tel,gtk_entry_get_text(GTK_ENTRY(input1)));
	 recid =gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (input2));

	 r = chercher("Agent.txt", CIN,sercice,ch);
	if(strcmp(r.tel,"-1")!=0){
	gtk_label_set_text(GTK_LABEL(output), vide);
	add(recid,tel);
 	    Modificationagt=create_Modificationagt();
	    gtk_widget_show(Modificationagt);
		gtk_widget_destroy(ModificationA);
}
   	else {   
	gtk_label_set_text(GTK_LABEL(output), mot1);
}
 }
void
on_return3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *ModificationA,*Modificationagt;
	Modificationagt=lookup_widget(button,"Modificationagt");
	gtk_widget_destroy(Modificationagt);
	ModificationA=create_ModificationA();
	gtk_widget_show(ModificationA);
}


void
on_home3_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *Espaceagent,*Modificationagt;
	Modificationagt=lookup_widget(button,"Modificationagt");
	gtk_widget_destroy(Modificationunrec);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent); 
}


void
on_buttonm_A_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    char vide[2] = "";
   char mot1[50] = "modifier avec succès";
   char mot2[50] = "ECHEC de modif  ";
	GtkWidget *output;

reserid=pull(reserid,CIN[10]);

GtkWidget *input1, *input2, *input3, *input4, *input5 , *input6, *input7;
GtkWidget *Listedesagents,*ModificationAg;

GtkWidget *ch1,*ch2,*ch3,*ch4;

ModificationAg=lookup_widget(button,"ModificationAg");

    input1 = lookup_widget(button, "combobox1_idM");
    input2 = lookup_widget(button, "entry4_nMMM");
    input3 = lookup_widget(button, "spinbutton5_ff");
    input4 = lookup_widget(button, "spinbutton6_yy");
    input5 = lookup_widget(button, "spinbutton9_uu");
input6 = lookup_widget(button, "combobox2_AMM");	
input7 = lookup_widget(button, "entry1_descnew");	

ch1=lookup_widget(button,"checkbutton1_rec_mod");
ch2=lookup_widget(button,"checkbutton2_rec_mod");
ch3=lookup_widget(button,"checkbutton3_rec_mod");
ch4=lookup_widget(button,"checkbutton4_rec_mod");
output = lookup_widget(button, "label263_confrdvmb");

	strcpy(v.desc,gtk_entry_get_text(GTK_ENTRY(input7)));
	a.date.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (input3));
	a.date.mois=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (input4));
	a.date.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (input5));
	strcpy(r.reser,gtk_combo_box_get_active_text(GTK_COMBO_BOX (input6)));
	strcpy(r.idreser,gtk_combo_box_get_active_text(GTK_COMBO_BOX (input1)));
	strcpy(r.duree,gtk_entry_get_text(GTK_ENTRY(input2)));



         a=modifier("agent.txt", tel,recid,v,cha,ch,choix2);
       if(a==1){
	afficherListedereservation("agent.txt");
    choix2[0]=0;
    choix2[1]=0;
    
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(input4), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(input5), 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(input3), 1);
                gtk_entry_set_text(GTK_ENTRY(input7), vide);
                gtk_combo_box_set_active(GTK_COMBO_BOX (input1),0);
                gtk_combo_box_set_active(GTK_COMBO_BOX (input6),0);
                gtk_entry_set_text(GTK_ENTRY(input2), vide);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch1),FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch2),FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch3),FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch4),FALSE);
        gtk_label_set_text(GTK_LABEL(output), mot1); 
    }
	else { 
		gtk_label_set_text(GTK_LABEL(output), mot2); 
}
		

}
void
on_checkbutton2_rdv_mod_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix2[1]=1;
}
}
void
on_checkbutton1_rdv_mod_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix2[0]=1;
}
}


void
on_home4_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *EspaceAgent,*Ajouteragent;
	AjouterAgent=lookup_widget(button,"Ajouteragent");
	gtk_widget_destroy(AjouterAgent);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent);
}


void
on_return4_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *Listedesagents,*AjouterAgent;
	AjouterAgent=lookup_widget(button,"AjouterAgent");
	gtk_widget_destroy(AjouterAgent);
	Listedesagents=create_Listedesagents();
	gtk_widget_show(Listedesagents); 
}


void
on_button_A_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
 
	 char vide[2] = "";
	char mot1[50] = "Ajout avec succès";
   	 char mot2[50] = "ECHEC d'ajout  ";
	GtkWidget *output;
	GtkWidget *ch1,*ch2,*ch3,*ch4;


	ch1=lookup_widget(button,"checkbutton5__A");
	ch2=lookup_widget(button,"checkbutton6__A");
	ch3=lookup_widget(button,"checkbutton7__A");
	ch4=lookup_widget(button,"checkbutton8__A");
	output = lookup_widget(button, "label256");

    int a;
    rec v;
    GtkWidget *input1, *input2, *input3, *input4, *input5, *input6 ,*input7 ;
    GtkWidget *Ajouteragent;

    AjouterRec = lookup_widget(button, "Ajouteraent");
    input1 = lookup_widget(button, "combobox1_Agent");
    input2 = lookup_widget(button, "entry2");
    input3 = lookup_widget(button, "spinbutton3");
    input4 = lookup_widget(button, "spinbutton4");
    input5 = lookup_widget(button, "spinbutton_Agent");
    input6 = lookup_widget(button, "combobox2_AgentA");
    input7 = lookup_widget(button, "entry_descA");
	

	strcpy(v.desc,gtk_entry_get_text(GTK_ENTRY(input7)));
	a.date.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (input3));
	a.date.mois=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (input4));
	a.date.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (input5));
	strcpy(r.reservation,gtk_combo_box_get_active_text(GTK_COMBO_BOX (input1)));
	strcpy(r.idreserv,gtk_combo_box_get_active_text(GTK_COMBO_BOX (input6)));
	strcpy(r.,gtk_entry_get_text(GTK_ENTRY(input2)));


     		a=ajouter("Agent.txt", v,choix,ch);
		   if(a==1){
	afficherListedereservation("Agent.txt");
		choix[0]=0;
		choix[1]=0;
		
		    gtk_spin_button_set_value(GTK_SPIN_BUTTON(input4), 1);
		    gtk_spin_button_set_value(GTK_SPIN_BUTTON(input5), 1);
		    gtk_spin_button_set_value(GTK_SPIN_BUTTON(input3), 1);
                    gtk_entry_set_text(GTK_ENTRY(input7), vide);
                    gtk_combo_box_set_active(GTK_COMBO_BOX (input1),0);
                    gtk_combo_box_set_active(GTK_COMBO_BOX (input6),0);
                    gtk_entry_set_text(GTK_ENTRY(input2), vide);
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch1),FALSE);
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch2),FALSE);
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch3),FALSE);
		    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ch4),FALSE);
   			gtk_label_set_text(GTK_LABEL(output), mot1);
		      }
	else { 
		gtk_label_set_text(GTK_LABEL(output), mot2); 
}
}


void
on_home5_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *EspaceAgent,*SuppAgt;
	SuppAgt=lookup_widget(button,"SuppAgt");
	gtk_widget_destroy(SuppAgt);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent); 
}


void
on_return5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

	GtkWidget *Listedesagents,*SuppAgt;
	SuppAgt=lookup_widget(button,"SuppAgt");
	gtk_widget_destroy(SuppAgt);
	Listedesagents=create_Listedesagents();
	gtk_widget_show(Listedesagents);
}
void
on_button_spr_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{

    char mot1[55] = "introuvable";
char vide[2] = "";
agt a;
char service[9];int CIN;
GtkWidget *input1,*input2;
GtkWidget*output;
GtkWidget *AssurerSuppAgt;
GtkWidget *Suppagent;

SuppRec=lookup_widget(button,"Suppagent");
input1=lookup_widget(button,"entry3_teltel");
    input2 = lookup_widget(button, "spinbutton8_oo");
output=lookup_widget(button,"label259_supp");



strcpy(tel,gtk_entry_get_text(GTK_ENTRY(input1)));
 recid = gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON (input2));
 r = chercher("Agent.txt", service,CIN,ch);
if(strcmp(a.service,"-1")!=0){
     AssurerSuppAgt=create_AssurerSuppAgt();
    gtk_widget_show(AssurerSuppAgt);
add(service,CIN);

}
	 else {
		gtk_label_set_text(GTK_LABEL(output), mot1);
}
}


void
on_return6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *AssurerSuppAgt,*Suppagent;
	AssurerSuppAgt=lookup_widget(button,"AssurerSuppAgt");
	gtk_widget_destroy(AssurerSuppAgt);
	Suppagent=create_Suppagent();
	gtk_widget_show(Suppagent); 
}

void
on_radiobutton_A_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton))){
supp = 2;
}
}


void
on_radiobutton_supp_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton))){
supp = 1;
}
}


void
on_home6_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *AssurerSuppAgt,*EspaceAgent;
	AssurerSuppAgt=lookup_widget(button,"AssurerSuppAgt");
	gtk_widget_destroy(AssurerSuppAgt);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent);
}


void
on_treeview1_tree_don_cin_row_activated
                                        (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
GtkTreeIter iter;
	
	gchar* reservation;	
	gchar* CIN;
	
	rec r;
	GtkTreeModel *model = gtk_tree_view_get_model (treeview) ;

	if (gtk_tree_model_get_iter(model, &iter, path) ){

		gtk_tree_model_get(GTK_LIST_STORE(model), &iter, 0,&duree,1,&,-1);
	
}
}


void
on_return7_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
  	GtkWidget *Affichagelistedereservation,*EspaceAgent ;
	Affichagelistedereservation=lookup_widget(button,"Affichagelistedereservation");
	gtk_widget_destroy(Affichagelistedereservation);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent);
}


void
on_home7_clicked                       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *Affichagelistedereservation,*EspaceAgent ;
	Affichagelistedereservation=lookup_widget(button,"Affichagelistedereservation");
	gtk_widget_destroy(Affichagelistedereservation);
	EspaceAgent=create_EspaceAgent();
	gtk_widget_show(EspaceAgent);
}


void
on_checkbutton5_rec_A_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix[0]=1;
}
}


void
on_checkbutton7_rec_A_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix[1]=1;
}
}


void
on_checkbutton6_A_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix[2]=1;
}
}


void
on_checkbutton8_rec_A_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix[3]=1;
}
}


void
on_checkbutton3_rec_mod_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix2[2]=1;}
}


void
on_checkbutton4_rec_mod_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton)){
choix2[3]=1;}
}


void
on_button52_bb_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
         
	GtkWidget *AssurerSuppagt;
	GtkWidget *Suppagt;
	char service[10];int agentcin;
	agtcin=pull(agtcin,service[10]);
	if(supp==1){
	supprimer("Agent.txt",tel[9],recid,ch);}
	AssurerSuppAgt=lookup_widget(button,"AssurerSuppAgt");
	gtk_widget_destroy(AssurerSuppAgt);
	supp =1;

}
void
on_afficheragt_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Affichagelistedereservation,*treeview1_tree_par_tri;
	Affichagelistedereservation = lookup_widget(button,"Affichagelistedereservation");
	treeview1_tree_par_tri=lookup_widget(Affichagelistedereservation,"treeview1_tree_par_tri");
	 afficher_rectree (treeview1_tree_par_tri);
}
*/

// ---------------------------------------------------------------------------------
// IBTIHEL BACCARI:
// ---------------------------------------------------------------------------------
/*
#include "service.h"
int selected = 1;
int available = 0;

char temp [20];

void
on_IBbuttonVA_clicked                  (GtkWidget      *objet_graphique,
                                        gpointer         user_data)
{
service s;
	GtkWidget *EntryID;
	GtkWidget *EntryNOM;
	GtkWidget *EntryDIS;
	GtkWidget *EntryPRI;
        GtkWidget *Entrybdis;

	EntryID = lookup_widget(objet_graphique,"entryId"); 
printf("test1\n");
	EntryNOM = lookup_widget(objet_graphique,"entrynom");
printf("test2\n");
	EntryDIS = lookup_widget(objet_graphique,"entrydis");
printf("test3\n");
	EntryPRI = lookup_widget(objet_graphique,"entrypri");
printf("test4\n");
            
        s.id = atoi(gtk_entry_get_text(GTK_ENTRY(EntryID)));
printf("test6\n");
	strcpy(s.nom ,gtk_entry_get_text(GTK_ENTRY(EntryNOM)));
printf("test7\n");
	strcpy(s.description ,gtk_entry_get_text(GTK_ENTRY(EntryDIS)));
printf("test8\n");
        strcpy(temp ,gtk_entry_get_text(GTK_ENTRY(EntryPRI)));
	printf("%s\n", temp);
        s.prix=atoi(temp);
printf("test9\n");
	if(selected ==1 )
	{
	   strcpy(s.disponibilite,"Disponible");
printf("test10\n"); 
	}	
	else if(selected ==2){
	   strcpy(s.disponibilite,"Non disponible");
        g_print("%d %s %s %d %s\n",s.id , s.nom ,s.description,s.prix, s.disponibilite );

printf("test0\n");
        }

   	   FILE * f=fopen("service.txt", "a");
if(f!=NULL)
    {
        fprintf(f,"%d %s %s %d %s\n",s.id , s.nom ,s.description,s.prix, s.disponibilite);
        fclose(f);
        printf("test\n");
	        
	

	
    };
}


void
on_button_annuler_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
printf("annuler");
}


void
on_IBradiobuttonDIS_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
        if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton)))
	{
	selected = 1;
	}

}


void
on_IBradiobuttonNON_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
        if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton)))
	{
	selected = 2;
	}

}




void
on_IBbuttonVA1_clicked                 (GtkWidget      *objet_graphique,
                                        gpointer         user_data)
{
service s;
	GtkWidget *EntryID1;
	GtkWidget *EntryNOM1;
	GtkWidget *EntryIDIS1;
	GtkWidget *EntryIPRI1;
        GtkWidget *EntryDISP1;

EntryID1 = lookup_widget(objet_graphique,"entryid1"); 
printf("test11\n");
	EntryNOM1 = lookup_widget(objet_graphique,"entrynom1");
printf("test12\n");
	EntryIDIS1 = lookup_widget(objet_graphique,"entrydis1");
printf("test13\n");
	EntryIPRI1 = lookup_widget(objet_graphique,"entrypri1");
printf("test14\n");
            
        s.id = atoi(gtk_entry_get_text(GTK_ENTRY(EntryID1)));
printf("test16\n");
	strcpy(s.nom ,gtk_entry_get_text(GTK_ENTRY(EntryNOM1)));
printf("test17\n");
	strcpy(s.description ,gtk_entry_get_text(GTK_ENTRY(EntryIDIS1)));
printf("test18\n");
        strcpy(temp ,gtk_entry_get_text(GTK_ENTRY(EntryIPRI1)));
	printf("%s\n", temp);
        s.prix=atoi(temp);
printf("test19\n");
	if(selected ==1 )
	{
	   strcpy(s.disponibilite,"Disponible");
printf("test20\n"); 
	}	
	else if(selected ==2){
	   strcpy(s.disponibilite,"Non disponible");
        g_print("%d %s %s %d %s\n",s.id , s.nom ,s.description,s.prix, s.disponibilite );

printf("test21\n");
        }

   	   FILE * f=fopen("service.txt", "a");
if(f!=NULL)
    {
        fprintf(f,"%d %s %s %d %s\n",s.id , s.nom ,s.description,s.prix, s.disponibilite);
        fclose(f);
        printf("test22\n");
	        
	

	
    };
}


void
on_button_annuler1_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
printf("annuler");
}


void
on_IBradiobuttonDIS1_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
        if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton)))
	{
	selected = 1;
	}

}


void
on_IBradiobuttonNON1_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
        if(gtk_toggle_button_get_active(GTK_RADIO_BUTTON(togglebutton)))
	{
	selected = 2;
	}

}


void
on_button_supprimer_clicked            (GtkWidget      *objet_graphique,
                                        gpointer         user_data)
{
  GtkWidget *EntryID;
    int id_to_delete;
    FILE *f, *temps;
    service s;

   
    EntryID = lookup_widget(GTK_WIDGET(objet_graphique), "entryId"); 
    id_to_delete = atoi(gtk_entry_get_text(GTK_ENTRY(EntryID)));

    
    if (id_to_delete == 0) {
        g_print("Veuillez entrer un ID valide !\n");
        return;
    }

    
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(objet_graphique))),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "Êtes-vous sûr de vouloir supprimer le service avec ID %d ?", id_to_delete);

    // Récupérer la réponse de l'utilisateur
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    if (response == GTK_RESPONSE_YES) {
        // Ouvrir les fichiers
        f = fopen("service.txt", "r");
        temps = fopen("temps.txt", "w");

        if (f != NULL && temps != NULL) {
            // Parcourir et copier les services sauf celui à supprimer
            while (fscanf(f, "%d %s %s %d %s\n", &s.id, s.nom, s.description, &s.prix, s.disponibilite) != EOF) {
                if (s.id != id_to_delete) {
                    fprintf(temps, "%d %s %s %d %s\n", s.id, s.nom, s.description, s.prix, s.disponibilite);
                }
            }
            fclose(f);
            fclose(temps);

            // Remplacer le fichier original
            remove("service.txt");
            rename("temps.txt", "service.txt");

            g_print("Service avec ID %d supprimé avec succès !\n", id_to_delete);
        } else {
            g_print("Erreur d'ouverture des fichiers.\n");
        }
    } else {
        g_print("Suppression annulée.\n");
    }

}


void
on_IBbuttonnon_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
 printf("Non");
}
*/

// ---------------------------------------------------------------------------------
// TAKWA WERGHEMMI:
// ---------------------------------------------------------------------------------
/*
#include "ajouter.h"


void on_Ajouter_clicked (GtkWidget *objet_graphique, gpointer user_data){
    printf("test");
    AvisReclamationDetails avis = {0};  // Initialisation complète de la structure

    GtkWidget *entry1;
    GtkWidget *entry6;
    GtkWidget *entry2;
    GtkWidget *RadioButton1, *reclamation;
    GtkWidget *entry_categorie;
    GtkWidget *spinbutton1;
    GtkWidget *spinbuttonjour; 
    GtkWidget *spinbuttonmois; 
    GtkWidget *spinbuttonannes; 
    GtkWidget *CheckButton1; 

    entry1 = lookup_widget(objet_graphique, "entry1");
    entry2 = lookup_widget(objet_graphique, "entry2");
    entry6 = lookup_widget(objet_graphique, "entry6");
    RadioButton1 = lookup_widget(objet_graphique, "RadioButton1");
    reclamation = lookup_widget(objet_graphique, "reclamation");
    entry_categorie = lookup_widget(objet_graphique, "entry_categorie");
    spinbutton1= lookup_widget(objet_graphique, "spinbutton1");
    spinbuttonjour=lookup_widget(objet_graphique,"spinbuttonjour");
    spinbuttonmois=lookup_widget(objet_graphique,"spinbuttonmois");
    spinbuttonannes=lookup_widget(objet_graphique,"spinbuttonannes");
    CheckButton1 = lookup_widget(objet_graphique, "CheckButton1");
    if (entry1 == NULL) {
        g_print("Erreur : entry1 non trouvé\n");
        return;  // Quitter la fonction si le widget est introuvable
    }
    printf("test2");
    // Récupérer le texte de l'entry1 (id_client)
    const gchar *id_client_text = gtk_entry_get_text(GTK_ENTRY(entry1));
    if (id_client_text == NULL || strlen(id_client_text) == 0) {
        g_print("Erreur : l'ID client est vide\n");
        return;
    }
    
    // Convertir le texte en entier pour id_client
    avis.id_client = atoi(id_client_text);
    if (avis.id_client == 0 && strcmp(id_client_text, "0") != 0) {
        g_print("Erreur : ID client invalide\n");
        return;
    }

strcpy(avis.contenu,gtk_entry_get_text(GTK_ENTRY(entry2)));
avis.id_parking=atoi(gtk_entry_get_text(GTK_ENTRY(entry6)));


if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(RadioButton1))){
  strcpy(avis.type_demande, "avis");
}
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(reclamation))){
  strcpy(avis.type_demande, "reclamation");
}

if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(CheckButton1))){
  strcpy(avis.urgent, "urgent");
}  

strcpy(avis.categorie,gtk_combo_box_get_active_text(GTK_COMBO_BOX(entry_categorie))); 
avis.date_reclamation.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttonjour));
avis.date_reclamation.mois=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttonmois));
avis.date_reclamation.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttonannes));
avis.note_satisfaction=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton1));   
    ajouterAvis(avis, "avis.txt");

GtkBuilder *builder = (GtkBuilder *)user_data;

    // Récupérer le dialogue depuis Glade
    GtkWidget *dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog1"));

    // Afficher le dialogue
    gtk_widget_show(dialog);

    // Attendre la réponse de l'utilisateur
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Cacher le dialogue après interaction
    gtk_widget_hide(dialog);


}






void
on_button_afficher_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *treeview;

    // Trouver le TreeView par son ID ou son nom dans la fenêtre principale
    treeview = lookup_widget(GTK_WIDGET(button), "treeview1"); // Remplacez "treeview1" par l'ID réel de votre TreeView

    if (treeview == NULL) {
        g_print("Erreur : Impossible de trouver le widget TreeView.\n");
        return;
    }

    // Appeler la fonction pour lire les données et les afficher dans le TreeView
    afficher_avis(GTK_TREE_VIEW(treeview), "avis.txt");
}


void
on_button_precedent_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
gtk_notebook_prev_page(GTK_NOTEBOOK(lookup_widget(button, "notebook")));
}


void
on_button_modifier_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
AvisReclamationDetails avis_temp;
AvisReclamationDetails avis;
 GtkWidget *entry3;
    int Id;
    entry3 = lookup_widget(GTK_WIDGET(button), "entry3");

    // Convertir le texte de l'entrée en entier
    Id = atoi(gtk_entry_get_text(GTK_ENTRY(entry3)));


 GtkWidget *entry4;
GtkWidget *entry7;
    GtkWidget *radiobutton1; 
    GtkWidget *radiobutton2;
    GtkWidget *combobox1;
    GtkWidget *spinbutton2;
    GtkWidget *spinbutton3;
    GtkWidget *spinbutton4;
    GtkWidget *spinbutton5;
    GtkWidget *checkbutton1;

    entry4 = lookup_widget(GTK_WIDGET(button), "entry4");
    entry7 = lookup_widget(GTK_WIDGET(button), "entry7");
    radiobutton1 = lookup_widget(GTK_WIDGET(button), "radiobutton1");
    radiobutton2 = lookup_widget(GTK_WIDGET(button), "radiobutton2");
    combobox1 = lookup_widget(GTK_WIDGET(button), "combobox1");
    spinbutton2 = lookup_widget(GTK_WIDGET(button), "spinbutton2");
    spinbutton3 = lookup_widget(GTK_WIDGET(button), "spinbutton3");
    spinbutton4 = lookup_widget(GTK_WIDGET(button), "spinbutton4");
    spinbutton5 = lookup_widget(GTK_WIDGET(button), "spinbutton5");
    checkbutton1 = lookup_widget(GTK_WIDGET(button), "checkbutton1");


strcpy(avis_temp.contenu,gtk_entry_get_text(GTK_ENTRY(entry4)));
avis.id_parking=atoi(gtk_entry_get_text(GTK_ENTRY(entry7)));

 avis_temp.note_satisfaction=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton2));
  avis_temp.date_reclamation.jour=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton3));
 avis_temp.date_reclamation.mois=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton4));
 avis_temp.date_reclamation.annee=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton5));

strcpy(avis_temp.categorie,gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox1)));
if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton1))){
  strcpy(avis_temp.urgent, "urgent");

}


if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton1))){
  strcpy(avis_temp.type_demande, "avis");

}
else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton2))){
        strcpy(avis_temp.type_demande, "reclamation");}

 modifier_reserv("avis.txt",  Id,  avis_temp);


   
}


void
on_button_suivant_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}




void
on_button_cherche_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
 AvisReclamationDetails avis;

    GtkWidget *entry3;
    int Id;
    entry3 = lookup_widget(GTK_WIDGET(button), "entry3");

    // Convertir le texte de l'entrée en entier
    Id = atoi(gtk_entry_get_text(GTK_ENTRY(entry3)));

    // Appeler la fonction de recherche avec un entier
    avis =cherche("avis.txt", Id);

    GtkWidget *entry4;
    GtkWidget *radiobutton1, *radiobutton2;
    GtkWidget *combobox1;
    GtkWidget *spinbutton2;
    GtkWidget *spinbutton3;
    GtkWidget *spinbutton4;
    GtkWidget *spinbutton5;
    GtkWidget *checkbutton1;

    entry4 = lookup_widget(GTK_WIDGET(button), "entry4");
    radiobutton1 = lookup_widget(GTK_WIDGET(button), "radiobutton1");
    radiobutton2 = lookup_widget(GTK_WIDGET(button), "radiobutton2");
    combobox1 = lookup_widget(GTK_WIDGET(button), "combobox1");
    spinbutton2 = lookup_widget(GTK_WIDGET(button), "spinbutton2");
    spinbutton3 = lookup_widget(GTK_WIDGET(button), "spinbutton3");
    spinbutton4 = lookup_widget(GTK_WIDGET(button), "spinbutton4");
    spinbutton5 = lookup_widget(GTK_WIDGET(button), "spinbutton5");
    checkbutton1 = lookup_widget(GTK_WIDGET(button), "checkbutton1");

    if (strcmp(avis.type_demande, "avis") == 0) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton1), TRUE);
    }
    if (strcmp(avis.type_demande, "reclamation") == 0) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton2), TRUE);
    }

    if (strcmp(avis.urgent, "urgent") == 0) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton1), TRUE);
    }

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton2), avis.note_satisfaction);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton3), avis.date_reclamation.jour);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton4), avis.date_reclamation.mois);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton5), avis.date_reclamation.annee);


    gtk_entry_set_text(GTK_ENTRY(entry4), avis.contenu);
    gtk_combo_box_append_text(GTK_COMBO_BOX(combobox1), avis.categorie);
}

void
on_button_suivant1_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
gtk_notebook_next_page(GTK_NOTEBOOK(lookup_widget(button, "notebook")));

}


void
on_button_supprimer_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
AvisReclamationDetails avis_temp;
 GtkWidget *entry5;
    int Id;
    entry5 = lookup_widget(GTK_WIDGET(button), "entry5");

    // Convertir le texte de l'entrée en entier
    Id = atoi(gtk_entry_get_text(GTK_ENTRY(entry5)));
  supprimerAvis(Id, -1, "avis.txt");  // -1 pour numero_reclamation si non utilisé


}


void
on_button_precedent2_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_suivant3_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}




void
on_button_trier_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *treeview;

    // Trouver le TreeView par son ID ou son nom dans la fenêtre principale
    treeview = lookup_widget(GTK_WIDGET(button), "treeview3"); // Remplacez "treeview1" par l'ID réel de votre TreeView

    if (treeview == NULL) {
        g_print("Erreur : Impossible de trouver le widget TreeView.\n");
        return;
    }

    // Appeler la fonction pour lire les données et les afficher dans le TreeView
    affichage_trie(GTK_TREE_VIEW(treeview), "avis.txt");
}


void
on_button_affichagetous_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{

}
*/

// ---------------------------------------------------------------------------------
// NOUR BEN YOUCEF:
// ---------------------------------------------------------------------------------
/*
#include "reservation.h"

int service[] = {0, 0, 0};
int type;

void
on_TD_standardajout_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
        type = 1;
}


void
on_TD_electriqueajout_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(GTK_RADIO_BUTTON (togglebutton)))
        type = 2;
}


void
on_TD_Entretienajout_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
    {
        service[0] = 1;
    }else {
        service[0] = 0;
    }
}


void
on_TD_nettoyageajout_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
    {
        service[1] = 1;
    }else {
        service[1] = 0;
}
}

void
on_TD_lavageajout_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_TD_rechargeajout_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_TD_validerajout_clicked             (GtkWidget       *objet,
                                        gpointer         user_data)
{

Reservation R;
GtkWidget   *input1, *input2, *input3, *input4, *input5 ,*input6,*input7,*input8;

    input1 = lookup_widget(objet, "TD_IDajoutreservation");
    input2 = lookup_widget(objet, "TD_jourajout");
    input3 = lookup_widget(objet, "TD_moisajout");
    input4 = lookup_widget(objet, "TD_anneeajout");
    input5 = lookup_widget(objet, "TD_heurajout");
    input6 = lookup_widget(objet, "TD_minajout");
    input7 = lookup_widget(objet, "TD_combobox1");
    input8 = lookup_widget(objet, "TD_dureeajout");

    R.id_client =atoi(gtk_entry_get_text(GTK_ENTRY(input1)));
    R.date_entree.jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input2));
    R.date_entree.mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input3));
    R.date_entree.annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input4));
    R.heure_entree.heure = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input5));
    R.heure_entree.minute = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(input6));

    //strcpy(R.duree, gtk_combo_box_text_get_active_text(input7));
    strcpy(R.duree, gtk_combo_box_get_active_text(GTK_COMBO_BOX(input8)));
   strcpy(R.id_parking, gtk_combo_box_get_active_text(GTK_COMBO_BOX(input7)));
//stgtk_combo_box_get_active_text(GTK_COMBO_BOX(Combobox_nature))
    if (type==1){
      strcpy(R.type,"standard");
    }    
    else
    {
    strcpy(R.type,"electrique");
    }
    if (service[0]==1)
    {
     R.entretient_c=1;
    }
    if (service[1]==1)
    {
     R.services_c=1;
     }
     if (service[2]==1)
     {
      R.priseencharge_c=1;  
     }
ajouter_Reservation(R);
   }
void
on_TD_standardmodif_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
}


void
on_TD_electriquemodif_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_TD_Entretienmodif_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_TD_nettoyagemodif_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}




void
on_TD_validermodif_activate            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_TD_recherchesupp_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_TD_supp_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

}

void
on_TD_rechercheaffiche_clicked         (GtkWidget       *button,
                                        gpointer         user_data)
{

}

void
on_TD_priseencharge_toggled            (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if (gtk_toggle_button_get_active(togglebutton))
    {
        service[2] = 1;
    }else {
        service[2] = 0;
    }
}

// Callback pour le bouton d'affichage
void
on_TD_button1_clicked                  (GtkWidget       *objet_graphique,
                                        gpointer         user_data)

{
    GtkWidget *treeview;
    GtkWidget *fenetre_afficher;
fenetre_afficher=lookup_widget(objet_graphique,"reservation");
   // GtkBuilder *builder = GTK_BUILDER(user_data);

    // Récupérer le TreeView depuis le builder
treeview=lookup_widget(fenetre_afficher,"TD_Afficheres");
  
    if (!treeview) {
        g_error("Le TreeView 'TD_Afficheres' est introuvable.");
    }

    // Appeler la fonction pour afficher les réservations
    Afficher_Reservation(treeview);
}
*/