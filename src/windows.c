#include <gtk/gtk.h>
#include "support.h"

#include "windows.h"
#include "interface.h"
#include "gerer_park.h"

// Declare windows here
GtkWidget* AABAwindowSupprimer = NULL;
GtkWidget* AABAwindowAjouter = NULL;
GtkWidget* AABAwindowModifier = NULL;
GtkWidget* AABAwindowGererParking = NULL;
GtkWidget* AABAwindowAttribuerAgent = NULL;

GtkWidget* current_window = NULL;

void populate_parking_treeview(const char* filename, GtkWidget* treeview)
{
    GtkListStore* store;
    GtkTreeIter iter;
    FILE *rf = fopen(filename, "r");
    
    // Get the model from the treeview
    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    
    // Clear existing items
    gtk_list_store_clear(store);
    
    if (rf != NULL)
    {
        Parking sp;
        while (fscanf(rf, "%d %49s %99s %19s %d %d %d %d %d %d\n",
                      &sp.id, sp.name, sp.location, sp.type, &sp.capacity,
                      &sp.is_24_hours, &sp.open_hour, &sp.close_hour, &sp.hourly_rate,
                      &sp.agent) != EOF) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                0, sp.id,
                1, sp.name,
                2, sp.location,
                3, sp.type,
                4, sp.capacity,
                5, sp.hourly_rate,
                6, sp.agent,
                -1);
        }
        fclose(rf);
    }
}
void setup_parking_treeview(GtkWidget* window) {
    GtkWidget* treeview;
    GtkListStore* store;
    GtkTreeViewColumn* column;
    GtkCellRenderer* renderer;
    
    // Find the treeview widget
    treeview = lookup_widget(window, "AABAtreeviewGPList");
    
    // Create a list store with the columns we want to display
    store = gtk_list_store_new(7, 
        G_TYPE_INT,        // ID
        G_TYPE_STRING,     // Name
        G_TYPE_STRING,     // Location
        G_TYPE_STRING,     // Type
        G_TYPE_INT,        // Capacity
        G_TYPE_INT,        // Tarif
        G_TYPE_INT         // Tarif
    );
    
    // Set the model for the treeview
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    
    // Create columns
    // ID Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("ID", renderer, 
        "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Name Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Name", renderer, 
        "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Location Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Location", renderer, 
        "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Type Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type", renderer, 
        "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Capacity Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Capacity", renderer, 
        "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Tarif Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Tarif (Dt/Hr)", renderer, 
        "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Agent Column
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Agent", renderer, 
        "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    
    // Populate the treeview
    populate_parking_treeview("/home/amen/Projects/AABAParky/parkings.txt",treeview);
    
    // Unref the store as it's now owned by the treeview
    g_object_unref(store);
}
// Call this in your main setup function
void AABAwindowGererParking_init() {
    AABAwindowGererParking = create_AABAwindowGererParking();
    setup_parking_treeview(AABAwindowGererParking);
}


void windows_init(void)
{
    // Add window initialization of each window here
    AABAwindowSupprimer = create_AABAwindowSupprimer();
    AABAwindowAjouter = create_AABAwindowAjouter();
    AABAwindowModifier = create_AABAwindowModifier();
    AABAwindowGererParking_init();
    AABAwindowAttribuerAgent = create_AABAwindowAttribuerAgent();

    // Connect destroy signal
    g_signal_connect(G_OBJECT(AABAwindowSupprimer), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(AABAwindowAjouter), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(AABAwindowModifier), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(AABAwindowGererParking), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(AABAwindowAttribuerAgent), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Hide all windows initially
    gtk_widget_hide(AABAwindowSupprimer);
    gtk_widget_hide(AABAwindowAjouter);
    gtk_widget_hide(AABAwindowModifier);
    gtk_widget_hide(AABAwindowGererParking);
    gtk_widget_hide(AABAwindowAttribuerAgent);
}
void windows_show_window(GtkWidget* window)
{
    if (current_window != NULL)
    {
        gtk_widget_hide(current_window);
    }
    current_window = window;
    gtk_widget_show(window);
}