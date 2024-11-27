void exit_window(void);

void updateconf(GtkButton *widget, gpointer user_data) 
{
	gint reset = GPOINTER_TO_INT(user_data);
	GKeyFile *config = g_key_file_new();
	GError *error = NULL;

	const gchar *activetext = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo)),
		*ncengine = NULL,
		*entry_text = gtk_entry_get_text(GTK_ENTRY(webctm)),
		*placeholder_text = gtk_entry_get_placeholder_text(GTK_ENTRY(webctm));

	if (!g_key_file_load_from_file(config, config_file_path, G_KEY_FILE_KEEP_COMMENTS, &error))
	{
		g_warning("Error loading config file: %s", error->message);
		g_clear_error(&error);
	}

	if (reset) 
	{
		dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION,
			GTK_BUTTONS_OK_CANCEL,
			"Are you sure you want to restore SGLauncher settings as default?");
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
		gtk_window_set_title(GTK_WINDOW(dialog), "Confirmation");
		gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK);
		gint result = gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);

		if (result == GTK_RESPONSE_CANCEL) 
		{
			g_info("Operation cancelled.\n");
			return;
		}

		g_key_file_set_string(config, "Elements", "cengine", "https://search.disroot.org/search?q");
		g_key_file_set_string(config, "Elements", "wengine", 0);
		g_key_file_set_integer(config, "Elements", "showcmd", 1);
		g_key_file_set_integer(config, "Elements", "showweb", 0);
		g_key_file_set_integer(config, "Elements", "showcalc", 1);
		g_key_file_set_integer(config, "Elements", "showofd", 1);
		g_key_file_set_integer(config, "View", "iconsize", 16);
		g_key_file_set_integer(config, "View", "useiconview", 0);
		g_key_file_set_integer(config, "View", "showda", 1);
		g_key_file_set_string(config, "View", "order", 0);
		g_key_file_set_integer(config, "Behavior", "closeterm", 0);
		g_key_file_set_integer(config, "Behavior", "showscientific", 1);
		g_key_file_set_integer(config, "Behavior", "ignorenodisplay", 1);
		g_key_file_set_integer(config, "Behavior", "ignoreterminal", 0);
		g_key_file_set_integer(config, "Behavior", "ignoreonlyshowin", 0);
		g_key_file_set_integer(config, "Window", "usecsd", 1);
		g_key_file_set_integer(config, "Window", "hidetitle", 0);
		g_key_file_set_integer(config, "Window", "hidewindeco", 0);
		g_key_file_set_integer(config, "Window", "exitwhenunfocused", 0);
	} 
	else 
	{
		showcmd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcmd));
		showweb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowweb));
		showofd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowofd));
		showcalc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcalc));
		showda = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowda));
		useiconview = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wuseiconview));
		entryonbottom = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wentryonbottom));
		closeterm = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wcloseterm));
		showscientific = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowscientific));
		ignorenodisplay = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wignorenodisplay));
		ignoreonlyshowin = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wignoreonlyshowin));
		ignoreterminal = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wignoreterminal));
		exitwhenunfocused = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused));
		usecsd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wusecsd));
		hidetitle = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(whidetitle));
		hidewindeco = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(whidewindeco));
		order = gtk_combo_box_get_active(GTK_COMBO_BOX(worder));
		wengine = gtk_combo_box_get_active(GTK_COMBO_BOX(webcombo));
		iconsize = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(wiconsize));
		ncengine = (entry_text && strlen(entry_text) > 0) ? entry_text : placeholder_text;

		g_key_file_set_string(config, "Elements", "cengine", ncengine);
		g_key_file_set_integer(config, "Elements", "wengine", wengine);
		g_key_file_set_integer(config, "Elements", "showcmd", showcmd);
		g_key_file_set_integer(config, "Elements", "showweb", showweb);
		g_key_file_set_integer(config, "Elements", "showofd", showofd);
		g_key_file_set_integer(config, "Elements", "showcalc", showcalc);
		g_key_file_set_integer(config, "View", "iconsize", iconsize);
		g_key_file_set_integer(config, "View", "useiconview", useiconview);
		g_key_file_set_integer(config, "View", "showda", showda);
		g_key_file_set_integer(config, "View", "order", order);
		g_key_file_set_integer(config, "Behavior", "closeterm", closeterm);
		g_key_file_set_integer(config, "Behavior", "showscientific", showscientific);
		g_key_file_set_integer(config, "Behavior", "ignorenodisplay", ignorenodisplay);
		g_key_file_set_integer(config, "Behavior", "ignoreterminal", ignoreterminal);
		g_key_file_set_integer(config, "Behavior", "ignoreonlyshowin", ignoreonlyshowin);
		g_key_file_set_integer(config, "Window", "usecsd", usecsd);
		g_key_file_set_integer(config, "Window", "hidetitle", hidetitle);
		g_key_file_set_integer(config, "Window", "hidewindeco", hidewindeco);
		g_key_file_set_integer(config, "Window", "exitwhenunfocused", exitwhenunfocused);
	}

	if (!g_key_file_save_to_file(config, config_file_path, &error)) 
	{
		g_warning("Error saving config file: %s", error->message);
		g_clear_error(&error);
	}

	g_key_file_unref(config);

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

		if (g_key_file_has_key(key_file, "Elements", "showofd", NULL))
			showofd = g_key_file_get_integer(key_file, "Elements", "showofd", NULL);

		if (g_key_file_has_key(key_file, "Elements", "showcalc", NULL))
			showcalc = g_key_file_get_integer(key_file, "Elements", "showcalc", NULL);


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

		if (g_key_file_has_key(key_file, "Behavior", "closeterm", NULL))
			closeterm = g_key_file_get_integer(key_file, "Behavior", "closeterm", NULL);
			ccloseterm = closeterm ? "": ";read" ;

		if (g_key_file_has_key(key_file, "Behavior", "showscientific", NULL))
			showscientific = g_key_file_get_integer(key_file, "Behavior", "showscientific", NULL);

		if (g_key_file_has_key(key_file, "Behavior", "ignorenodisplay", NULL))
			ignorenodisplay = g_key_file_get_integer(key_file, "Behavior", "ignorenodisplay", NULL);

		if (g_key_file_has_key(key_file, "Behavior", "ignoreterminal", NULL))
			ignoreterminal = g_key_file_get_integer(key_file, "Behavior", "ignoreterminal", NULL);

		if (g_key_file_has_key(key_file, "Behavior", "ignoreonlyshowin", NULL))
			ignoreonlyshowin = g_key_file_get_integer(key_file, "Behavior", "ignoreonlyshowin", NULL);


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
		if (g_key_file_has_key(key_file, "Window", "hidewindeco", NULL))
			hidewindeco = g_key_file_get_integer(key_file, "Window", "hidewindeco", NULL);

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
