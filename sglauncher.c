#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
const char* app_dir = "/usr/share/applications";
GtkWidget *cmd_row;
GtkWidget *entry;

void on_item_activated(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data) {
    const gchar *filename = gtk_widget_get_name(GTK_WIDGET(row)); 
    gchar *command = NULL;
    gchar *line = NULL;
    size_t len = 0;
    FILE *file = fopen(filename, "r");
    while (getline(&line, &len, file) != -1) {
        if (strstr(line, "Exec=") == line) {
            command = line + 5;
            break;
        }
    }
    fclose(file);
if (command != NULL) {
    gchar *pos = strchr(command, '\n');
    if (pos != NULL) {
        *pos = '\0';
    }
    gchar *cmd= g_strdelimit(command, "%F", '\0');
    gchar *trimmed_command = g_strstrip(cmd);

        GError *error = NULL;
        GPid pid;
        gint exit_status = 0;
        gboolean success = g_spawn_async_with_pipes(NULL,
                                                    (gchar * []) {cmd, NULL},
                                                    NULL,
                                                    G_SPAWN_SEARCH_PATH,
                                                    NULL,
                                                    NULL,
                                                    &pid,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    &error);
        if (!success)
        {
            g_warning("Failed to restart program: %s", error->message);
            g_error_free(error);
        }
        else
        {
            g_spawn_close_pid(pid);
            gtk_main_quit();
        }
        

    }
    g_free(line);
}

void load_apps(GtkListBox *listbox) {
    DIR *dir = opendir(app_dir);
    if (dir != NULL) {
        struct dirent *ent;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] == '.') {
                continue;
            }
            gchar *path = g_strdup_printf("%s/%s", app_dir, ent->d_name);
            FILE *file = fopen(path, "r");
            if (file != NULL) {
                gchar *line = NULL;
                size_t len = 0;

gchar *path = g_strdup_printf("%s/%s", app_dir, ent->d_name);
FILE *file = fopen(path, "r");
if (file != NULL) {
    gchar *line = NULL;
    size_t len = 0;
    gchar *icon_name = NULL; // to store icon name
    gchar *app_name = NULL; // to store app name


while (getline(&line, &len, file) != -1) {
    if (strstr(line, "GenericName=") == line) {
        // Remove trailing newline from name
        gchar *pos = strchr(line + 12, '\n');
        if (pos != NULL) {
            *pos = '\0';
        }
        if (strlen(line + 12) > 0) {
            app_name = g_strdup(line + 12);
        }
    } else if (strstr(line, "Name=") == line) {
        // Use Name if GenericName is not available or empty
        gchar *pos = strchr(line + 5, '\n');
        if (pos != NULL) {
            *pos = '\0';
        }
        if (strlen(line + 5) > 0 && app_name == NULL) {
            app_name = g_strdup(line + 5);
        }
    } else if (strstr(line, "Icon=") == line) {
        // Extract icon name from the desktop entry file
        gchar *pos = strchr(line + 5, '\n');
        if (pos != NULL) {
            *pos = '\0';
        }
        icon_name = g_strdup(line + 5);
    }
}

    // Create a GtkListBoxRow with a GtkBox container
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(row), box);
    gtk_widget_set_name(row, path);
    
    // Create a GtkImage widget and set its icon with a size limit of 32x32
    GtkWidget *icon = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_BUTTON);
    if (icon == NULL) {
        icon = gtk_image_new_from_icon_name("application-x-executable", GTK_ICON_SIZE_BUTTON);
    }
    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
    // Create a GtkLabel widget for the app name and set its text
    GtkWidget *label = gtk_label_new(app_name);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    // Add the GtkListBoxRow to the GtkListBox
    gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
gtk_widget_set_size_request(row, -1,32);
gtk_widget_set_size_request(row, -1, 32);

    g_free(icon_name);
    g_free(app_name);
}
                fclose(file);
                g_free(line);
            }
            g_free(path);
        }
        closedir(dir);
    }
 
         // Create a scrolled window and add the list box to it
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(listbox));
       
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    GtkWidget *grid;
GtkWidget *listbox2 = gtk_list_box_new();




    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "SGLauncher");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 333, 333);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
            GtkIconTheme *theme = gtk_icon_theme_get_default();
    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "menulibre", 48, 0);
    if (info != NULL) {
        GdkPixbuf *icon = gtk_icon_info_load_icon(info, NULL);
        gtk_window_set_icon(GTK_WINDOW(window), icon);
        g_object_unref(icon);
        g_object_unref(info);
    }
    
    // Create the header bar
    GtkWidget *headerbar = gtk_header_bar_new();
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);
    
    // Create the button with an icon
    entry = gtk_entry_new();
    GtkWidget *button = gtk_menu_button_new();
    GtkWidget *image = gtk_image_new_from_icon_name("menulibre", GTK_ICON_SIZE_BUTTON);
    gtk_container_add(GTK_CONTAINER(button), image);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), entry);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
    GtkWidget *wtitle = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wtitle), "<b>SGLauncher</b>");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);

    // Create the submenu
    GtkWidget *submenu = gtk_menu_new();

    // Create the submenu items
    GtkWidget *submenu_item1 = gtk_menu_item_new_with_label("Open Settings");
    GtkWidget *submenu_item3 = gtk_menu_item_new_with_label("About");
    // Add the submenu items to the submenu
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item1);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);

    // Show all the submenu items
    gtk_widget_show_all(submenu);

    // Connect the button to the submenu
    gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);
    

    // Add the header bar to the main window
    gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

    // Create a scrolled window widget and set its properties
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);


    // Create a list box widget and set its properties
    GtkWidget *listbox = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(listbox), GTK_SELECTION_SINGLE);
    gtk_container_add(GTK_CONTAINER(scrolled_window), listbox);
    gtk_widget_grab_focus(entry);
    
    // Create a new GtkListBoxRow
cmd_row = gtk_list_box_row_new();
GtkWidget *new_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
gtk_container_add(GTK_CONTAINER(cmd_row), new_box);

// Create a GtkImage widget and set its icon with a size limit of 32x32
GtkWidget *cmdicon = gtk_image_new_from_icon_name("terminal-tango", GTK_ICON_SIZE_BUTTON);
gtk_box_pack_start(GTK_BOX(new_box), cmdicon, FALSE, FALSE, 0);

// Create a GtkLabel widget for the app name and set its text
GtkWidget *cmdrun = gtk_label_new("Run in Terminal");
gtk_box_pack_start(GTK_BOX(new_box), cmdrun, FALSE, FALSE, 0);

// Add the new GtkListBoxRow to the GtkListBox
gtk_list_box_insert(GTK_LIST_BOX(listbox2), cmd_row, -1);
gtk_widget_set_size_request(cmd_row, -1, 32);









void filter_listbox(GtkEntry *entry, GtkListBox *listbox) {
    const gchar *text = gtk_entry_get_text(entry);
    GList *children = gtk_container_get_children(GTK_CONTAINER(listbox));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) {
        GtkWidget *row = iter->data;
        const gchar *name = gtk_widget_get_name(row);
        if (name != NULL && strstr(name, text) != NULL) {
            gtk_widget_show(row);
            gtk_widget_hide(listbox2);
        } else {
            gtk_widget_hide(row);
               gtk_widget_show(listbox2);
        }
    }
    g_list_free(children);
}


void on_run_command(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry) {
    if (event->type == GDK_BUTTON_PRESS && event->button == GDK_BUTTON_PRIMARY) {
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        
        int found = 0;
        char *path = getenv("PATH");
        char *path_copy = strdup(path);
        char *dir = strtok(path_copy, ":");

        while (dir != NULL && !found) {
            char *full_path = malloc(strlen(dir) + strlen(text) + 2);
            sprintf(full_path, "%s/%s", dir, text);

            if (access(full_path, F_OK) == 0) {
                found = 1;
                printf("File %s found in %s\n", text, dir);

                char *terminal = getenv("TERMINAL");
                if (terminal != NULL) {
                    char *cmd = g_strdup_printf("%s %s", terminal, full_path);
                    GError *error = NULL;
                    g_spawn_command_line_async(cmd, &error);
                    if (error != NULL) {
                        printf("Error launching command: %s\n", error->message);
                        g_error_free(error);
                    }
                    g_free(cmd);
                } else {
                    printf("TERMINAL environment variable not set\n");
                              
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "ERROR: TERMINAL environment variable not set. \n you can declare it on /etc/environment or your ~/.profile \n E.G.: TERMINAL=sakura");

            gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
                }
            }

            free(full_path);
            dir = strtok(NULL, ":");
        }

        free(path_copy);

        if (!found) {
            printf("File %s not found in $PATH\n", text);

            char *msg = g_strdup_printf("Error: Command '%s' not found", text);
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", msg);
            g_free(msg);

            gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}




// Connect signals to the new row
g_signal_connect(listbox2, "button-press-event", G_CALLBACK(on_run_command), entry); 
    g_signal_connect(listbox, "row-activated", G_CALLBACK(on_item_activated), NULL);
    g_signal_connect(entry, "changed", G_CALLBACK(filter_listbox), listbox);
    
    
    
    // Load apps into the list box
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_size_request(listbox2, -1, 32);
    gtk_widget_set_size_request(scrolled_window, -1, 300);
    gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 1, 1, 1);

/*
GtkWidget *focus_chain[] = { scrolled_window, listbox2, NULL };
gtk_container_set_focus_child(GTK_CONTAINER(grid), focus_chain);
*/
    
    
    load_apps(GTK_LIST_BOX(listbox));
    gtk_widget_show_all(window);
    gtk_widget_hide(listbox2);
    gtk_main();



    return 0;
}

