void on_item_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *null, gpointer user_data)
{
	GtkTreeModel *model = gtk_tree_view_get_model(treeview);
	GtkTreeIter iter;

	if (gtk_tree_model_get_iter(model, &iter, path))
	{
		gchar *app_name = NULL;
		gchar *toexec = NULL;
		gchar *icon_name = NULL;

		gtk_tree_model_get(model, &iter, 0, &app_name, 1, &toexec, 2, &icon_name, -1);

		if (toexec)
		{

		char *percentPos = strchr(toexec, '%');
			if (percentPos != NULL)
			{
				int length = percentPos - toexec;
				toexec[length] = '\0';
			}

			GError *error = NULL;
			GPid pid;
			gboolean success = g_spawn_async_with_pipes(NULL,
			(gchar *[]){"/bin/sh", "-c", toexec, NULL}, NULL, G_SPAWN_SEARCH_PATH,
			NULL, NULL, &pid, NULL, NULL, NULL, &error);

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
		else
		{
			g_print("No command to execute.\n");
		}

		g_free(app_name);
		g_free(toexec);
		g_free(icon_name);
	}
	else
	{
		g_print("Failed to get iterator for path.\n");
	}
}

void on_run_command(GtkWidget *widget, GdkEventButton *event, GtkWidget *input)
{
	const char *text = gtk_entry_get_text(GTK_ENTRY(input));
	GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(listbox2));
	if ((void*)selected_row == (void*)cmd_row) 
	{
		int found = 0;
		char *path = getenv("PATH"), *path_copy = strdup(path), *dir = strtok(path_copy, ":");

		while (dir != NULL && !found) 
		{
			char *terminal = getenv("TERMINAL");
			if (terminal == NULL) 
			{
				printf("TERMINAL environment variable not set\n");

				dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
					"TERMINAL environment variable not set. \n You can declare it in /etc/environment or in your ~/.profile \n E.G.: TERMINAL=urxvt");

				gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);

				return;
			}

			char *full_path = malloc(strlen(dir) + strlen(text) + 2);
			sprintf(full_path, "%s/%s", dir, text);

			if (access(full_path, F_OK) == 0) 
			{
				found = 1;
				printf("File %s found in %s\n", text, dir);

				char *cmd = g_strdup_printf("%s -e %s", terminal, full_path);
				GError *error = NULL;
				g_spawn_command_line_async(cmd, &error);
				g_print("%s", cmd);
				
				if (error != NULL) 
				{
					printf("Error launching command: %s\n", error->message);
					g_error_free(error);
				}
				g_free(cmd);
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

			GError *error = NULL;
			gboolean success = g_spawn_async(NULL, args, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, &error);
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
		g_print("%s", command);
		system(command);
		gtk_main_quit();
	}
}
