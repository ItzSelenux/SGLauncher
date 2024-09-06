void on_webcombo_changed(GtkComboBox *webcombo, gpointer user_data)
{
	active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo));
	GtkWidget *webctm = GTK_WIDGET(user_data);
	//GtkWidget *webctmlb = GTK_WIDGET(g_object_get_data(G_OBJECT(webctm), "webctmlb"));

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

void on_dialog_destroy(GtkWidget *widget, gpointer data)
{
	callconfig = 0;
}

void showcfg()
{
	callconfig = 1;
	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(dialog), "Settings - SGLauncher");
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
	gtk_widget_set_size_request(dialog, 400, 300);
	g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	theme = gtk_icon_theme_get_default();
	info = gtk_icon_theme_lookup_icon(theme, "menulibre", 48, 0);
	if (info != NULL)
	{
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(GTK_WINDOW(dialog), icon);
		g_object_unref(icon);
		g_object_unref(info);
	}

	grid = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_container_add(GTK_CONTAINER(dialog), grid);

	webcombo = gtk_combo_box_text_new();
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "DuckDuckGo");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Google");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Bing");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Custom");

	weblabel = gtk_label_new("Custom Search Engine:");
	webctm = gtk_entry_new();
	worder = gtk_combo_box_text_new(); 

		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at top");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at bottom");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at left");
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at right");

	wshowda = gtk_check_button_new();
	wshowcmd = gtk_check_button_new();
	wshowweb =  gtk_check_button_new();
	wshowcalc =  gtk_check_button_new();
	wshowscientific =  gtk_check_button_new();
	wexitwhenunfocused =  gtk_check_button_new();

	defbtn = gtk_button_new_with_label("Default");
	applybtn  = gtk_button_new_with_label("Apply");

	//Items Grid position
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Search Engine:"), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), webcombo, 1, 0, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), weblabel, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), webctm, 1, 2, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Item Order:"), 0, 3, 1, 1);    
	gtk_grid_attach(GTK_GRID(grid), worder, 1, 3, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Desktop actions "), 0, 4, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), wshowda, 1, 4, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Run in Terminal\" entry "), 0, 5, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), wshowcmd, 1, 5, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Search on Web\" entry "), 0, 6, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), wshowweb,   1, 6, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Math answer when numbers are typed "), 0, 7, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), wshowcalc,   1, 7, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Use scientific notation on math answers"), 0, 8, 1, 1); 
	gtk_grid_attach(GTK_GRID(grid), wshowscientific,   1, 8, 1, 1);

	//gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Exit when unfocused"), 0, 9, 1, 1); 
	//gtk_grid_attach(GTK_GRID(grid), wexitwhenunfocused,   1, 9, 1, 1);

	gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 10, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 10, 1, 1);

	gtk_entry_set_placeholder_text(GTK_ENTRY(webctm), "https://search.disroot.org/search?q");


	//SET Options STATUS
	if (showda == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowda), TRUE);
	}
	if (showcmd == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcmd), TRUE);
	}
	if (showweb == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowweb), TRUE);
	}
	if (showcalc == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcalc), TRUE);
	}
	if (showscientific == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowscientific), TRUE);
	}
	if (exitwhenunfocused == 1)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused), TRUE);
	}

	//fix newline at end of cengine
	int length = strlen(cengine);
	if (length > 0 && cengine[length - 1] == '\n')
	{
		cengine[length - 1] = '\0';
	}

	gtk_entry_set_text(GTK_ENTRY(webctm), cengine);

	gtk_combo_box_set_active(GTK_COMBO_BOX(webcombo), wengine);
	gtk_combo_box_set_active(GTK_COMBO_BOX(worder), order);

	g_signal_connect(G_OBJECT(webcombo), "changed", G_CALLBACK(on_webcombo_changed), webctm);
	g_signal_connect(applybtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(0));
	g_signal_connect(defbtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(1));

	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(dialog);
	if (wengine != 3 )
	{
		gtk_widget_hide(webctm);
		gtk_widget_hide(weblabel);
	}

	g_signal_connect(dialog, "destroy", G_CALLBACK(on_dialog_destroy), NULL);

	gtk_main();
}