void on_item_activated(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data) 
{
	const gchar *widget_name = gtk_widget_get_name(GTK_WIDGET(row));
	gchar *filename;
	gchar *action_name;
	gchar *toexec;
	gchar *src = (gchar *)widget_name;
	gchar *dst = src;

	while (*src)
	{
		if (*src == '%' && *(src + 1) != '\0')
		{
			src += 2;
		} 
		else
		{
			*dst++ = *src++;
		}
	}
	*dst = '\0';

	gchar *comma_position = strchr(widget_name, ',');

	if (comma_position)
	{
		gchar *first_comma_position = strchr(widget_name, ',');

		if (first_comma_position)
		{
			gsize length = first_comma_position - widget_name;
			toexec = g_strndup(widget_name, length);
		}
		else
		{
			toexec = g_strdup(widget_name);
		}
	}
	else
	{
		toexec = g_strdup(widget_name);
	}

	GError *error = NULL;
	GPid pid;
	gboolean success = g_spawn_async_with_pipes(NULL,
		(gchar * []) {"/bin/sh", "-c", toexec, NULL},
		NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL, NULL, NULL, &error);

	g_print(toexec);
	if (!success)
	{
		g_warning("Failed to start program: %s", error->message);
		g_error_free(error);
	}
	else
	{
		g_spawn_close_pid(pid);
		gtk_main_quit();
	}
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