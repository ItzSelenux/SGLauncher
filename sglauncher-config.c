#include <gtk/gtk.h>

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "~/.config/sglauncher.conf"


 int main(int argc, char *argv[])
{
    GtkWidget *dialog;

    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Settings - SGLauncher");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    
    // Create the header bar
    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);


    GtkWidget *button = gtk_menu_button_new();
    GtkWidget *image = gtk_image_new_from_icon_name("menulibre", GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(button), image);

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
    GtkWidget *wtitle = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wtitle), "<b>Settings - SGLauncher</b>");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
    // Create the submenu
    GtkWidget *submenu = gtk_menu_new();

    // Create the submenu items
    GtkWidget *submenu_item3 = gtk_menu_item_new_with_label("About");
    // Add the submenu items to the submenu

    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);

    // Show all the submenu items
    gtk_widget_show_all(submenu);

    // Connect the button to the submenu
    gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);
    

    // Add the header bar to the main window
    gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

    
    
    
    // Create grid
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    /// Widgets
    GtkWidget *webcombo = gtk_combo_box_text_new();
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "DuckDuckGo");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Google");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Bing");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Custom");
    
    GtkWidget *webctm = gtk_entry_new();
    GtkWidget *order = gtk_check_button_new(); 
    GtkWidget *showcmd = gtk_check_button_new(); 
    GtkWidget *showweb =  gtk_check_button_new();
    GtkWidget *showcalc =  gtk_check_button_new(); 
        
    GtkWidget *defbtn    = gtk_button_new_with_label("Default");
    GtkWidget *applybtn  = gtk_button_new_with_label("Apply");
    

    //Items Grid position
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Search Engine:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), webcombo, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Custom Search Engine:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), webctm, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Apps first"), 0, 3, 1, 1);    
    gtk_grid_attach(GTK_GRID(grid), order, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Run in Terminal\" entry "), 0, 4, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), showcmd, 1, 4, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Search on Web\" entry "), 0, 5, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), showweb,   1, 5, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Math answer when numbers are typed "), 0, 6, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), showcalc,   1, 6, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 7, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), gtk_label_new("WARNING: this settings screen is not implemented yet "), 0, 9, 1, 1); 
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(webctm), "https://you.com/search?q=");
    
    //button void
    void on_default_button_clicked(GtkButton *button, gpointer user_data) 
    {
    printf("this is default");
    }
    
    void on_apply_button_clicked(GtkButton *button, gpointer user_data) 
    {
 printf("this is apply");
    }

    const char *web_ctm = gtk_entry_get_text(GTK_ENTRY(webctm));
    const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo));
    gboolean gorder = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(order));
    gboolean gshowcmd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(showcmd));
    gboolean gshowweb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(showweb));
    gboolean gshowcalc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(showcalc));
    const gchar *cweb = NULL;
    
        if (active_text != NULL) 
        {
                 if (g_strcmp0(active_text, "Google") == 0) 
            {
                cweb = "--same-as";
            } 
            else if (g_strcmp0(active_text, "DuckDuckGo") == 0) 
            {
                cweb = "--left-of";
            } 
            else if (g_strcmp0(active_text, "Bing") == 0) 
            {
                cweb = "--right-of";
            } 
            else if (g_strcmp0(active_text, "Custom") == 0) 
            {
                cweb = web_ctm;
        }
    }
    
    
        void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
    {
        dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGLauncher-config");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2023 ItzSelenux for Simple GTK Desktop Environment");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "SGDE SGLauncher Settings Manager");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/sglauncher");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_MIT_X11);
    gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"menulibre");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    }

    g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
    g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), NULL);
    
    g_signal_connect(defbtn, "clicked", G_CALLBACK(on_default_button_clicked), NULL);

    // End
        gtk_widget_show_all(window);
    gtk_main();
return 0;
}
