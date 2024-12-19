#ifndef UTILS_H
#define UTILS_H

#include <gtk/gtk.h>
#include <stdarg.h>
char* get_combo_box_text(GtkWidget* combo_box) {
    int active_index = gtk_combo_box_get_active(GTK_COMBO_BOX(combo_box));
    if (active_index == -1) return NULL;

    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo_box));
    GtkTreeIter iter;
    
    if (gtk_tree_model_iter_nth_child(model, &iter, NULL, active_index)) {
        char *text;
        gtk_tree_model_get(model, &iter, 0, &text, -1);
        return text;
    }
    
    return NULL;
}

char* get_text_view_text(GtkWidget* text_view)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

int get_selected_tree_view_row(GtkWidget* treeview)
{
    GtkTreeSelection* selection;
    GtkTreeModel* model;
    GtkTreeIter iter;
    int id = -1;

    // Get the tree selection
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    // Check if a row is selected
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Get the ID from the first column (index 0)
        gtk_tree_model_get(model, &iter, 0, &id, -1);
    }

    return id;
}


#endif // UTILS_H