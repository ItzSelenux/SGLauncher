//this is not a compilable code, this contains pseudocode for posterity

#define CONFIG_FILE "~/.config/menuconfig.conf"

// Function to write the configuration to a file
void write_config_file(const char *filename, const char *webengine, const char *webctm, gboolean order, gboolean showcmd, gboolean showweb) 
{
    char *home_dir = g_get_home_dir();
    char config_file_path[512];
    sprintf(config_file_path, "%s%s", home_dir, CONFIG_FILE);
    g_free(home_dir);

    // Open the file for writing
    FILE *fp = fopen(config_file_path, "w");
    if (!fp) 
    {
        g_warning("Failed to open configuration file for writing: %s", config_file_path);
        return;
    }

    // Write the configuration options to the file
    fprintf(fp, "[MenuSettings]\n");
    fprintf(fp, "WebEngine=%s\n", webengine);
    fprintf(fp, "CustomSearchEngine=%s\n", webctm);
    fprintf(fp, "ShowAppsFirst=%s\n", order ? "true" : "false");
    fprintf(fp, "ShowRunInTerminal=%s\n", showcmd ? "true" : "false");
    fprintf(fp, "ShowSearchOnWeb=%s\n", showweb ? "true" : "false");

    // Close the file
    fclose(fp);
}


// Function to read the configuration from a file
void read_config_file(const char *filename, char *webengine, char *webctm, gboolean *order, gboolean *showcmd, gboolean *showweb) {
    char *home_dir = g_get_home_dir();
    char config_file_path[512];
    sprintf(config_file_path, "%s%s", home_dir, CONFIG_FILE);
    g_free(home_dir);

    // Open the file for reading
    FILE *fp = fopen(config_file_path, "r");
    if (!fp) {
        g_warning("Failed to open configuration file for reading: %s", config_file_path);
        return;
    }

    // Read the configuration options from the file
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "WebEngine=%s", webengine) == 1) {
            continue;
        }
        if (sscanf(line, "CustomSearchEngine=%s", webctm) == 1) {
            continue;
        }
        if (sscanf(line, "ShowAppsFirst=%s", line) == 1) {
            *order = (strcmp(line, "true") == 0);
            continue;
        }
        if (sscanf(line, "ShowRunInTerminal=%s", line) == 1) {
            *showcmd = (strcmp(line, "true") == 0);
            continue;
        }
        if (sscanf(line, "ShowSearchOnWeb=%s", line) == 1) {
            *showweb = (strcmp(line, "true") == 0);
            continue;
        }
    }

    // Close the file
    fclose(fp);
}
