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
		double minscientific = 999999;
		double result = evaluate((char*)text);
		char buffer[256];
		if (result < minscientific)
		{
			snprintf(buffer, 256, "%g", result);
		}
		else if (result > minscientific && showscientific == 0)
		{
			snprintf(buffer, 256, "%f", result);
		}
		else if (result > minscientific && showscientific == 1)
		{
			snprintf(buffer, 256, "%g", result);
		}
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