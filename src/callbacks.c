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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "agent.h"

//done
/*int confmod=0;
void
on_checkbutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

	if(gtk_toggle_button_get_active(togglebutton))
		confmod=1;
	else
		confmod=0;

}
*/


//int sexe = 0;

//void
//on_radiobutton1_toggled                (GtkToggleButton *togglebutton,
                                     //   gpointer         user_data)
//{
// if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
       // sexe = 1; //homme
   // }
//}


//void
//on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        //gpointer         user_data)
//{
//if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(togglebutton))) {
        //sexe = 2; // femme
  //  }

//}




// Fonction nbr ou non
int is_number(const char *str) {
    while (*str) {
        if (*str < '0' || *str > '9') {
            return 0; 
        }
        str++;
    }
    return 1; 
}

// controle de saisie CIN
int validate_CIN(const char *cin) {
    return (strlen(cin) == 8 ) && is_number(cin);
}

// controle de saisie telephone
int validate_phone_number(const char *number) {
    return strlen(number) == 8 && is_number(number);
}
// bouton ajouter


void on_button1_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *entry1, *entry2, *entry3, *entry4;
    GtkWidget *entry5, *entry6, *combobox1, *combobox2,*combobox5;
    agent a;
    char msg[50];
    char erreur[500];
    gboolean valid = TRUE;

    entry1 = lookup_widget(GTK_WIDGET(button), "entry1");//cin
    entry2 = lookup_widget(GTK_WIDGET(button), "entry2");
    entry3 = lookup_widget(GTK_WIDGET(button), "entry3");
    entry4 = lookup_widget(GTK_WIDGET(button), "entry4");
    combobox5 = lookup_widget(GTK_WIDGET(button), "combobox5"); //sexe
    combobox1 = lookup_widget(GTK_WIDGET(button), "combobox1");
    combobox2 = lookup_widget(GTK_WIDGET(button), "combobox2");
    entry5 = lookup_widget(GTK_WIDGET(button), "entry5");
    entry6 = lookup_widget(GTK_WIDGET(button), "entry6");
   

    strcpy(a.CIN, gtk_entry_get_text(GTK_ENTRY(entry1)));
    strcpy(a.prenom, gtk_entry_get_text(GTK_ENTRY(entry2)));
    strcpy(a.nom, gtk_entry_get_text(GTK_ENTRY(entry3)));
    strcpy(a.email, gtk_entry_get_text(GTK_ENTRY(entry4)));
    strcpy(a.sexe, gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox5)));
    strcpy(a.service, gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox1)));

    const gchar *active_text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combobox2));
    if (active_text != NULL) {
        strcpy(a.ID_Parking, active_text);
    }

    const gchar *tel_text = gtk_entry_get_text(GTK_ENTRY(entry5));
    strcpy(a.num_tel, tel_text);

    strcpy(a.adresse, gtk_entry_get_text(GTK_ENTRY(entry6)));

if (strcmp(a.nom, "") == 0) {
        strcat(erreur, "Le champ Nom est vide.\n");
        valid = FALSE;
    }
    if (strcmp(a.prenom, "") == 0) {
        strcat(erreur, "Le champ Prénom est vide.\n");
        valid = FALSE;
    }
    if (strcmp(a.email, "") == 0) {
        strcat(erreur, "Le champ Email est vide.\n");
        valid = FALSE;
    }

   //msg erreur CIN
    if (!validate_CIN(a.CIN) ) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Le CIN doit contenir 8");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        valid = FALSE;
    }


    // msg erreur tlf
    if (!validate_phone_number(a.num_tel)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Le numéro de téléphone doit contenir 8 chiffres.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        valid = FALSE;
    }


 
   

    

    
        //if (sexe == 1) {
           // strcpy(msg, "Homme");
       // } else {
          //  strcpy(msg, "Femme");
      //  }
	
	if (valid) {
      
        ajouter_agent("agent.txt", a); 
	// msg succes
	GtkWidget *success_dialog = gtk_message_dialog_new(GTK_WINDOW(user_data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Agent ajouté avec succès !");
gtk_dialog_run(GTK_DIALOG(success_dialog));
gtk_widget_destroy(success_dialog);

    }
}


//////////////////
/*void on_button3_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *input_CIN; 
    GtkWidget *input_nom; 
    GtkWidget *input_prenom;
    GtkWidget *input_email;
    GtkWidget *input_sexe;
    GtkWidget *input_service; 
    GtkWidget *input_ID_Parking;
    GtkWidget *input_num_tel; 
    GtkWidget *input_adresse; 
    GtkWidget *dialog = NULL;

    agent a; 
    char erreur[500] = "";  
    FILE *f = fopen("agent.txt", "a");
    char CIN[20], nom[100], prenom[100], email[100]; 
    char service[100], ID_Parking[100]; 
    char num_tel[100], adresse[100];
    int  tr=0;


    
    input_CIN = lookup_widget(GTK_WIDGET(button), "entry8");
    input_nom = lookup_widget(GTK_WIDGET(button), "entry9");
    input_prenom = lookup_widget(GTK_WIDGET(button), "entry10");
    input_email = lookup_widget(GTK_WIDGET(button), "entry11");
    input_service = lookup_widget(GTK_WIDGET(button), "combobox3");    
    input_ID_Parking = lookup_widget(GTK_WIDGET(button), "combobox4"); 
    input_num_tel = lookup_widget(GTK_WIDGET(button), "entry13");
    input_adresse = lookup_widget(GTK_WIDGET(button), "entry12");

    
    strcpy(a.CIN, gtk_entry_get_text(GTK_ENTRY(input_CIN)));
    a = chercher_Agent("agent.txt", a.CIN); 

    
    if (strcmp(a.CIN, "-1") != 0) {
        strcpy(a.nom, gtk_entry_get_text(GTK_ENTRY(input_nom)));
        strcpy(a.prenom, gtk_entry_get_text(GTK_ENTRY(input_prenom)));
        strcpy(a.email, gtk_entry_get_text(GTK_ENTRY(input_email)));
         strcpy(a.service, gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_etatCivil))); 
         strcpy(a.ID_Parking, gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_ID_Parking)));
        strcpy(a.num_tel, gtk_entry_get_text(GTK_ENTRY(input_num_tel)));         
        strcpy(a.adresse, gtk_entry_get_text(GTK_ENTRY(input_horaires_travail)));

        
        if (strcmp(a.CIN, "") == 0) {
            strcat(erreur, "Le champ CIN est vide\n");
        }
        if (strcmp(a.CIN, "-1") == 0) {
            strcat(erreur, "CIN n'existe pas dans la base de données\n");
        }

        
        if (strcmp(erreur, "") != 0) {
            dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                           GTK_DIALOG_DESTROY_WITH_PARENT, 
                                           GTK_MESSAGE_WARNING, 
                                           GTK_BUTTONS_OK, "%s", erreur); 
            gtk_window_set_title(GTK_WINDOW(dialog), "Alerte");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } 
        
        
             tr=modifier_agent("agent.txt", a.CIN, a);
            if (tr) {
                dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                               GTK_DIALOG_DESTROY_WITH_PARENT, 
                                               GTK_MESSAGE_INFO, 
                                               GTK_BUTTONS_OK, 
                                               "Modification réussie avec succès !");
                gtk_window_set_title(GTK_WINDOW(dialog), "Succès");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
            } else {
                dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                               GTK_DIALOG_DESTROY_WITH_PARENT, 
                                               GTK_MESSAGE_WARNING, 
                                               GTK_BUTTONS_OK, 
                                               "Erreur lors de la modification !");
                gtk_window_set_title(GTK_WINDOW(dialog), "Erreur");
                gtk_dialog_run(GTK_DIALOG(dialog));
                gtk_widget_destroy(dialog);
            }
        
    } else {
        dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                       GTK_DIALOG_DESTROY_WITH_PARENT, 
                                       GTK_MESSAGE_WARNING, 
                                       GTK_BUTTONS_OK, 
                                       "CIN introuvable dans la base de données");
        gtk_window_set_title(GTK_WINDOW(dialog), "Erreur");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}*/

void on_button3_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *input_CIN; 
    GtkWidget *input_nom; 
    GtkWidget *input_prenom;
    GtkWidget *input_email;
    GtkWidget *input_service; 
    GtkWidget *input_ID_Parking;
    GtkWidget *input_num_tel; 
    GtkWidget *input_adresse; 
    GtkWidget *dialog;
    agent a; 
    char erreur[500] = "";  
    FILE *f = fopen("agent.txt", "a");
    char CIN[20], nom[100], prenom[100], email[100]; 
    char service[100], ID_Parking[100]; 
    char num_tel[100], adresse[100];
    int  tr = 0;

    // Vérification de l'ouverture du fichier
    if (f == NULL) {
        strcat(erreur, "Erreur d'ouverture du fichier.\n");
        dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                       GTK_DIALOG_DESTROY_WITH_PARENT, 
                                       GTK_MESSAGE_WARNING, 
                                       GTK_BUTTONS_OK, "%s", erreur); 
        gtk_window_set_title(GTK_WINDOW(dialog), "Alerte");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return; // Arrêter la fonction si le fichier ne peut être ouvert
    }

    // Récupérer les widgets
    input_CIN = lookup_widget(GTK_WIDGET(button), "entry8");
    input_nom = lookup_widget(GTK_WIDGET(button), "entry9");
    input_prenom = lookup_widget(GTK_WIDGET(button), "entry10");
    input_email = lookup_widget(GTK_WIDGET(button), "entry11");
    input_service = lookup_widget(GTK_WIDGET(button), "combobox3");    
    input_ID_Parking = lookup_widget(GTK_WIDGET(button), "combobox4"); 
    input_num_tel = lookup_widget(GTK_WIDGET(button), "entry13");
    input_adresse = lookup_widget(GTK_WIDGET(button), "entry12");

    // Initialiser dialog
    dialog = NULL;

    // Récupérer les valeurs des champs
    strcpy(a.CIN, gtk_entry_get_text(GTK_ENTRY(input_CIN)));
    a = chercher_Agent("agent.txt", a.CIN);

    // Vérifier si l'agent a été trouvé
    if (strcmp(a.CIN, "-1") == 0) {
        strcat(erreur, "CIN introuvable dans la base de données\n");
    }

    // Si l'agent est trouvé, procéder à la modification
    if (strcmp(a.CIN, "-1") != 0) {
        strcpy(a.nom, gtk_entry_get_text(GTK_ENTRY(input_nom)));
        strcpy(a.prenom, gtk_entry_get_text(GTK_ENTRY(input_prenom)));
        strcpy(a.email, gtk_entry_get_text(GTK_ENTRY(input_email)));
        
        // Vérifier les valeurs des combobox
        const char *serviceText = gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_service));
        if (serviceText != NULL) {
            strcpy(a.service, serviceText);
        } else {
            strcat(erreur, "Service non sélectionné\n");
        }

        const char *ID_ParkingText = gtk_combo_box_get_active_text(GTK_COMBO_BOX(input_ID_Parking));
        if (ID_ParkingText != NULL) {
            strcpy(a.ID_Parking, ID_ParkingText);
        } else {
            strcat(erreur, "ID Parking non sélectionné\n");
        }

        strcpy(a.num_tel, gtk_entry_get_text(GTK_ENTRY(input_num_tel)));         
        strcpy(a.adresse, gtk_entry_get_text(GTK_ENTRY(input_adresse)));

        // Vérifier si CIN est vide
        if (strcmp(a.CIN, "") == 0) {
            strcat(erreur, "Le champ CIN est vide\n");
        }

        // Si des erreurs sont présentes, afficher un message d'erreur
        if (strcmp(erreur, "") != 0) {
            dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                           GTK_DIALOG_DESTROY_WITH_PARENT, 
                                           GTK_MESSAGE_WARNING, 
                                           GTK_BUTTONS_OK, "%s", erreur); 
            gtk_window_set_title(GTK_WINDOW(dialog), "Alerte");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            return; // Arrêter la fonction si des erreurs existent
        }

        // Appeler la fonction pour modifier l'agent
        tr = modifier_agent("agent.txt", a.CIN, a);

        // Afficher le résultat de la modification
        if (tr) {
            dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                           GTK_DIALOG_DESTROY_WITH_PARENT, 
                                           GTK_MESSAGE_INFO, 
                                           GTK_BUTTONS_OK, "Modification réussie avec succès !");
            gtk_window_set_title(GTK_WINDOW(dialog), "Succès");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else {
            dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                           GTK_DIALOG_DESTROY_WITH_PARENT, 
                                           GTK_MESSAGE_WARNING, 
                                           GTK_BUTTONS_OK, "Erreur lors de la modification !");
            gtk_window_set_title(GTK_WINDOW(dialog), "Erreur");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    } else {
        // Si l'agent n'a pas été trouvé, afficher une erreur
        dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                       GTK_DIALOG_DESTROY_WITH_PARENT, 
                                       GTK_MESSAGE_WARNING, 
                                       GTK_BUTTONS_OK, "CIN introuvable dans la base de données");
        gtk_window_set_title(GTK_WINDOW(dialog), "Erreur");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }

    // Fermer le fichier
    fclose(f);
}




void
on_button2_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

    agent a;
    char CIN[50];
    GtkWidget *input_CIN;
    input_CIN = lookup_widget(GTK_WIDGET(button), "entry7");
    const gchar *CIN_const = gtk_entry_get_text(GTK_ENTRY(input_CIN));
    
    strcpy(CIN, gtk_entry_get_text(GTK_ENTRY(input_CIN)));

    
    GtkWidget *output_CIN, *output_nom, *output_prenom, *output_email, *output_service;
    GtkWidget *output_ID_Parking, *output_num_tel, *output_adresse;
    GtkWidget *dialog;

    a = chercher_Agent("agent.txt", CIN);
    output_CIN = lookup_widget(GTK_WIDGET(button), "entry8");
    output_nom = lookup_widget(GTK_WIDGET(button), "entry9");
    output_prenom = lookup_widget(GTK_WIDGET(button), "entry10");
    output_email = lookup_widget(GTK_WIDGET(button), "entry11");
   // output_service = lookup_widget(GTK_WIDGET(button), "combobox3"); 
  //  output_ID_Parking = lookup_widget(GTK_WIDGET(button), "combobox4"); 
    output_num_tel = lookup_widget(GTK_WIDGET(button), "entry13"); 
    output_adresse = lookup_widget(GTK_WIDGET(button), "entry12");

   
    

    
    if (strcmp(a.CIN, "-1") != 0) {
        
        gtk_entry_set_text(GTK_ENTRY(output_CIN), a.CIN);
        gtk_entry_set_text(GTK_ENTRY(output_nom), a.nom);
        gtk_entry_set_text(GTK_ENTRY(output_prenom), a.prenom);
        gtk_entry_set_text(GTK_ENTRY(output_email), a.email);
 	gtk_entry_set_text(GTK_ENTRY(output_num_tel), a.num_tel);
        gtk_entry_set_text(GTK_ENTRY(output_adresse), a.adresse); 
	

    } 
else {
        
dialog = gtk_message_dialog_new(GTK_WINDOW(dialog),
                                GTK_DIALOG_DESTROY_WITH_PARENT,
                                GTK_MESSAGE_WARNING,
                                GTK_BUTTONS_OK,
                                "CIN n'existe pas");
        gtk_window_set_title(GTK_WINDOW(dialog), "Alerte");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}







int choice=1;
void
on_radiobutton3_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton))
		choice=1;

}


void
on_radiobutton4_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
if(gtk_toggle_button_get_active(togglebutton))
		choice=2;
}


void on_button4_clicked(GtkButton *button, gpointer user_data) {
    if (choice == 1) {
        char cin[50];
        GtkWidget *input_CIN;
        input_CIN = lookup_widget(GTK_WIDGET(button), "entry14");
        strcpy(cin, gtk_entry_get_text(GTK_ENTRY(input_CIN)));

        // Appeler la fonction pour supprimer l'agent
        if (supprimer_agent("agent.txt", cin)) {
            // Si la suppression est réussie, afficher un message de succès
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_INFO,
                                                       GTK_BUTTONS_OK,
                                                       "Agent supprimé avec succès !");
            gtk_window_set_title(GTK_WINDOW(dialog), "Succès");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        } else {
            // Si la suppression échoue (par exemple, agent non trouvé), afficher un message d'erreur
            GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(user_data),
                                                       GTK_DIALOG_MODAL,
                                                       GTK_MESSAGE_WARNING,
                                                       GTK_BUTTONS_OK,
                                                       "L'agent n'a pas été trouvé dans la base de données !");
            gtk_window_set_title(GTK_WINDOW(dialog), "Erreur");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}

void
on_treeview1_row_activated             (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

	GtkTreeIter iter;
	gchar *CIN;

	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	
	if (gtk_tree_model_get_iter(model,&iter,path))
	{
		gtk_tree_model_get (model, &iter, 0, &CIN,-1);
	
		supprimer_agent("agent.txt",CIN);
	
        	
	}


}


void
on_button5_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *treeview, *Ajouter_N, *w_afficher;
 
    Ajouter_N = lookup_widget(GTK_WIDGET(button), "Ajouter_N");
      
    gtk_widget_destroy(Ajouter_N);
   
    Ajouter_N = create_Ajouter_N();
    
    gtk_widget_show(Ajouter_N);
    
    w_afficher = lookup_widget(GTK_WIDGET(button), "afficherlisteagents");
    
    treeview = lookup_widget(w_afficher, "treeview1");

    afficherAgents(treeview, "agent.txt");

 


}


void
on_button12_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Home,*Ajouter_N;
Ajouter_N=lookup_widget(GTK_WIDGET(button),"Ajouter_N");
gtk_widget_destroy(Ajouter_N);
Home= create_Home();
gtk_widget_show (Home);
}


void
on_button13_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Home,*modifier;
modifier=lookup_widget(GTK_WIDGET(button),"modifier_N");
gtk_widget_destroy(modifier);
Home= create_Home();
gtk_widget_show (Home);
}


void
on_button14_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Home,*supprimer_N;
supprimer_N=lookup_widget(GTK_WIDGET(button),"supprimer_N");
gtk_widget_destroy(supprimer_N);
Home= create_Home();
gtk_widget_show (Home);
}


void
on_button15_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Home,*afficherlisteagents;
afficherlisteagents=lookup_widget(GTK_WIDGET(button),"afficherlisteagents");
gtk_widget_destroy(afficherlisteagents);
Home= create_Home();
gtk_widget_show (Home);
}


void
on_button6_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
//GtkWidget *treeview, *w, *w_afficher;
 
   // w = lookup_widget(GTK_WIDGET(button), "");
      
   // gtk_widget_destroy(w);
   
   // w_ = create_();
    
  //  gtk_widget_show(w_);
    
  //  w_afficher = lookup_widget(GTK_WIDGET(button), "affichageressa");
    
   // treeview = lookup_widget(w_afficher, "treeview2");

  //  afficherReservations(treeview, "reservation.txt");
}


void
on_button16_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Home,*afficherlistereservation;
afficherlistereservation=lookup_widget(GTK_WIDGET(button),"afficherlistereservation");
gtk_widget_destroy(afficherlistereservation);
Home= create_Home();
gtk_widget_show (Home);
}


void
on_button7_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *Ajouter_N,*w;
w=lookup_widget(GTK_WIDGET(button),"Home");
gtk_widget_destroy(w);
Ajouter_N= create_Ajouter_N();
gtk_widget_show (Ajouter_N);
}


void
on_button8_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *modifier_N,*w;
w=lookup_widget(GTK_WIDGET(button),"Home");
gtk_widget_destroy(w);
modifier_N= create_modifier_N();
gtk_widget_show (modifier_N);
}


void
on_button9_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *supprimer_N,*w;
w=lookup_widget(GTK_WIDGET(button),"Home");
gtk_widget_destroy(w);
supprimer_N= create_supprimer_N();
gtk_widget_show (supprimer_N);
}


void
on_button10_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *afficherlisteagents,*w;
w=lookup_widget(GTK_WIDGET(button),"Home");
gtk_widget_destroy(w);
afficherlisteagents= create_afficherlisteagents();
gtk_widget_show (afficherlisteagents);
}


void
on_button11_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *afficherlistereservation,*w;
w=lookup_widget(GTK_WIDGET(button),"Home");
gtk_widget_destroy(w);
afficherlistereservation= create_afficherlistereservation();
gtk_widget_show (afficherlistereservation);
}


void on_button17_clicked(GtkButton *button, gpointer user_data) {
    char ID_Parkingr[50];
    GtkWidget *input_ID;
    
    input_ID = lookup_widget(GTK_WIDGET(button), "entry15");
    
    
    strcpy(ID_Parkingr, gtk_entry_get_text(GTK_ENTRY(input_ID)));
    
    
    if (strlen(ID_Parkingr) == 0) {
        g_warning("L'ID_Parkingr est vide !");
        return;  
    }

    
    reservation("resrv.txt", ID_Parkingr);

    
    GtkWidget *Ajouter_N, *w_afficher, *treeview;
    Ajouter_N = lookup_widget(GTK_WIDGET(button), "Ajouter_N");
    gtk_widget_destroy(Ajouter_N);

    Ajouter_N = create_Ajouter_N();
    gtk_widget_show(Ajouter_N);

   
    w_afficher = lookup_widget(GTK_WIDGET(button), "afficherlistereservation");
    treeview = lookup_widget(w_afficher, "treeview2");
    afficherReservations(treeview, "reservation.txt");
}

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
