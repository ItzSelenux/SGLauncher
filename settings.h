void exit_window(void);

void updateconf(GtkButton *widget, gpointer user_data) 
{
	gint reset = GPOINTER_TO_INT(user_data);

	const gchar *activetext = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo)),
				*active_order = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(worder)),
				*ncengine,
				*entry_text = gtk_entry_get_text(GTK_ENTRY(webctm)),
				*placeholder_text = gtk_entry_get_placeholder_text(GTK_ENTRY(webctm));

	FILE *fp = fopen(config_file_path, "w+");
	if (fp == NULL) 
	{
		perror("Failed to open config file");
		exit(EXIT_FAILURE);
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
		} 
		else 
		{
			printf("Operation cancelled.\n");
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
		entryonbottom = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wentryonbottom));
		exitwhenunfocused = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused));
		usecsd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wusecsd));
		hidetitle = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(whidetitle));

		if (entry_text && strlen(entry_text) > 0) 
			ncengine = entry_text;
		else 
			ncengine = placeholder_text;

		g_print("%s", ncengine);
		cweb = NULL;
		cwebng = gtk_entry_get_text(GTK_ENTRY(webctm));
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
		snprintf(config_file_path, sizeof(config_file_path), "%s/.config/sglauncher.conf", home_dir);
		FILE *file = fopen(config_file_path, "r");

		if (file == NULL)
		{
			return;
		}

		char line[ML];
		// Read each line from the file and parse the variable assignments
		while (fgets(line, ML, file) != NULL)
		{
			char *name = strtok(line, "=");
			char *value_str = strtok(NULL, "=");

			if (name != NULL && value_str != NULL)
			{
				if (strcmp(name, "order") == 0) 
					order = atoi(value_str);
				else if (strcmp(name, "cengine") == 0) 
					strncpy(cengine, value_str, sizeof(cengine));
				else if (strcmp(name, "wengine") == 0)
					wengine = atoi(value_str);
				else if (strcmp(name, "showweb") == 0) 
					showweb = atoi(value_str);
				else if (strcmp(name, "showcmd") == 0) 
					showcmd = atoi(value_str);
				else if (strcmp(name, "showcalc") == 0) 
					showcalc = atoi(value_str);
				else if (strcmp(name, "showscientific") == 0) 
					showscientific = atoi(value_str);
				else if (strcmp(name, "showda") == 0) 
					showda = atoi(value_str);
				else if (strcmp(name, "entryonbottom") == 0) 
					entryonbottom = atoi(value_str);
				else if (strcmp(name, "usecsd") == 0)
				{
					usecsd = atoi(value_str);
					if (!fcsd)
					{
						nocsd = (usecsd == 0);
					}
				}
				else if (strcmp(name, "hidetitle") == 0) 
					hidetitle = atoi(value_str);
				else if (strcmp(name, "exitwhenunfocused") == 0) 
					exitwhenunfocused = atoi(value_str);
			}
		}
		fclose(file);
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
	printf("WebEngine: %s\nOrder: %d\nShowDA: %d\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\nShowScientific: %d\nExitWhenUnfocused: %d\nUseCSD: %d\nHideTitle: %d\n",
		webengine, order, showda, showcmd, showweb, showcalc, showscientific, exitwhenunfocused,
		usecsd, hidetitle);
}
