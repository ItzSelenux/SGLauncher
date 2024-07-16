typedef struct
{
	gchar *name;
	gchar *icon;
	gchar *path;
	gchar *toexec;
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
					gchar *toexec = NULL;
					gchar *app_name = NULL;
					gboolean nodisplay = FALSE;
					GtkWidget *row = NULL;
					gboolean alreadyexec = FALSE;

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
						else if (strstr(line, "Exec=") == line)
						{
							if (!alreadyexec)
							{
								gchar *pos = strchr(line + 5, '\n');
								if (pos != NULL)
								{
									*pos = '\0';
								}
								toexec = g_strdup(line + 5);
								alreadyexec = TRUE;
							}
						}
						else if (strstr(line, "NoDisplay=") == line)
						{
							nodisplay = TRUE;
						}
						if (showda && strstr(line, "[Desktop Action") == line)
						{
							gchar *action_name_start = strchr(line, '[') + 1;
							gchar *action_name_end = strchr(line, ']');
							if (action_name_start != NULL && action_name_end != NULL)
							{
								*action_name_end = '\0';
								gchar *action_name = g_strdup(action_name_start);
								gchar *exec_value = NULL;
								gboolean done = FALSE;

								while (getline(&line, &len, file) != -1)
								{
									gchar *stripped_line = g_strstrip(line);

									if (stripped_line[0] == '[' && strstr(stripped_line, "[Desktop Action") == stripped_line)
									{
										gchar *search_str = g_strdup_printf("%s,%s,%s", exec_value ? exec_value : "", app_name ? app_name : "", action_name);

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

										action_name_start = strchr(stripped_line, '[') + 1;
										action_name_end = strchr(stripped_line, ']');
										*action_name_end = '\0';
										action_name = g_strdup(action_name_start);
										exec_value = NULL;
									}
									else if (strstr(stripped_line, "Name=") == stripped_line)
									{
										gchar *pos = strchr(stripped_line + 5, '\n');
										if (pos != NULL)
										{
											*pos = '\0';
										}
										if (strlen(stripped_line + 5) > 0)
										{
											action_name = g_strdup(stripped_line + 5);
										}
									}
									else if (strstr(stripped_line, "Exec=") == stripped_line)
									{
										gchar *pos = strchr(stripped_line + 5, '\n');
										if (pos != NULL)
										{
											*pos = '\0';
										}
										exec_value = g_strdup(stripped_line + 5);
									}
								}

								gchar *search_str = g_strdup_printf("%s,%s,%s", exec_value ? exec_value : "", app_name ? app_name : "", action_name);

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
						app_details[num_apps].toexec = toexec;
						num_apps++;
					}
					else
					{
						g_free(app_name);
						g_free(icon_name);
						g_free(path);
						g_free(toexec);
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
			gchar *search_str2 = g_strdup_printf("%s,%s%s", app_details[i].toexec ? app_details[i].toexec : "", app_details[i].name ? app_details[i].name : "", app_details[i].icon);
			gtk_widget_set_name(row, search_str2);
	}
}