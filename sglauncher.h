#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>
#define ML 256

const gchar *cweb, *cwebng, *corder, *active_text; 
const gchar* app_dirs[] = {"/usr/share/applications", "", NULL};
const gchar* quick_dirs[] = {NULL};
const gchar* pver = mver;
GPtrArray *program_icon_names;

gchar *pm, *webengine, *cwengine, cengine[ML], *home_dir, config_file_path[256], *program_icon;
gint wengine, order=0, showweb=1, showcmd=1, showcalc=1, showda=1,
	showscientific=1, exitwhenunfocused=0, sgcfg, nocsd=0, hidetitle=0, entryonbottom=0,
callconfig=0, nohome=0, restarting=0, ismoving=0, usecsd=1, fcsd=0;

GtkWidget *window, *grid, *button, *cmd_row, *dialog, *web_row, *entry, *manswer, *mathtext,
*listbox2,*pr, *row, *headerbar, *button, *image, *wtitle, *submenu, *submenu_item_settings,
*submenu_item_quickhelp, *submenu_item_about, *submenu_item4, *submenu_item5, *weblabel, *webcombo,
*webctm, *worder, *wshowcmd, *wshowweb, *wshowcalc, *wshowscientific, *wexitwhenunfocused, *defbtn,
*applybtn, *applist, *web_box, *wshowda, *wusecsd, *whidetitle ,*scrolled_window, *math,
*submenu_item_onlinehelp, *submenu_menu_help, *submenu_item_help, *wentryonbottom;

GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

GtkTreeStore *store;
GtkTreeViewColumn *column;
GtkCellRenderer *renderer;
GtkTreeModelSort *sorted_model;
GtkTreeModelFilter *filter_model;

#include "events.h"
#include "settings.h"
#include "calc.h"
#include "exec.h"
#include "entryloader.h"
#include "keyhandler.h"
#include "cfgmgr.h"
#include "mainwindow.h"
