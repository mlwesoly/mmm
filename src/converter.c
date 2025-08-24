#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <geanyplugin.h>
#include "mmm.h"

static GtkWidget *sidebar_vbox;
static gint page_number = 0;

void create_sidebar_addition(){
    
    GtkWidget 			*wid;
    
    //toolbar2 = gtk_toolbar_new();
	sidebar_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	
    /*
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(sidebar_vbox), GTK_ICON_SIZE_MENU);
	gtk_toolbar_set_style(GTK_TOOLBAR(sidebar_vbox), GTK_TOOLBAR_ICONS);

#if GTK_CHECK_VERSION(3, 10, 0)
	wid = gtk_image_new_from_icon_name("go-up", GTK_ICON_SIZE_SMALL_TOOLBAR);
	wid = GTK_WIDGET(gtk_tool_button_new(wid, NULL));
#else
	wid = GTK_WIDGET(gtk_tool_button_new_from_stock(GTK_STOCK_GO_UP));
#endif
	// gtk_widget_set_tooltip_text(wid, _("Go up"));
	//g_signal_connect(wid, "clicked", G_CALLBACK(on_button_go_up), NULL);
	
    TVCnumberEntry = gtk_entry_new();
	gtk_entry_set_placeholder_text( GTK_ENTRY(TVCnumberEntry),"TVC Number AA_BB_C.XYZ");

	g_signal_connect(TVCnumberEntry, "changed", G_CALLBACK(ui_textEntry_changed), NULL);
	g_signal_connect(TVCnumberEntry, "activate", G_CALLBACK(ui_textEntry_activate), NULL);

	gtk_box_pack_start(GTK_BOX(toolbar2), TVCnumberEntry, TRUE, TRUE, 0);
    //gtk_container_add(GTK_CONTAINER(toolbar2), TVCnumberEntry);
    */

    gtk_widget_show_all(sidebar_vbox);
    page_number = gtk_notebook_append_page(GTK_NOTEBOOK(geany->main_widgets->sidebar_notebook),
							sidebar_vbox, gtk_label_new(_("UC")));
}

void initthegui(){

    create_sidebar_addition();
    // gtk_notebook_set_current_page(GTK_NOTEBOOK(geany->main_widgets->sidebar_notebook), page_number);

}