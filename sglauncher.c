#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 64

const char* app_dirs[] = {"/usr/share/applications", "/usr/share/sglauncher", NULL};
const char* quick_dirs[] = {NULL};
char *webengine;
GtkWidget *cmd_row;
GtkWidget *web_row;
GtkWidget *entry;
GtkWidget *label;
GtkWidget *mathtext;
GtkWidget *listbox2;
GtkWidget *pr;
GtkWidget *row;

double evaluate(char* expression) 
{

               gtk_widget_show(mathtext);
    char* endptr;
    double result = strtod(expression, &endptr);
    char op;
    double operand;

    while (*endptr != '\0') 
    {
        op = *endptr++;
        operand = strtod(endptr, &endptr);

        switch (op) 
        {
            case '+': result += operand; break;
            case '-': result -= operand; break;
            case '*': result *= operand; break;
            case '/': result /= operand; break;
            case '^': result = pow(result, operand); break;
            case '%': result = operand / 100.0 * result; break;
            case 'r': result = sqrt(result); break;
            case 'c': result = cos(result); break;
            case 't': result = tan(result); break;
            case 's': result = sin(result); break;
        }
    }
    return result;
}
void on_item_activated(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data) 
{
    const gchar *filename = gtk_widget_get_name(GTK_WIDGET(row)); 
    gchar *command = NULL;
    gchar *line = NULL;
    size_t len = 0;
    FILE *file = fopen(filename, "r");
    while (getline(&line, &len, file) != -1) 
    {
        if (strstr(line, "Exec=") == line) 
        {
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
            g_warning("Failed to start program: %s", error->message);
            g_error_free(error);
            
                // Split the command string into separate arguments
    gchar **args = g_strsplit(command, " ", -1);

    // Spawn a new process asynchronously with the command and its arguments
    GError *error = NULL;
    gboolean success = g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error);
    if (!success) 
    {
        g_warning("Failed to launch process: %s", error->message);
        g_error_free(error);
    }
        }
        else
        {
            g_spawn_close_pid(pid);
            gtk_main_quit();
        }
        g_free(line);
}
}

void load_apps(GtkListBox *listbox) 
{
    for (int i = 0; i < sizeof(app_dirs)/sizeof(app_dirs[0]); i++)
     {
        DIR *dir = opendir(app_dirs[i]);
        if (dir != NULL) 
        {
            struct dirent *ent;
            while ((ent = readdir(dir)) != NULL) 
            {
                if (ent->d_name[0] == '.') 
                {
                    continue;
                }
                gchar *path = g_strdup_printf("%s/%s", app_dirs[i], ent->d_name);
                FILE *file = fopen(path, "r");
                if (file != NULL) 
                {
                    gchar *line = NULL;
                    size_t len = 0;
                    gchar *icon_name = NULL; // to store icon name
                    gchar *app_name = NULL; // to store app name

                    while (getline(&line, &len, file) != -1) 
                    {
                        if (strstr(line, "GenericName=") == line) 
                        {
                            // Remove trailing newline from name
                            gchar *pos = strchr(line + 12, '\n');
                            if (pos != NULL) 
                            {
                                *pos = '\0';
                            }
                            if (strlen(line + 12) > 0) 
                            {
                                app_name = g_strdup(line + 12);
                            }
                        } else if (strstr(line, "Name=") == line) 
                        {
                            // Use Name if GenericName is not available or empty
                            gchar *pos = strchr(line + 5, '\n');
                            if (pos != NULL) 
                            {
                                *pos = '\0';
                            }
                            if (strlen(line + 5) > 0 && app_name == NULL) 
                            {
                                app_name = g_strdup(line + 5);
                            }
                        } else if (strstr(line, "Icon=") == line) 
                        {
                            // Extract icon name from the desktop entry file
                            gchar *pos = strchr(line + 5, '\n');
                            if (pos != NULL) 
                            {
                                *pos = '\0';
                            }
                            icon_name = g_strdup(line + 5);
                        }
                    }
                                      

                    // Create a GtkListBoxRow with a GtkBox container
                    row = gtk_list_box_row_new();
                    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
                    gtk_container_add(GTK_CONTAINER(row), box);
                    
                    ///create an query with the values, and then set is as name of row (for search)
                    char query[256];
                    sprintf(query, "%s%s%s", app_name, path, icon_name);
                    gtk_widget_set_name(row, query);

                    // Create a GtkImage widget and set its icon with a size limit of 32x32
                    GtkWidget *icon = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_BUTTON);
                    if (icon == NULL) 
                    {
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
            }
        }
        closedir(dir);
    }
 
         // Create a scrolled window and add the list box to it
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(listbox));
       
}

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
            fprintf(stderr, "Error: could not open default configuration file /etc/seconfig.conf, please reinstall the program or put a config file in ~/.config/sglauncher.conf.\n");
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
    printf("WebEngine: %s\nShowAppsFirst: %d\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\n", webengine, order, showcmd, showweb, showcalc);
    ////////////////////////////////////////
    
    
    gtk_init(&argc, &argv);
    GtkWidget *dialog;

    char local_app_dir[1024] = "";
    sprintf(local_app_dir, "%s/.local/share/applications", home_dir);
    sprintf(local_app_dir, "%s/.local/share/sglauncher/quickaccess", home_dir);
    app_dirs[2] = local_app_dir;
    quick_dirs[0] = local_app_dir;
    
    GtkWidget *grid;
listbox2 = gtk_list_box_new();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "SGLauncher");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 333, 333);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
            GtkIconTheme *theme = gtk_icon_theme_get_default();
    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, "menulibre", 48, 0);
    if (info != NULL) 
    {
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

    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
    GtkWidget *wtitle = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(wtitle), "<b>SGLauncher</b>");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);
    gtk_widget_set_hexpand(GTK_WIDGET(entry), TRUE);
    gtk_widget_set_vexpand(GTK_WIDGET(entry), TRUE);

    gtk_header_bar_pack_end(GTK_HEADER_BAR(headerbar), entry);
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
GtkWidget *cmd_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
gtk_container_add(GTK_CONTAINER(cmd_row), cmd_box);

// Create a GtkImage widget and set its icon with a size limit of 32x32
GtkWidget *cmdicon = gtk_image_new_from_icon_name("terminal-tango", GTK_ICON_SIZE_BUTTON);
gtk_box_pack_start(GTK_BOX(cmd_box), cmdicon, FALSE, FALSE, 0);

// Create a GtkLabel widget for the app name and set its text
GtkWidget *cmdrun = gtk_label_new("Run in Terminal");
gtk_box_pack_start(GTK_BOX(cmd_box), cmdrun, FALSE, FALSE, 0);

web_row = gtk_list_box_row_new();
GtkWidget *web_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
gtk_container_add(GTK_CONTAINER(web_row), web_box);

// Create a GtkImage widget and set its icon with a size limit of 32x32
GtkWidget *webicon = gtk_image_new_from_icon_name("edit-find", GTK_ICON_SIZE_BUTTON);
gtk_box_pack_start(GTK_BOX(web_box), webicon, FALSE, FALSE, 0);

// Create a GtkLabel widget for the app name and set its text
GtkWidget *webrun = gtk_label_new("Search on Web");
gtk_box_pack_start(GTK_BOX(web_box), webrun, FALSE, FALSE, 0);

// Add the new GtkListBoxRow to the GtkListBox

if (showcmd == 1)
{
gtk_list_box_insert(GTK_LIST_BOX(listbox2), cmd_row, -1);
}

if (showweb == 1)
{
gtk_list_box_insert(GTK_LIST_BOX(listbox2), web_row, -1);
}
gtk_widget_set_size_request(web_row, -1, 32);
gtk_widget_set_size_request(cmd_row, -1, 32);


    
    
void filter_listbox(GtkEntry *entry, GtkListBox *listbox) 
{
    
    const gchar *text = gtk_entry_get_text(entry);
    GList *children = gtk_container_get_children(GTK_CONTAINER(listbox));
    GList *iter;
    for (iter = children; iter != NULL; iter = iter->next) 
    {
        
        GtkWidget *row = iter->data;
        const gchar *name = gtk_widget_get_name(row);
        if (name != NULL && strstr(name, text) != NULL) 
        {
            gtk_widget_show(row);
            gtk_widget_hide(listbox2);
            gtk_widget_hide(mathtext);
            gtk_widget_hide(pr);
        } else 
        {
            gtk_widget_hide(row);
            gtk_widget_show(listbox2);
        }
    }
    
       if (strlen(text) > 0 && isdigit(text[0])) 
    {
        double result = evaluate((char*)text);
        char buffer[256];
        snprintf(buffer, 256, "%g", result);
        gtk_label_set_text(GTK_LABEL(label), buffer);
        gtk_widget_hide(pr);
    } else if  (strlen(text) > 0 && !isdigit(text[0])) 
    {
        gtk_label_set_text(GTK_LABEL(label), "");
        gtk_widget_show(pr);
    }
    
}


void on_run_command(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry)
{
        const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        
           GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox2));
        if ((void*)selected_row == (void*)cmd_row) 
    {
        
        int found = 0;
        char *path = getenv("PATH");
        char *path_copy = strdup(path);
        char *dir = strtok(path_copy, ":");

        while (dir != NULL && !found) 
        {
            char *full_path = malloc(strlen(dir) + strlen(text) + 2);
            sprintf(full_path, "%s/%s", dir, text);

            if (access(full_path, F_OK) == 0) 
            {
                found = 1;
                printf("File %s found in %s\n", text, dir);

                char *terminal = getenv("TERMINAL");
                if (terminal != NULL) 
                {
                    char *cmd = g_strdup_printf("%s -e %s", terminal, full_path);
                    GError *error = NULL;
                    g_spawn_command_line_async(cmd, &error);
                    printf(cmd);
                    if (error != NULL) 
                    {
                        printf("Error launching command: %s\n", error->message);
                        g_error_free(error);
                    }
                    g_free(cmd);
                } else 
                {
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

        if (!found) 
        {

            char *msg = g_strdup_printf("Error: Command '%s' not found", text);
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", msg);
            g_free(msg);

            gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
    else if ((void*)selected_row == (void*)web_row)
    {
 
           char command[256];
            sprintf(command, "xdg-open %s%s", webengine, text);

    // execute the command
    printf(command);
    system(command);
    gtk_main_quit();
    }
}



       void on_submenu_item1_selected(GtkMenuItem *menuitem, gpointer userdata) 
    {
        system("./sglauncher-config");
    
    }


       void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
    {
        dialog = gtk_about_dialog_new();

    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGLauncher");
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2023 ItzSelenux for Simple GTK Desktop Environment");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Simple GTK Launcher");
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/sglauncher");
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
    gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_MIT_X11);
    gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"menulibre");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    }
    
    
gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
    const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
        
    if (event->keyval == GDK_KEY_Escape) 
    {
        gtk_main_quit();
        return TRUE;
    }
    else if(event->keyval == GDK_KEY_Return)
    {
           if (strlen(text) > 0 && !isdigit(text[0])) 
    {
       GError *error = NULL;
        GPid pid;
        gint exit_status = 0;
        gboolean success = g_spawn_async_with_pipes(NULL,
                                                    (gchar * []) {(gchar *) text, NULL},
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
            g_warning("Failed to start program: %s", error->message);
            g_error_free(error);
        }
        else if (success)
        {
            gtk_main_quit();
            g_spawn_close_pid(pid);
        }
    }
    }
    else if(event->keyval == GDK_KEY_Down && gtk_widget_has_focus(entry))
    {
        gtk_widget_grab_focus(row);
        gtk_widget_set_state_flags(row, GTK_STATE_FLAG_SELECTED, TRUE);
    }
    return FALSE;
}




 g_signal_connect(window, "key-release-event", G_CALLBACK(on_key_release), row);
    g_signal_connect(submenu_item1, "activate", G_CALLBACK(on_submenu_item1_selected), NULL);
    g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
   g_signal_connect(listbox2, "row-activated", G_CALLBACK(on_run_command), entry);
    //g_signal_connect(cmd_row, "activate", G_CALLBACK(on_run_command), entry); 
    g_signal_connect(listbox, "row-activated", G_CALLBACK(on_item_activated), NULL);
    g_signal_connect(entry, "changed", G_CALLBACK(filter_listbox), listbox);
    
    
    
    // Load apps into the list box
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);
// Set the vertical expand of the scrolled window to fill the available space

    mathtext = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    GtkWidget *math = gtk_label_new("Math Answer: ");
    label = gtk_label_new("");
    pr = gtk_label_new("Press Enter to Run entry on background");

        if (showcalc == 1)
        {
        gtk_box_pack_start(GTK_BOX(mathtext), math, TRUE, TRUE, 0);
        gtk_box_pack_start(GTK_BOX(mathtext), label, TRUE, TRUE, 0);
        }

gtk_widget_set_vexpand(scrolled_window, TRUE);
gtk_widget_set_hexpand(scrolled_window, TRUE);
// Set the size request of the listbox to have a fixed height of 64 pixels


        if (order == 0)
        {
        gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 4, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 3, 1, 1);
        }
        else if (order == 1)
        {
        gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 4, 1, 1);
        }
        else if (order == 2)
        {
        gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), listbox2, 1, 3, 1, 1);
        }
        else if (order == 3)
        {
        gtk_grid_attach(GTK_GRID(grid), scrolled_window, 1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), listbox2, 0, 3, 1, 1);
        }
        
        gtk_widget_set_size_request(listbox2, -1, -1);
        gtk_widget_set_size_request(scrolled_window, -1, 256);
    gtk_grid_attach(GTK_GRID(grid), pr, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), mathtext, 0, 2, 1, 1);


/*
GtkWidget *focus_chain[] = { scrolled_window, listbox2, NULL };
gtk_container_set_focus_child(GTK_CONTAINER(grid), focus_chain[0]);
*/
gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    load_apps(GTK_LIST_BOX(listbox));
    gtk_widget_show_all(window);
gtk_widget_hide(mathtext);
gtk_widget_hide(listbox2);
gtk_widget_hide(pr);
    gtk_main();
    return 0;
}

