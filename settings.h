void exit_window(void);

void updateconf(GtkButton *widget, gpointer user_data) 
{
	gint reset = GPOINTER_TO_INT(user_data);

	const gchar *activetext = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo)),
				*active_order = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(worder)),
				*ncengine,
				*entry_text = gtk_entry_get_text(GTK_ENTRY(webctm)),
				*placeholder_text = gtk_entry_get_placeholder_text(GTK_ENTRY(webctm));

	FILE *fp = fopen(config_file_path, "a+");
	if (fp == NULL) 
	{
		g_warning("Failed to open config file");
		return;
	}

	if (reset) 
	{
		dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL, "Are you sure you want to restore SGLauncher settings as default?");
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
		gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
		gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
		gint result = gtk_dialog_run(GTK_DIALOG(dialog));
		if (result == GTK_RESPONSE_OK)
		{
			ncengine = "https://search.disroot.org/search?q";
			wengine = 1;
			order = 0;
			showda = 1;
			showweb = 1;
			showcmd = 1;
			showcalc = 1;
			showscientific = 1;
			exitwhenunfocused = 0;
			iconsize = 16;
		} 
		else if  (result == GTK_RESPONSE_CANCEL) 
		{
			g_info("Operation cancelled.\n");
			gtk_widget_destroy(dialog);
			return;
		}
		gtk_widget_destroy(dialog);
	} 
	else 
	{
		showcmd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcmd));
		showweb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowweb));
		showcalc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcalc));
		showscientific = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowscientific));
		showda = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowda));
		useiconview = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wuseiconview));
		entryonbottom = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wentryonbottom));
		exitwhenunfocused = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused));
		usecsd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wusecsd));
		hidetitle = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(whidetitle));

		if (entry_text && strlen(entry_text) > 0) 
			ncengine = entry_text;
		else 
			ncengine = placeholder_text;

		cweb = NULL;
		cwebng = gtk_entry_get_text(GTK_ENTRY(webctm));
		iconsize = gtk_spin_button_get_value(GTK_SPIN_BUTTON(wiconsize));
		corder = NULL;

		if (activetext) 
		{
			if (g_strcmp0(activetext, "Google") == 0) 
				cweb = "1";
			else if (g_strcmp0(activetext, "DuckDuckGo") == 0) 
				cweb = "0";
			else if (g_strcmp0(activetext, "Bing") == 0) 
				cweb = "2";
			else if (g_strcmp0(activetext, "Custom") == 0) 
				cweb = "3";

			if (active_order) 
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
	}

	fclose(fp);
	fp = fopen(config_file_path, "w");

	fprintf(fp, "[SGLauncher Configuration File]\n");
	fprintf(fp, "[Elements]\n");
	fprintf(fp, "cengine=%s\n", ncengine);
	fprintf(fp, "wengine=%s\n", cweb);
	fprintf(fp, "showcmd=%d\n", showcmd);
	fprintf(fp, "showweb=%d\n", showweb);
	fprintf(fp, "showcalc=%d\n", showcalc);
	fprintf(fp, "showscientific=%d\n", showscientific);
	fprintf(fp, "[View]\n");
	fprintf(fp, "order=%s\n", corder);
	fprintf(fp, "iconsize=%d\n", iconsize);
	fprintf(fp, "useiconview=%d\n", useiconview);
	fprintf(fp, "showda=%d\n", showda);
	fprintf(fp, "entryonbottom=%d\n", entryonbottom);
	fprintf(fp, "[Window]\n");
	fprintf(fp, "usecsd=%d\n", usecsd);
	fprintf(fp, "hidetitle=%d\n", hidetitle);
	fprintf(fp, "exitwhenunfocused=%d\n", exitwhenunfocused);

	fclose(fp);
	restarting = 1;
	exit_window();
}


void readconf(void)
{
	//READ THE CONF
	if (home_dir == NULL)
	{
		nohome = 1;
		return;
	}
	else
	{
		config_file_path = g_build_filename(g_get_user_config_dir(), "sglauncher.conf", NULL);

		GKeyFile *key_file;
		GError *error = NULL;

		key_file = g_key_file_new();

		if (!g_key_file_load_from_file(key_file, config_file_path, G_KEY_FILE_NONE, &error))
		{
			g_warning("Failed to load configuration file: %s", error->message);
			g_error_free(error);
			g_key_file_free(key_file);
			return;
		}

		if (g_key_file_has_key(key_file, "Elements", "cengine", NULL))
			g_strlcpy(cengine, g_key_file_get_string(key_file, "Elements", "cengine", NULL), sizeof(cengine));

		if (g_key_file_has_key(key_file, "Elements", "wengine", NULL))
			wengine = g_key_file_get_integer(key_file, "Elements", "wengine", NULL);

		if (g_key_file_has_key(key_file, "Elements", "showcmd", NULL))
			showcmd = g_key_file_get_integer(key_file, "Elements", "showcmd", NULL);

		if (g_key_file_has_key(key_file, "Elements", "showweb", NULL))
			showweb = g_key_file_get_integer(key_file, "Elements", "showweb", NULL);

		if (g_key_file_has_key(key_file, "Elements", "showcalc", NULL))
			showcalc = g_key_file_get_integer(key_file, "Elements", "showcalc", NULL);

		if (g_key_file_has_key(key_file, "Elements", "showscientific", NULL))
			showscientific = g_key_file_get_integer(key_file, "Elements", "showscientific", NULL);


		if (g_key_file_has_key(key_file, "View", "order", NULL))
			order = g_key_file_get_integer(key_file, "View", "order", NULL);
		if (g_key_file_has_key(key_file, "View", "useiconview", NULL))
			useiconview = g_key_file_get_integer(key_file, "View", "useiconview", NULL);
		if (g_key_file_has_key(key_file, "View", "iconsize", NULL))
			iconsize = g_key_file_get_integer(key_file, "View", "iconsize", NULL);
		if (g_key_file_has_key(key_file, "View", "showda", NULL))
			showda = g_key_file_get_integer(key_file, "View", "showda", NULL);
		if (g_key_file_has_key(key_file, "View", "entryonbottom", NULL))
			entryonbottom = g_key_file_get_integer(key_file, "View", "entryonbottom", NULL);

		if (g_key_file_has_key(key_file, "Window", "usecsd", NULL))
		{
			usecsd = g_key_file_get_integer(key_file, "Window", "usecsd", NULL);
			if (!fcsd)
			{
				nocsd = (usecsd == 0);
			}
		}

		if (g_key_file_has_key(key_file, "Window", "hidetitle", NULL))
			hidetitle = g_key_file_get_integer(key_file, "Window", "hidetitle", NULL);

		if (g_key_file_has_key(key_file, "Window", "exitwhenunfocused", NULL))
			exitwhenunfocused = g_key_file_get_integer(key_file, "Window", "exitwhenunfocused", NULL);

		g_key_file_free(key_file);
	}

	switch (wengine)
	{
		case 0:
			webengine = "https://duckduckgo.com/?q";
			break;
		case 1:
			webengine = "https://www.google.com/search?q";
			break;
		case 2:
			webengine = "https://www.bing.com/search?q";
			break;
		case 3:
			webengine = cengine;
			break;
		default:
			webengine = "https://duckduckgo.com/?q";
			break;
	}
	g_info("WebEngine: %s\nOrder: %d\nShowDA: %d\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\nShowScientific: %d\nExitWhenUnfocused: %d\nUseCSD: %d\nHideTitle: %d\nIcon Size: %d\n",
		webengine, order, showda, showcmd, showweb, showcalc, showscientific, exitwhenunfocused,
		usecsd, hidetitle, iconsize);
}
