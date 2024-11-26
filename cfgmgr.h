void on_webcombo_changed(GtkComboBox *widget, gpointer user_data)
{
	active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo));
	webctm = GTK_WIDGET(user_data);

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

void togglewidget(GtkWidget *input, gpointer data)
{
	GtkWidget *target= GTK_WIDGET(data);
	if (gtk_widget_get_visible(target)) 
	{
		gtk_widget_hide(target);
	}
	else
	{
		gtk_widget_show(target);
	}
}

void showcfg(void)
{
	callconfig = 1;
	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(dialog), "Settings - SGLauncher");
	gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
	gtk_widget_set_size_request(dialog, 400, 300);
	window_set_icon(GTK_WINDOW(dialog), program_icon);
	g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *confbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	GtkWidget *notebook = gtk_notebook_new();

		webcombo = gtk_combo_box_text_new();
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "DuckDuckGo");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Google");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Bing");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Custom");
		weblabel = gtk_label_new("Custom Search Engine:");
			webctm = gtk_entry_new();
		wshowcmd = gtk_check_button_new_with_label("Show \"Run in Terminal\" entry");
			gtk_widget_set_direction(wshowcmd, GTK_TEXT_DIR_RTL);
		wcloseterm = gtk_check_button_new_with_label("Close terminal when process finishes");
			gtk_widget_set_direction(wcloseterm, GTK_TEXT_DIR_RTL);
		wshowweb = gtk_check_button_new_with_label("Show \"Search on Web\" entry");
			gtk_widget_set_direction(wshowweb, GTK_TEXT_DIR_RTL);
		wshowcalc = gtk_check_button_new_with_label("Show Math answer when numbers are typed");
			gtk_widget_set_direction(wshowcalc, GTK_TEXT_DIR_RTL);
		wshowscientific = gtk_check_button_new_with_label("Use scientific notation on math answers");
			gtk_widget_set_direction(wshowscientific, GTK_TEXT_DIR_RTL);

		worder = gtk_combo_box_text_new();
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at top");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at bottom");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at left");
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at right");
		wshowda = gtk_check_button_new_with_label("Show Desktop actions");
			gtk_widget_set_direction(wshowda, GTK_TEXT_DIR_RTL);
		wentryonbottom = gtk_check_button_new_with_label("Search entry on bottom");
			gtk_widget_set_direction(wentryonbottom, GTK_TEXT_DIR_RTL);
		wuseiconview = gtk_check_button_new_with_label("Use icon view");
			gtk_widget_set_direction(wuseiconview, GTK_TEXT_DIR_RTL);
		GtkAdjustment *Icon_adjustment = gtk_adjustment_new(1, 0, 1024, 1, 1, 0);
			wiconsize = gtk_spin_button_new(Icon_adjustment, 1, 0);

		wusecsd = gtk_check_button_new_with_label("Use CSD Decoration");
			gtk_widget_set_direction(wusecsd, GTK_TEXT_DIR_RTL);
		whidetitle = gtk_check_button_new_with_label("Hide CSD Title");
			gtk_widget_set_direction(whidetitle, GTK_TEXT_DIR_RTL);
		wexitwhenunfocused = gtk_check_button_new_with_label("Exit when unfocused");
			gtk_widget_set_direction(wexitwhenunfocused, GTK_TEXT_DIR_RTL);

		defbtn = gtk_button_new_with_label("Default");
		applybtn = gtk_button_new_with_label("Apply");

	GtkWidget *tab1 = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab1), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab1, gtk_label_new("Elements"));
		gtk_grid_attach(GTK_GRID(tab1), gtk_label_new("Search Engine:"), 0, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(tab1), webcombo, 1, 0, 1, 1);

		gtk_grid_attach(GTK_GRID(tab1), weblabel, 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(tab1), webctm, 1, 1, 1, 1);

		gtk_grid_attach(GTK_GRID(tab1), wshowcmd, 0, 2, 2, 1);
		gtk_grid_attach(GTK_GRID(tab1), wcloseterm, 0, 3, 2, 1);
		gtk_grid_attach(GTK_GRID(tab1), wshowweb, 0, 4, 2, 1);
		gtk_grid_attach(GTK_GRID(tab1), wshowcalc, 0, 5, 2, 1);
		gtk_grid_attach(GTK_GRID(tab1), wshowscientific, 0, 6, 2, 1);

	GtkWidget *tab2 = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab2), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab2, gtk_label_new("View"));
		gtk_grid_attach(GTK_GRID(tab2), gtk_label_new("Item order:"), 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(tab2), worder, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(tab2), gtk_label_new("Icon Size"), 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(tab2), wiconsize, 1, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(tab2), wuseiconview, 0, 2, 2, 1);
		gtk_grid_attach(GTK_GRID(tab2), wshowda, 0, 3, 2, 1);
		gtk_grid_attach(GTK_GRID(tab2), wentryonbottom, 0, 4, 2, 1);

	GtkWidget *tab3 = gtk_grid_new();
	gtk_grid_set_column_homogeneous(GTK_GRID(tab3), TRUE);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab3, gtk_label_new("Window"));
		gtk_grid_attach(GTK_GRID(tab3), wusecsd, 0, 1, 2, 1);
		gtk_grid_attach(GTK_GRID(tab3), whidetitle, 0, 2, 2, 1);
		gtk_grid_attach(GTK_GRID(tab3), wexitwhenunfocused, 0, 3, 2, 1);

	gtk_box_pack_start(GTK_BOX(confbox), notebook, TRUE, TRUE, 0);

	GtkWidget *applybox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		gtk_box_pack_end(GTK_BOX(applybox), applybtn, FALSE, FALSE, 2);
		gtk_box_pack_end(GTK_BOX(applybox), defbtn, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(confbox), applybox, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(dialog), confbox);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowda), showda);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wuseiconview), useiconview);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wentryonbottom), entryonbottom);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcmd), showcmd);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wcloseterm), closeterm);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowweb), showweb);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowcalc), showcalc);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wshowscientific), showscientific);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wusecsd), usecsd);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(whidetitle), hidetitle);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused), exitwhenunfocused);

	gint length = strlen(cengine);
	if (length > 0 && cengine[length - 1] == '\n')
	{
		cengine[length - 1] = '\0';
	}

	gtk_entry_set_text(GTK_ENTRY(webctm), cengine);

	gtk_combo_box_set_active(GTK_COMBO_BOX(webcombo), wengine);
	gtk_combo_box_set_active(GTK_COMBO_BOX(worder), order);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(wiconsize), iconsize);

	g_signal_connect(G_OBJECT(webcombo), "changed", G_CALLBACK(on_webcombo_changed), webctm);

	g_signal_connect(wusecsd, "clicked", G_CALLBACK(togglewidget), whidetitle);
	g_signal_connect(wusecsd, "clicked", G_CALLBACK(togglewidget), wentryonbottom);
	g_signal_connect(wshowcalc, "clicked", G_CALLBACK(togglewidget), wshowscientific);

	g_signal_connect(defbtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(1));
	g_signal_connect(applybtn, "clicked", G_CALLBACK(updateconf), GINT_TO_POINTER(0));

	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_widget_show_all(dialog);

	if (wengine != 3)
	{
		gtk_widget_hide(webctm);
		gtk_widget_hide(weblabel);
	}

	if (!showcalc)
	{
		gtk_widget_hide(wshowscientific);
	}
	if (!usecsd)
	{
		gtk_widget_hide(whidetitle);
	}
	else if (usecsd)
	{
		gtk_widget_hide(wentryonbottom);
	}

	g_signal_connect(dialog, "destroy", G_CALLBACK(on_dialog_destroy), NULL);
	gtk_main();
}
