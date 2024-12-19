#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "windows.h"
#include "gerer_park.h"
#include "utils.h"

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

