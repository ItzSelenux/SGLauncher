void exit_window();

void updateconf(GtkButton *button, gpointer user_data) 
{
	int reset = GPOINTER_TO_INT(user_data);

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
			fprintf(fp, "[SGLauncher Configuration File]\n");
			fprintf(fp, "cengine=https://search.disroot.org/search?q\n");
			fprintf(fp, "wengine=1\n");
			fprintf(fp, "order=0\n");
			fprintf(fp, "showda=1\n");
			fprintf(fp, "showweb=1\n");
			fprintf(fp, "showcmd=1\n");
			fprintf(fp, "showcalc=1\n");
			fprintf(fp, "showscientific=1\n");
			//fprintf(fp, "exitwhenunfocused=1\n");
		}
		else
		{
			printf("Operation cancelled.\n");
		}
		gtk_widget_destroy(dialog);
	}
	else
	{
		gshowcmd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcmd));
		gshowweb = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowweb));
		gshowcalc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowcalc));
		gshowscientific = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowscientific));
		gshowda = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wshowda));
		//gexitwhenunfocused = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wexitwhenunfocused));
		const gchar *active_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(webcombo)),
			*active_order = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(worder)),
			*ncengine,
			*entry_text = gtk_entry_get_text(GTK_ENTRY(webctm)),
			*placeholder_text = gtk_entry_get_placeholder_text(GTK_ENTRY(webctm));

		if (entry_text && strlen(entry_text) > 0)
			ncengine = entry_text;
		else
			ncengine = placeholder_text;

		g_print("%s", ncengine);
		cweb = NULL;
		cwebng = gtk_entry_get_text(GTK_ENTRY(webctm));
		corder = NULL;
		if (active_text != NULL) 
		{
			if (g_strcmp0(active_text, "Google") == 0) 
				cweb = "1";
			else if (g_strcmp0(active_text, "DuckDuckGo") == 0) 
				cweb = "0";
			else if (g_strcmp0(active_text, "Bing") == 0) 
				cweb = "2";
			else if (g_strcmp0(active_text, "Custom") == 0) 
				cweb = "3";

			if (active_order != NULL) 
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
		fprintf(fp, "[SGLauncher Configuration File]\n");
		fprintf(fp, "cengine=%s\n", ncengine);
		fprintf(fp, "wengine=%s\n", cweb);
		fprintf(fp, "order=%s\n", corder);
		fprintf(fp, "showda=%d\n", gshowda);
		fprintf(fp, "showweb=%d\n", gshowweb);
		fprintf(fp, "showcmd=%d\n", gshowcmd);
		fprintf(fp, "showcalc=%d\n", gshowcalc);
		fprintf(fp, "showscientific=%d\n", gshowscientific);
		//fprintf(fp, "exitwhenunfocused=%d\n", gexitwhenunfocused);
	}
	fclose(fp);
	restarting = 1;
	exit_window();
}

void readconf()
{
	//READ THE CONF
	if (home_dir == NULL)
	{
		return;
	}
	else
	{
		snprintf(config_file_path, sizeof(config_file_path), "%s/.config/sglauncher.conf", home_dir);
		FILE *file = fopen(config_file_path, "r");
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

	// Use the values that were read from the file
	printf("WebEngine: %s\nOrder: %d\nShowDA: %d\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\nShowScientific: %d\n", webengine, order, showda, showcmd, showweb, showcalc, showscientific);
}