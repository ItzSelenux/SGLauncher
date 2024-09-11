static gboolean on_filter_visible(GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	FilterData *filter_data = (FilterData *)data;
	gchar *name;
	gboolean visible = FALSE;

	gtk_tree_model_get(model, iter, 3, &name, -1);

	gchar *name_lower = g_utf8_strdown(name, -1);
	gchar *filter_text_lower = g_utf8_strdown(filter_data->filter_text, -1);

	visible = (name_lower && filter_text_lower && g_strrstr(name_lower, filter_text_lower) != NULL);

	if (!visible && gtk_tree_model_iter_has_child(model, iter))
	{
		GtkTreeIter child_iter;
		gboolean valid_child = gtk_tree_model_iter_children(model, &child_iter, iter);
		while (valid_child && !visible)
		{
			visible = on_filter_visible(model, &child_iter, data);
			valid_child = gtk_tree_model_iter_next(model, &child_iter);
		}

		if (visible)
		{
			GtkTreePath *path = gtk_tree_model_get_path(model, iter);
			gtk_tree_view_expand_to_path(filter_data->treeview, path);
			gtk_tree_path_free(path);
		}
	}

	g_free(name_lower);
	g_free(filter_text_lower);
	g_free(name);

	return visible;
}

static void on_entry_changed(GtkEntry *entry, FilterData *filter_data)
{
	if (filter_data->filter == NULL)
	{
		return;
	}

	g_free(filter_data->filter_text);
	filter_data->filter_text = g_strdup(gtk_entry_get_text(entry));

	gtk_tree_model_filter_refilter(filter_data->filter);

	GtkTreeIter iter;
	GtkTreeModel *model = GTK_TREE_MODEL(filter_data->filter);
	gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

	while (valid)
	{
		if (on_filter_visible(model, &iter, filter_data))
		{
			GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
			gtk_tree_view_expand_to_path(filter_data->treeview, path);
			gtk_tree_path_free(path);
		}
		valid = gtk_tree_model_iter_next(model, &iter);
	}

	if (filter_data->filter_text != NULL)
	{
		if (strlen(filter_data->filter_text) > 0 && isdigit(filter_data->filter_text[0])) 
		{
			double minscientific = 999999;
			double result = evaluate((char*)filter_data->filter_text);
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

			const gchar *message = isnan(result) ? "Invalid operation" : buffer;
			gtk_label_set_text(GTK_LABEL(manswer), message);

			gtk_widget_show(mathtext);
			gtk_widget_show(math);
			gtk_widget_show(manswer);
			gtk_widget_hide(pr);
		}
		else if (strlen(filter_data->filter_text) > 0 && !isdigit(filter_data->filter_text[0])) 
		{
			gtk_widget_show(mathtext);
			gtk_widget_hide(math);
			gtk_widget_hide(manswer);
			gtk_widget_show(pr);
			gtk_widget_show(listbox2);
		}
	}
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
			//gint exit_status = 0;
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

	else if (event->keyval == GDK_KEY_Return && gtk_widget_has_focus(entry))
	{
		GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
		GtkTreeIter iter;
		GtkTreePath *path;
		gchar *app_name = NULL;
		const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));

		if (gtk_tree_model_get_iter_first(model, &iter))
		{
			gtk_tree_model_get(model, &iter, 0, &app_name, -1);
			gchar *entry_text_lower = g_ascii_strdown(entry_text, -1);
			gchar *app_name_lower = g_ascii_strdown(app_name, -1);

			if (gtk_tree_model_iter_has_child(model, &iter))
			{
				if (g_strstr_len(app_name_lower, -1, entry_text_lower) == NULL)
				{
					GtkTreeIter child_iter;
					if (gtk_tree_model_iter_children(model, &child_iter, &iter))
					{
						path = gtk_tree_model_get_path(model, &child_iter);
						gtk_tree_view_set_cursor(GTK_TREE_VIEW(treeview), path, NULL, FALSE);
						gtk_tree_view_row_activated(GTK_TREE_VIEW(treeview), path, NULL);
						gtk_tree_path_free(path);
						g_free(app_name);
						return 0;
					}
				}
				else
				{
					path = gtk_tree_model_get_path(model, &iter);
					gtk_tree_view_set_cursor(GTK_TREE_VIEW(treeview), path, NULL, FALSE);
					gtk_tree_view_row_activated(GTK_TREE_VIEW(treeview), path, NULL);
					gtk_tree_path_free(path);
				}
			}
			else
			{
				path = gtk_tree_model_get_path(model, &iter);
				gtk_tree_view_set_cursor(GTK_TREE_VIEW(treeview), path, NULL, FALSE);
				gtk_tree_view_row_activated(GTK_TREE_VIEW(treeview), path, NULL);
				gtk_tree_path_free(path);
			}
			g_free(app_name);
		}
	}

	else if(event->keyval == GDK_KEY_Down && gtk_widget_has_focus(entry))
	{
		gtk_widget_grab_focus(treeview);
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

gboolean on_focus_out(GtkWidget *widget, GdkEventFocus *event, gpointer user_data)
{
	GtkWidget *window = gtk_widget_get_toplevel(widget);

	GtkWidget *current_focus = gtk_window_get_focus(GTK_WINDOW(window));
	if (current_focus == NULL)
	{
		gtk_main_quit();
	}
	return FALSE;
}