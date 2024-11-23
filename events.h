gchar* probe_icons_from_theme(GPtrArray *icon_names)
{
	theme = gtk_icon_theme_get_default();
	gint i;

	for (i = 0; i < icon_names->len; i++)
	{
		gchar *icon_name = g_ptr_array_index(icon_names, i);
		info = gtk_icon_theme_lookup_icon(theme, icon_name, 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			if (icon != NULL)
			{
				g_object_unref(icon);
				g_object_unref(info);
				return g_strdup(icon_name);
			}
			g_object_unref(info);
		}
	}
	return g_strdup("\0");
}

void window_set_icon(GtkWindow *target, gchar *iconname)
{
	info = gtk_icon_theme_lookup_icon(theme, iconname, 16, 16);
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(target, icon);
		g_object_unref(icon);
		g_object_unref(info);
}

//void adjust_iconview(GtkWidget *target, gpointer *input)
//{
	//gint new_width, new_height, new_columns;
	//gtk_window_get_size(GTK_WINDOW(window), &new_width, &new_height);

	//new_columns = new_width / (iconsize * 4.375); 

	//gtk_icon_view_set_columns(GTK_ICON_VIEW(iconview), new_columns);
	//gtk_icon_view_set_item_padding(GTK_ICON_VIEW(iconview), new_columns);
	//g_print("%d, %d\n", new_width, iconsize / (new_columns*new_columns));
//}
