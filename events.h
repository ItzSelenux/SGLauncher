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