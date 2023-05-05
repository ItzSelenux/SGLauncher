#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
char *webengine;
 int main(int argc, char *argv[])
{
    
    char cengine[MAX_LINE_LENGTH];
    //READ THE CONF
    /////////////////////////////////////////
    char *home_dir = getenv("HOME");
    char *cwengine;
    cwengine = " ";
    if (home_dir == NULL) {
        fprintf(stderr, "Error: HOME environment variable is not set.\n");
        exit(1);
    }

    char config_file_path[256];
    snprintf(config_file_path, sizeof(config_file_path), "%s/.config/sglauncher.conf", home_dir);

    FILE *file = fopen(config_file_path, "r");
    if (file == NULL) {
        
         if (file == NULL) {
        FILE *default_conf = fopen("/etc/sglauncher.conf", "r");
        if (default_conf == NULL) {
            fprintf(stderr, "Error: could not open default configuration file /etc/sgconfig.conf, please reinstall the program or put a config file in ~/.config/sglauncher.conf.\n");
            exit(1);
        }

        file = fopen(config_file_path, "w");
        if (file == NULL) {
            fprintf(stderr, "Error: could not create %s for writing.\n", config_file_path);
            exit(1);
        }

        int ch;
        while ((ch = fgetc(default_conf)) != EOF) {
            fputc(ch, file);
        }

        fclose(default_conf);
        printf("Default configuration file copied to %s.\n", config_file_path);
    } else {
        fclose(file);
        printf("%s exists and can be read.\n", config_file_path);
    }
        
        
    }

// Open the file for reading
    char line[MAX_LINE_LENGTH];
    int wengine = 0, order = 0, showweb = 0, showcmd = 0, showcalc = 0;

    if (file != NULL) {
        // Read each line from the file and parse the variable assignments
        while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
            char *name = strtok(line, "=");
            char *value_str = strtok(NULL, "=");
            


if (file != NULL) {
    // Read each line from the file and parse the variable assignments
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        char *name = strtok(line, "=");
        char *value_str = strtok(NULL, "=");

        if (name != NULL && value_str != NULL) {
            // Set the value of the corresponding variable based on the name
            if (strcmp(name, "order") == 0) 
            {
                order = atoi(value_str);
            } 
            else if (strcmp(name, "cengine") == 0) 
            {
                strncpy(cengine, value_str, sizeof(cengine));
                cengine[sizeof(cengine)-1] = '\0'; // Ensure null-terminated
            } 
            else if (strcmp(name, "wengine") == 0)
            {
                wengine = atoi(value_str);
            } 
            else if (strcmp(name, "showweb") == 0) 
            {
                showweb = atoi(value_str);
            } 
            else if (strcmp(name, "showcmd") == 0) 
            {
                showcmd = atoi(value_str);
            } 
            else if (strcmp(name, "showcalc") == 0) 
            {
                showcalc = atoi(value_str);
            }
        }
    }
}
        }
        fclose(file); // Close the file
    } else {
        // Handle error if the file couldn't be opened
        printf("Error opening file");
        return 1;
    }


        if (wengine == 0 )
    {
        webengine = "https://duckduckgo.com/?q=";
    }
    else if (wengine == 1 )
    {
        webengine = "https://www.google.com/search?q=";
    }
    else if (wengine == 2 )
    {
        webengine = "https://www.bing.com/search?q=";
    }
    else if (wengine == 3 )
    {
        webengine = cengine;
    }
    
    
    // Use the values that were read from the file
   // printf("WebEngine: %s\nShowAppsFirst: %d\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\n", webengine, order, showcmd, showweb, showcalc);
    ////////////////////////////////////////
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
     //   gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Custom");
    
    GtkWidget *weblabel = gtk_label_new("Custom Search Engine:");
    GtkWidget *webctm = gtk_entry_new();
    GtkWidget *worder = gtk_combo_box_text_new(); 
    
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at top");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at bottom");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at left");
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at right");
        
    GtkWidget *wshowcmd = gtk_check_button_new(); 
    GtkWidget *wshowweb =  gtk_check_button_new();
    GtkWidget *wshowcalc =  gtk_check_button_new(); 
        
    GtkWidget *defbtn    = gtk_button_new_with_label("Default");
    GtkWidget *applybtn  = gtk_button_new_with_label("Apply");
    

    //Items Grid position
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Search Engine:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), webcombo, 1, 0, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), weblabel, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), webctm, 1, 2, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Item Order:"), 0, 3, 1, 1);    
    gtk_grid_attach(GTK_GRID(grid), worder, 1, 3, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Run in Terminal\" entry "), 0, 4, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), wshowcmd, 1, 4, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Search on Web\" entry "), 0, 5, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), wshowweb,   1, 5, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Math answer when numbers are typed "), 0, 6, 1, 1); 
    gtk_grid_attach(GTK_GRID(grid), wshowcalc,   1, 6, 1, 1);
    
    gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 7, 1, 1);
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(webctm), "https://you.com/search?q=");

    //button void
 void on_webcombo_changed(GtkComboBox *webcombo, gpointer user_data) {
    const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo));
    GtkWidget *webctm = GTK_WIDGET(user_data);
    GtkWidget *webctmlb = GTK_WIDGET(g_object_get_data(G_OBJECT(webctm), "webctmlb"));

    if (g_strcmp0(active_text, "Custom") != 0) {
        gtk_widget_hide(webctm);
        gtk_widget_hide(weblabel);
    } else {
        gtk_widget_show(webctm);
        gtk_widget_show(weblabel);
    }

    g_free((gpointer) active_text);
}


    const char *web_ctm = gtk_entry_get_text(GTK_ENTRY(webctm));



    
    //SET Options STATUS
    if (showcmd == 1)
    {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcmd), TRUE);
    }
    if (showweb == 1)
    {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowweb), TRUE);
    }
    if (showcalc == 1)
    {
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcalc), TRUE);
    }
    
gtk_combo_box_set_active(GTK_COMBO_BOX(webcombo), wengine);
gtk_combo_box_set_active(GTK_COMBO_BOX(worder), order);
   

    
   
void on_default_button_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Are you sure you want to restore SGLauncher settings as default?");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        char *home_dir = getenv("HOME");
        if (home_dir == NULL) {
            fprintf(stderr, "Error: HOME environment variable is not set.\n");
            exit(1);
        }
        char *source_file = "/etc/sglauncher.conf";
        char *destination_file = "/.config/sglauncher.conf";
        char command[100];
        sprintf(command, "cp %s %s%s", source_file, home_dir, destination_file);
        system(command);
   
        char *args[] = {argv[0], NULL};
    execvp(args[0], args);

    } else {
        // User clicked Cancel
        printf("Operation cancelled.\n");
    }
    gtk_widget_destroy(dialog);
}

    void on_apply_button_clicked(GtkButton *button, gpointer user_data) 
    {
    gboolean gshowcmd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcmd));
    gboolean gshowweb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowweb));
    gboolean gshowcalc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcalc));
    const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo));  
    const gchar *active_order = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(worder));   
    const gchar *cweb = NULL;
    const gchar *cwebng = gtk_entry_get_text(GTK_ENTRY(webctm));
    const gchar *corder = NULL;
        if (active_text != NULL) 
        {
                 if (g_strcmp0(active_text, "Google") == 0) 
            {
                cweb = "1";
            } 
            else if (g_strcmp0(active_text, "DuckDuckGo") == 0) 
            {
                cweb = "0";
            } 
            else if (g_strcmp0(active_text, "Bing") == 0) 
            {
                cweb = "2";
            } 
            else if (g_strcmp0(active_text, "Custom") == 0) 
            {
                  cweb = "3";
        }
        
        
              if (active_order != NULL) 
        {
                 if (g_strcmp0(active_order, "Horizontal - Apps at bottom") == 0) 
            {
                corder = "0";
            } 
            else if (g_strcmp0(active_order, "Horizontal - Apps at top") == 0) 
            {
                corder = "1";
            } 
            else if (g_strcmp0(active_order, "Vertical - Apps at left") == 0) 
            {
                corder = "2";
            } 
            else if (g_strcmp0(active_order, "Vertical - Apps at right") == 0) 
            {
                corder = "3";
        }
    }
    
    
    }
        
        
 printf("\nWebEngine: %s\nCustomWebEngine: %s\nOrder: %s\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\n", cweb, cwebng, corder, gshowcmd, gshowweb, gshowcalc);

    FILE *fp = fopen(config_file_path, "r+");
    if (fp == NULL) {
        perror("Failed to open config file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) 
    {
        if (strncmp(line, "order=", 6) == 0) 
        {
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "order=%s\n", corder);
        }
        else if (strncmp(line, "wengine=", 8) == 0) 
        {
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "wengine=%s\n", cweb);
        }
    /*    else if (strncmp(line, "cengine=", 8) == 0) 
     * {

       if (cwebng !=NULL) {
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "cengine=%s\n", cwebng);
        }
        } */
        else if (strncmp(line, "showweb=", 8) == 0) 
        {
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "showweb=%d\n", gshowweb);
        }
        else if (strncmp(line, "showcmd=", 8) == 0) 
        {
  
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "showcmd=%d\n", gshowcmd);
        }
        else if (strncmp(line, "showcalc=", 9) == 0) 
        {
            fseek(fp, -strlen(line), SEEK_CUR);
            fprintf(fp, "showcalc=%d\n", gshowcalc);
        }
    }

    // close the file
    fclose(fp);
        char *args[] = {argv[0], NULL};
    execvp(args[0], args);

    // The code below will only execute if the execvp() call fails
    perror("execvp");

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
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
    gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"menulibre");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    }

    g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
    g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), NULL);
    g_signal_connect(G_OBJECT(webcombo), "changed", G_CALLBACK(on_webcombo_changed), webctm);
    g_signal_connect(defbtn, "clicked", G_CALLBACK(on_default_button_clicked), NULL);

    // End
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show_all(window);
    if (wengine != 3 )
    {
        gtk_widget_hide(webctm);
        gtk_widget_hide(weblabel);
    }

    gtk_main();
return 0;
}
