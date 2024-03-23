#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#define ML 256

const gchar *cweb, *cwebng, *corder, *active_text; 
const char* app_dirs[] = {"/usr/share/applications", "", NULL};
const char* quick_dirs[] = {NULL};
const char* pver = mver;
char *pm, *webengine, *cwengine, cengine[ML], *home_dir, config_file_path[256];
int wengine, order, showweb, showcmd, showcalc, showda;

gboolean gshowcmd, gshowcalc, gshowweb, gshowda;

GtkWidget *window, *grid, *cmd_row, *dialog, *web_row, *entry, *manswer, *mathtext, *listbox2,
*pr, *row, *headerbar, *button, *image, *wtitle, *submenu, *submenu_item1, *submenu_item2,
*submenu_item3, *submenu_item4, *submenu_item5, *weblabel, *webcombo, *webctm, *worder,
*wshowcmd, *wshowweb, *wshowcalc, *defbtn, *applybtn, *listbox, *web_box, *wshowda;

GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

void restart_program(GtkWidget *widget, gpointer data)
{
	printf("Program Reloaded...\n");
	char *args[] = {pm, NULL};
	execvp(args[0], args);
}

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
	const gchar *widget_name = gtk_widget_get_name(GTK_WIDGET(row));
	gchar *filename;
	gchar *action_name;

	gchar *comma_position = strchr(widget_name, ',');
	if (comma_position)
	{
		gchar *start_position = strchr(comma_position, '[');

		if (start_position)
		{
			action_name = g_strdup(start_position + 1);
			filename = g_strndup(comma_position + 1, start_position - comma_position - 1);
		} 
		else
		{
			filename = g_strdup(comma_position + 1);
			action_name = g_strdup("");
		}
	}
	else
	{
		filename = g_strdup(widget_name);
		action_name = g_strdup("");
	}

	FILE *file = fopen(filename, "r");
	g_free(filename);

	if (!file)
	{
		g_warning("Failed to open file: %s", filename);
		return;
	}

	gchar *line = NULL;
	size_t len = 0;

	gboolean within_desired_content = FALSE;

	while (getline(&line, &len, file) != -1)
	{
		if (g_strstr_len(line, -1, g_strdup_printf("Name=%s", action_name)))
		{
			within_desired_content = TRUE;
		}
		if (within_desired_content && g_str_has_prefix(line, "Exec="))
		{
			gchar *command = g_strchomp(line + 5);
			gchar *trimmed_command = g_strstrip(command);
			size_t command_len = strlen(trimmed_command);
			if (command_len >= 2 && (trimmed_command[command_len - 2] == '%' || trimmed_command[command_len - 2] == ' '))
			{
				trimmed_command[command_len - 2] = '\0';
			}

			gchar **parts = g_strsplit(trimmed_command, "=", -1);

			for (gchar **part = parts; *part; ++part)
			{
				gchar *var_name = g_strdup_printf("PART%d", part - parts);
				g_setenv(var_name, *part, TRUE);
				g_free(var_name);
			}

			g_strfreev(parts);

			GError *error = NULL;
			GPid pid;
			gboolean success = g_spawn_async_with_pipes(NULL,
				(gchar * []) {"/bin/sh", "-c", trimmed_command, NULL},
				NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL, NULL, NULL, &error);

			if (!success)
			{
				g_warning("Failed to start program: %s", error->message);
				g_error_free(error);
				g_free(command);
			}
			else
			{
				g_spawn_close_pid(pid);
				gtk_main_quit();
			}
			break;
		}
	}

	fclose(file);
	g_free(line);
}


typedef struct
{
	gchar *name;
	gchar *icon;
	gchar *path;
}AppDetails;

int compare_app_details(const void *a, const void *b)
{
	const AppDetails *app1 = (const AppDetails *)a;
	const AppDetails *app2 = (const AppDetails *)b;
	return strcmp(app1->name, app2->name);
}

void load_apps(GtkListBox *listbox)
{
	AppDetails *app_details = NULL;
	int num_apps = 0;

	for (int i = 0; i < sizeof(app_dirs) / sizeof(app_dirs[0]); i++)
	{
		DIR *dir = opendir(app_dirs[i]);
		if (dir != NULL)
		{
			struct dirent *ent;
			while ((ent = readdir(dir)) != NULL)
			{
				if (ent->d_name[0] == '.' || strstr(ent->d_name, ".desktop") == NULL)
				{
					continue;
				}

				gchar *path = g_strdup_printf("%s/%s", app_dirs[i], ent->d_name);
				FILE *file = fopen(path, "r");
				if (file != NULL)
				{
					gchar *line = NULL;
					size_t len = 0;
					gchar *icon_name = NULL;
					gchar *app_name = NULL;
					gboolean nodisplay = FALSE;
					GtkWidget *row = NULL;

					while (getline(&line, &len, file) != -1)
					{
						if (strstr(line, "Name=") == line && app_name == NULL)
						{
							gchar *pos = strchr(line + 5, '\n');
							if (pos != NULL)
							{
								*pos = '\0';
							}
							if (strlen(line + 5) > 0)
							{
								app_name = g_strdup(line + 5);
							}
						}
						else if (strstr(line, "Icon=") == line)
						{
							gchar *pos = strchr(line + 5, '\n');
							if (pos != NULL)
							{
								*pos = '\0';
							}
							icon_name = g_strdup(line + 5);
						}
						else if (strstr(line, "NoDisplay=") == line)
						{
							nodisplay = TRUE;
						}
						else if (showda && strstr(line, "[Desktop Action") == line)
						{
							gchar *action_name_start = strchr(line, '[') + 1;
							gchar *action_name_end = strchr(line, ']');
							if (action_name_start != NULL && action_name_end != NULL)
							{
								*action_name_end = '\0';
								gchar *action_name = g_strdup(action_name_start);
								getline(&line, &len, file);
								gchar *action_name_line = g_strstrip(line);
								if (strstr(action_name_line, "Name=") == action_name_line)
								{
									gchar *pos = strchr(action_name_line + 5, '\n');
									if (pos != NULL)
									{
										*pos = '\0';
									}
									if (strlen(action_name_line + 5) > 0)
									{
										action_name = g_strdup(action_name_line + 5);
									}
								}

								getline(&line, &len, file);
								gchar *exec_line = g_strstrip(line);
								gchar *exec_value = NULL;
								if (strstr(exec_line, "Exec=") == exec_line)
								{
									gchar *pos = strchr(exec_line + 5, '\n');
									if (pos != NULL)
									{
										*pos = '\0';
									}
									exec_value = g_strdup(exec_line + 5);
								}

								gchar *search_str = g_strdup_printf("%s,%s[%s", app_name ? app_name : "", path, action_name);

								GtkWidget *action_row = gtk_list_box_row_new();
								GtkWidget *action_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
								gtk_container_add(GTK_CONTAINER(action_row), action_box);
								gtk_widget_set_name(action_row, search_str);

								GtkIconTheme *theme = gtk_icon_theme_get_default();
								GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, icon_name, 16, 0);
								GdkPixbuf *licon = gtk_icon_info_load_icon(info, NULL);

								if (licon != NULL)
								{
									GdkPixbuf *resized_icon = gdk_pixbuf_scale_simple(licon, 16, 16, GDK_INTERP_BILINEAR);
									GtkWidget *icon = gtk_image_new_from_pixbuf(resized_icon);
									gtk_box_pack_start(GTK_BOX(action_box), icon, FALSE, FALSE, 0);
									g_object_unref(resized_icon);
									g_object_unref(licon);
								}
								else
								{
									GtkWidget *icon = gtk_image_new_from_icon_name("application-x-executable", GTK_ICON_SIZE_BUTTON);
									gtk_box_pack_start(GTK_BOX(action_box), icon, FALSE, FALSE, 0);
								}

								GtkWidget *label = gtk_label_new(action_name);
								gtk_box_pack_start(GTK_BOX(action_box), label, FALSE, FALSE, 0);
								gtk_list_box_insert(GTK_LIST_BOX(listbox), action_row, -1);
								gtk_widget_set_size_request(action_box, -1, 32);

								g_free(search_str);
								g_free(action_name);
								g_free(exec_value);
							}
						}
					}

					if (!nodisplay && app_name != NULL)
					{
						app_details = realloc(app_details, (num_apps + 1) * sizeof(AppDetails));
						app_details[num_apps].name = app_name;
						app_details[num_apps].icon = icon_name;
						app_details[num_apps].path = path;
						num_apps++;
					}
					else
					{
						g_free(app_name);
						g_free(icon_name);
						g_free(path);
					}

					fclose(file);
				}
			}
		}
		closedir(dir);
	}

	qsort(app_details, num_apps, sizeof(AppDetails), compare_app_details);

	for (int i = 0; i < num_apps; i++)
	{
		GtkWidget *row = gtk_list_box_row_new();
		GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		gtk_container_add(GTK_CONTAINER(row), box);

		GdkPixbuf *icon_pixbuf = NULL;
		if (app_details[i].icon != NULL && strstr(app_details[i].icon, "/") != NULL)
		{
			icon_pixbuf = gdk_pixbuf_new_from_file(app_details[i].icon, NULL);
		}
		else if (app_details[i].icon != NULL)
		{
			GtkIconTheme *theme = gtk_icon_theme_get_default();
			GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, app_details[i].icon, 16, 0);
			if (info != NULL)
			{
				icon_pixbuf = gtk_icon_info_load_icon(info, NULL);
				g_object_unref(info);
			}
		}

		GtkWidget *icon;
		if (icon_pixbuf != NULL)
		{
			GdkPixbuf *resized_icon = gdk_pixbuf_scale_simple(icon_pixbuf, 16, 16, GDK_INTERP_BILINEAR);
			icon = gtk_image_new_from_pixbuf(resized_icon);
			g_object_unref(resized_icon);
			g_object_unref(icon_pixbuf);
		}
		else
		{
			icon = gtk_image_new_from_icon_name("application-x-executable", GTK_ICON_SIZE_BUTTON);
		}
		gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);

		GtkWidget *label = gtk_label_new(app_details[i].name);
		gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
		gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
		gtk_widget_set_size_request(row, -1, 32);
		char query[4096];
			sprintf(query, "%s%s%s", app_details[i].name ? app_details[i].name : "", app_details[i].path, app_details[i].icon ? app_details[i].icon : "");
			gchar *search_str2 = g_strdup_printf("%s%s,%s[", app_details[i].name ? app_details[i].name : "", app_details[i].icon, app_details[i].path);
			gtk_widget_set_name(row, search_str2);
	}
	
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(listbox));
}

void on_submenu_item3_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "SGLauncher");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2024 ItzSelenux for Simple GTK Desktop Environment");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Simple GTK Launcher");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/sglauncher");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"menulibre");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void on_submenu_item2_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Help");
		gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
		gtk_container_set_border_width(GTK_CONTAINER(window), 10);
		GtkWidget *label = gtk_label_new("- SGLauncher is a quick launcher that can run programs, desktop actions, run on terminal or search on web\n - You can run the first item on the list by pressing enter, or press down to select other item \n - You can press CTRL+T to run on terminal and CTRL+B to Search on Web\n - You can customize the program in the Settings or editing the config file on ~/.config/sglauncher.conf");
	gtk_container_add(GTK_CONTAINER(window), label);
	gtk_widget_show_all(window);
}

void on_submenu_item1_selected(GtkMenuItem *menuitem, gpointer userdata) 
{
	if (access("/usr/bin/sglauncher-cfg", F_OK) == 0) 
		system("/usr/bin/sglauncher-cfg");
	else if (access("./sglauncher-cfg", F_OK) == 0) 
		system("./sglauncher-cfg");
	else 
		printf("\033[1;31mERROR\033[0m: sglauncher-cfg not detected, please reinstall this program\n");
}

void on_run_command(GtkWidget *widget, GdkEventButton *event, GtkWidget *entry)
{
	const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
	GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox2));
	if ((void*)selected_row == (void*)cmd_row) 
	{
		int found = 0;
		char *path = getenv("PATH"), *path_copy = strdup(path), *dir = strtok(path_copy, ":");

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
				}
				else 
				{
					printf("TERMINAL environment variable not set\n");

					GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "ERROR: TERMINAL environment variable not set. \n you can declare it on /etc/environment or your ~/.profile \n E.G.: TERMINAL=sakura");

					gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
					gtk_dialog_run(GTK_DIALOG(dialog));
					gtk_widget_destroy(dialog);
				}
			gtk_main_quit();
			}

			free(full_path);
			dir = strtok(NULL, ":");
		}
		free(path_copy);

		if (!found) 
		{
			GPid pid;
			gchar **args = g_strsplit(text, " ", -1);

			// Spawn a new process asynchronously with the command and its arguments
			GError *error = NULL;
			gboolean success = g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error);
			if (!success) 
			{
				g_warning("Failed to launch process: %s", error->message);
				g_error_free(error);
			}
			else
			{
				g_spawn_close_pid(pid);
				gtk_main_quit();
			}
			gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
		}
	}
	else if ((void*)selected_row == (void*)web_row)
	{
		char command[256];
		sprintf(command, "xdg-open '%s=%s'", webengine, text);

	printf(command);
	system(command);
	gtk_main_quit();
	}
}


GtkWidget* filter_listbox(GtkEntry *entry, GtkListBox *listbox) 
{
	const gchar *text = gtk_entry_get_text(entry);
	GList *children = gtk_container_get_children(GTK_CONTAINER(listbox));
	GList *iter;
	GtkWidget *firstVisibleRow = NULL;

	for (iter = children; iter != NULL; iter = iter->next) 
	{
		GtkWidget *row = GTK_WIDGET(iter->data);
		const gchar *name = gtk_widget_get_name(row);

		if (name != NULL && strcasestr(name, text) != NULL) 
		{
			if (firstVisibleRow == NULL)
			{
				firstVisibleRow = row;
			}

			gtk_widget_show(row);
			gtk_widget_hide(listbox2);
			gtk_widget_hide(mathtext);
			gtk_widget_hide(pr);
		}
		else 
		{
			gtk_widget_hide(row);
			gtk_widget_show(listbox2);
		}
	}

	g_list_free(children);

	if (strlen(text) > 0 && isdigit(text[0])) 
	{
		double result = evaluate((char*)text);
		char buffer[256];
		snprintf(buffer, 256, "%g", result);
		gtk_label_set_text(GTK_LABEL(manswer), buffer);
		gtk_widget_hide(pr);
	}
	else if  (strlen(text) > 0 && !isdigit(text[0])) 
	{
		gtk_label_set_text(GTK_LABEL(manswer), "");
		gtk_widget_show(pr);
	}

	return firstVisibleRow;
}

gboolean on_key_release(GtkWidget *widget, GdkEventKey *event, gpointer user_data) 
{
	const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
	if (event->keyval == GDK_KEY_Escape || (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK))) 
	{
		gtk_main_quit();
		return TRUE;
	}
	else if (event->keyval == GDK_KEY_Escape || (event->keyval == GDK_KEY_m && (event->state & GDK_CONTROL_MASK))) 
	{
		if (strlen(text) > 0 && !isdigit(text[0])) 
		{
			GError *error = NULL;
			GPid pid;
			gint exit_status = 0;
			gboolean success = g_spawn_async_with_pipes(NULL,
				(gchar * []) {(gchar *) text, NULL},
				NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL, NULL, NULL, &error);
			if (!success)
			{
				g_warning("Failed to start program: %s", error->message);
				gchar **args = g_strsplit(text, " ", -1);

				// Spawn a new process asynchronously with the command and its arguments
				GError *error = NULL;
				gboolean success = g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, NULL, &error);
				if (!success) 
				{
					g_warning("Failed to launch process: %s", error->message);
					g_error_free(error);
				}
				else
				{
					g_spawn_close_pid(pid);
					gtk_main_quit();
				}
			}
			else if (success)
			{
				gtk_main_quit();
				g_spawn_close_pid(pid);
			}
		}
	}
	else if(event->keyval == GDK_KEY_Return && gtk_widget_has_focus(entry))
	{
		GtkWidget *firstVisibleRow = filter_listbox(GTK_ENTRY(entry), GTK_LIST_BOX(listbox));
		if (firstVisibleRow != NULL)
		{
		gtk_widget_activate(firstVisibleRow);
		}
		else
		{
		gtk_widget_grab_focus(GTK_WIDGET(entry));
		}
	}
	else if(event->keyval == GDK_KEY_Down && gtk_widget_has_focus(entry))
	{
		GtkWidget *firstVisibleRow = filter_listbox(GTK_ENTRY(entry), GTK_LIST_BOX(listbox));
		if (firstVisibleRow != NULL)
		{
		gtk_widget_grab_focus(firstVisibleRow);
		}
		else
		{
		gtk_widget_grab_focus(GTK_WIDGET(entry));
		}
	}
	else if((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_b))
	{
		gtk_widget_activate(web_row);
	}
	else if((event->state & GDK_CONTROL_MASK) && (event->keyval == GDK_KEY_t))
	{
		gtk_widget_activate(cmd_row);
	}

	return FALSE;
}

gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	if (event->type == GDK_BUTTON_PRESS && event->button == 3)
	{
		GtkWidget *submenu = GTK_WIDGET(data);
		gtk_menu_popup_at_pointer(GTK_MENU(submenu), NULL);
		return TRUE;
	}
	return FALSE;
}

void readconf()
{
	//READ THE CONF
	char *cwengine;
	cwengine = " ";
	if (home_dir == NULL)
	{
		fprintf(stderr, "Error: HOME environment variable is not set.\n");
		exit(1);
	}

	snprintf(config_file_path, sizeof(config_file_path), "%s/.config/sglauncher.conf", home_dir);

	FILE *file = fopen(config_file_path, "r");
	if (file == NULL)
	{
		if (file == NULL)
		{
		FILE *default_conf = fopen("/etc/sglauncher.conf", "r");
		if (default_conf == NULL)
		{
			fprintf(stderr, "Error: could not open default configuration file /etc/sglauncher.conf, please reinstall the program or put a config file in ~/.config/sglauncher.conf.\n");
			exit(1);
		}

		file = fopen(config_file_path, "w");
		if (file == NULL)
		{
			fprintf(stderr, "Error: could not create %s for writing.\n", config_file_path);
			exit(1);
		}

		int ch;
		while ((ch = fgetc(default_conf)) != EOF)
		{
			fputc(ch, file);
		}

		fclose(default_conf);
		printf("Default configuration file copied to %s.\n", config_file_path);
		}
		else
		{
			fclose(file);
			printf("%s exists and can be read.\n", config_file_path);
		}
	}

	char line[ML];
	if (file != NULL)
	{
		// Read each line from the file and parse the variable assignments
		while (fgets(line, ML, file) != NULL)
		{
			char *name = strtok(line, "=");
			char *value_str = strtok(NULL, "=");

			if (name != NULL && value_str != NULL)
			{
				if (strcmp(name, "order") == 0) 
					order = atoi(value_str);
				else if (strcmp(name, "cengine") == 0) 
					strncpy(cengine, value_str, sizeof(cengine));
				else if (strcmp(name, "wengine") == 0)
					wengine = atoi(value_str);
				else if (strcmp(name, "showweb") == 0) 
					showweb = atoi(value_str);
				else if (strcmp(name, "showcmd") == 0) 
					showcmd = atoi(value_str);
				else if (strcmp(name, "showcalc") == 0) 
					showcalc = atoi(value_str);
				else if (strcmp(name, "showda") == 0) 
					showda = atoi(value_str);
			}
		}
		fclose(file);
	}
	else
	{
		printf("Error opening file");
	}

	if (wengine == 0 )
		webengine = "https://duckduckgo.com/?q";
	else if (wengine == 1 )
		webengine = "https://www.google.com/search?q";
	else if (wengine == 2 )
		webengine = "https://www.bing.com/search?q";
	else if (wengine == 3 )
		webengine = cengine;

	// Use the values that were read from the file
	printf("WebEngine: %s\nOrder: %d\nShowDA: %d\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\n", webengine, order, showda, showcmd, showweb, showcalc);
}

void on_webcombo_changed(GtkComboBox *webcombo, gpointer user_data)
{
	active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo));
	GtkWidget *webctm = GTK_WIDGET(user_data);
	GtkWidget *webctmlb = GTK_WIDGET(g_object_get_data(G_OBJECT(webctm), "webctmlb"));

	if (g_strcmp0(active_text, "Custom") != 0)
	{
		gtk_widget_hide(webctm);
		gtk_widget_hide(weblabel);
	}
	else
	{
		gtk_widget_show(webctm);
		gtk_widget_show(weblabel);
	}

	g_free((gpointer) active_text);
}


void on_default_button_clicked(GtkButton *button, gpointer user_data)
{
	dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Are you sure you want to restore SGLauncher settings as default?");
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
	gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
	gint result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_OK)
	{
		char *home_dir = getenv("HOME");
		if (home_dir == NULL)
		{
			fprintf(stderr, "Error: HOME environment variable is not set.\n");
			exit(1);
		}
		char *source_file = "/etc/sglauncher.conf";
		char *destination_file = "/.config/sglauncher.conf";
		char command[100];
		sprintf(command, "cp %s %s%s", source_file, home_dir, destination_file);
		system(command);
		restart_program(NULL, pm);

	}
	else
	{
		printf("Operation cancelled.\n");
	}
	gtk_widget_destroy(dialog);
}

void on_apply_button_clicked(GtkButton *button, gpointer user_data) 
{
	gshowcmd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcmd));
	gshowweb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowweb));
	gshowcalc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcalc));
	gshowda = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowda));
	const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo)),
		*active_order = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(worder)),
		*ncengine,
		*entry_text = gtk_entry_get_text(GTK_ENTRY(webctm)),
		*placeholder_text = gtk_entry_get_placeholder_text(GTK_ENTRY(webctm));

	if (entry_text && strlen(entry_text) > 0)
		ncengine = entry_text;
	else
		ncengine = placeholder_text;

	g_print(ncengine);
	cweb = NULL;
	cwebng = gtk_entry_get_text(GTK_ENTRY(webctm));
	corder = NULL;
	if (active_text != NULL) 
	{
		if (g_strcmp0(active_text, "Google") == 0) 
			cweb = "1";
		else if (g_strcmp0(active_text, "DuckDuckGo") == 0) 
			cweb = "0";
		else if (g_strcmp0(active_text, "Bing") == 0) 
			cweb = "2";
		else if (g_strcmp0(active_text, "Custom") == 0) 
			cweb = "3";

		if (active_order != NULL) 
		{
			if (g_strcmp0(active_order, "Horizontal - Apps at bottom") == 0) 
				corder = "1";
			else if (g_strcmp0(active_order, "Horizontal - Apps at top") == 0) 
				corder = "0";
			else if (g_strcmp0(active_order, "Vertical - Apps at left") == 0) 
				corder = "2";
			else if (g_strcmp0(active_order, "Vertical - Apps at right") == 0) 
				corder = "3";
		}
	}
	
	FILE *fp = fopen(config_file_path, "w+");
	if (fp == NULL)
	{
		perror("Failed to open config file");
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "[SGLauncher Configuration File]\n");
	fprintf(fp, "cengine=%s\n", ncengine);
	fprintf(fp, "wengine=%s\n", cweb);
	fprintf(fp, "order=%s\n", corder);
	fprintf(fp, "showda=%d\n", gshowda);
	fprintf(fp, "showweb=%d\n", gshowweb);
	fprintf(fp, "showcmd=%d\n", gshowcmd);
	fprintf(fp, "showcalc=%d\n", gshowcalc);

	fclose(fp);
	perror("execvp");
	restart_program(NULL, pm);
}