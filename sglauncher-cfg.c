#include "sglauncher.h"

int main(int argc, char *argv[])
{
if (argc > 0) 
{
	pm = argv[0];
}
else
{
	pm = "sglauncher";
}

home_dir = getenv("HOME");
readconf();

gtk_init(&argc, &argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "Settings - SGLauncher");
gtk_container_set_border_width(GTK_CONTAINER(window), 10);
gtk_widget_set_size_request(window, 400, 300);
g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

headerbar = gtk_header_bar_new();
gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerbar), TRUE);


button = gtk_menu_button_new();
image = gtk_image_new_from_icon_name("menulibre", GTK_ICON_SIZE_BUTTON);
gtk_container_add(GTK_CONTAINER(button), image);

gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), button);
wtitle = gtk_label_new(NULL);
gtk_label_set_markup(GTK_LABEL(wtitle), "<b>Settings - SGLauncher</b>");
gtk_header_bar_pack_start(GTK_HEADER_BAR(headerbar), wtitle);

submenu = gtk_menu_new();
submenu_item3 = gtk_menu_item_new_with_label("About");
gtk_menu_shell_append(GTK_MENU_SHELL(submenu), submenu_item3);
gtk_widget_show_all(submenu);
gtk_menu_button_set_popup(GTK_MENU_BUTTON(button), submenu);
gtk_window_set_titlebar(GTK_WINDOW(window), headerbar);

grid = gtk_grid_new();
gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
gtk_container_add(GTK_CONTAINER(window), grid);


webcombo = gtk_combo_box_text_new();
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "DuckDuckGo");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Google");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(webcombo), "Bing");


weblabel = gtk_label_new("Custom Search Engine:");
webctm = gtk_entry_new();
worder = gtk_combo_box_text_new(); 

	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at top");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Horizontal - Apps at bottom");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at left");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(worder), "Vertical - Apps at right");

wshowcmd = gtk_check_button_new(); 
wshowweb =  gtk_check_button_new();
wshowcalc =  gtk_check_button_new(); 

defbtn    = gtk_button_new_with_label("Default");
applybtn  = gtk_button_new_with_label("Apply");


//Items Grid position
gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Search Engine:"), 0, 0, 1, 1);
gtk_grid_attach(GTK_GRID(grid), webcombo, 1, 0, 1, 1);

gtk_grid_attach(GTK_GRID(grid), weblabel, 0, 2, 1, 1);
gtk_grid_attach(GTK_GRID(grid), webctm, 1, 2, 1, 1);

gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Item Order:"), 0, 3, 1, 1);    
gtk_grid_attach(GTK_GRID(grid), worder, 1, 3, 1, 1);

gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Run in Terminal\" entry "), 0, 4, 1, 1); 
gtk_grid_attach(GTK_GRID(grid), wshowcmd, 1, 4, 1, 1);

gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show \"Search on Web\" entry "), 0, 5, 1, 1); 
gtk_grid_attach(GTK_GRID(grid), wshowweb,   1, 5, 1, 1);

gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Math answer when numbers are typed "), 0, 6, 1, 1); 
gtk_grid_attach(GTK_GRID(grid), wshowcalc,   1, 6, 1, 1);

gtk_grid_attach(GTK_GRID(grid), defbtn,   0, 7, 1, 1);
gtk_grid_attach(GTK_GRID(grid), applybtn, 1, 7, 1, 1);

gtk_entry_set_placeholder_text(GTK_ENTRY(webctm), "https://you.com/search?q=");
const char *web_ctm = gtk_entry_get_text(GTK_ENTRY(webctm));



 
//SET Options STATUS
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

gtk_combo_box_set_active(GTK_COMBO_BOX(webcombo), wengine);
gtk_combo_box_set_active(GTK_COMBO_BOX(worder), order);



printf("\nWebEngine: %s\nCustomWebEngine: %s\nOrder: %s\nShowCMD: %d\nShowWeb: %d\nShowCalc: %d\n", cweb, cwebng, corder, gshowcmd, gshowweb, gshowcalc);

g_signal_connect(submenu_item3, "activate", G_CALLBACK(on_submenu_item3_selected), NULL);
g_signal_connect(applybtn, "clicked", G_CALLBACK(on_apply_button_clicked), NULL);
g_signal_connect(G_OBJECT(webcombo), "changed", G_CALLBACK(on_webcombo_changed), webctm);
g_signal_connect(defbtn, "clicked", G_CALLBACK(on_default_button_clicked), NULL);

gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
gtk_widget_show_all(window);
if (wengine != 3 )
{
	gtk_widget_hide(webctm);
	gtk_widget_hide(weblabel);
}

gtk_main();
return 0;
}
