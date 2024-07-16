#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#define ML 256

const gchar *cweb, *cwebng, *corder, *active_text; 
const char* app_dirs[] = {"/usr/share/applications", "", NULL};
const char* quick_dirs[] = {NULL};
const char* pver = mver;
char *pm, *webengine, *cwengine, cengine[ML], *home_dir, config_file_path[256];
int wengine, order, showweb, showcmd, showcalc, showda, showscientific,sgcfg, nocsd = 0;;

gboolean gshowcmd, gshowcalc, gshowweb, gshowda, gshowscientific;

GtkWidget *window, *grid, *cmd_row, *dialog, *web_row, *entry, *manswer, *mathtext, *listbox2,
*pr, *row, *headerbar, *button, *image, *wtitle, *submenu, *submenu_item1, *submenu_item2,
*submenu_item3, *submenu_item4, *submenu_item5, *weblabel, *webcombo, *webctm, *worder,
*wshowcmd, *wshowweb, *wshowcalc, *wshowscientific, *defbtn, *applybtn, *listbox, *web_box, *wshowda,
*scrolled_window;

GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

#include "settings.h"
#include "calc.h"
#include "exec.h"
#include "entryloader.h"
#include "keyhandler.h"
#include "cfgmgr.h"
#include "mainwindow.h"