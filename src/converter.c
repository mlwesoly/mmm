#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <geanyplugin.h>
#include "mmm.h"
#include <glib-2.0/glib.h>


static GtkWidget *sidebar_vbox, *treeview_vbox;
//static GtkWidget *length_vbox, *mass_vbox, *force_vbox, *torque_vbox, *power_vbox, *angularvel_vbox, *inertia_vbox;
static gint page_number = 0;


enum
{
  COL_FIRST_NAME,
  COL_YEAR_BORN,
  NUM_COLS
} ;

static GtkTreeModel *
scroll_fixture_mixed_tree_setup (void)
{
	GtkTreeStore *store;
	GtkTreeIter iter, child, iter1, child2;
	int i;

	store = gtk_tree_store_new (2, G_TYPE_STRING, G_TYPE_STRING);

	gtk_tree_store_append (store, &iter, NULL);
	gtk_tree_store_set (store, &iter, 0, "Root\nnode", -1);

	for (i = 0; i < 5; i++) {
		gtk_tree_store_append (store, &child, &iter);
		if (i % 2 != 0)
			gtk_tree_store_set (store, &child, 0, "Child node",1, "Child node",  -1);
		else
			gtk_tree_store_set (store, &child, 0, "Child\nnode",1, "Child node",  -1);
	}

  
	gtk_tree_store_append (store, &iter, NULL);
	gtk_tree_store_set (store, &iter, 0, "Root\nnode2", -1);

	for (i = 0; i < 5; i++) {
		gtk_tree_store_append (store, &child2, &iter);
		if (i % 2 == 0)
			gtk_tree_store_set (store, &child2, 0, "Other node",1, "Child node", -1);
		else
			gtk_tree_store_set (store, &child2, 0, "Other\nnode",1, "Child node", -1);
	}

	/* The teardown will also destroy the model */
	// scroll_fixture_setup (fixture, GTK_TREE_MODEL (store), test_data);

  return GTK_TREE_MODEL(store);
}


static GtkTreeModel *
create_and_fill_model (void)
{
  GtkTreeStore  *treestore;
  GtkTreeIter    toplevel, child;

  treestore = gtk_tree_store_new(NUM_COLS,
                                 G_TYPE_STRING,
                                 G_TYPE_UINT);

  /* Append a top level row and leave it empty */
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_FIRST_NAME, "length",
                     -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_FIRST_NAME, "in",
                     COL_YEAR_BORN, (guint) 1,
                     -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_FIRST_NAME, "ft",
                     COL_YEAR_BORN, (guint) 2,
                     -1);

  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_FIRST_NAME, "m",
                     COL_YEAR_BORN, "3",
                     -1);

  /*
  gtk_tree_store_append(treestore, &toplevel, NULL);
  gtk_tree_store_set(treestore, &toplevel,
                     COL_FIRST_NAME, "Inertias",
                     -1);
                     
  
  gtk_tree_store_append(treestore, &child, &toplevel);
  gtk_tree_store_set(treestore, &child,
                     COL_FIRST_NAME, "lb-in-s^2",
                     COL_YEAR_BORN, (guint) 1,
                     -1);
    */
  return GTK_TREE_MODEL(treestore);
}

void
age_cell_data_func (GtkTreeViewColumn *col,
                    GtkCellRenderer   *renderer,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    gpointer           user_data)
{
  guint  year_born;
  guint  year_now = 0; /* to save code not relevant for the example */
  gchar  buf[64];

  gtk_tree_model_get(model, iter, COL_YEAR_BORN, &year_born, -1);

  if (year_born <= year_now && year_born > 0)
  {
    guint age = year_now - year_born;

    g_snprintf(buf, sizeof(buf), "%u years old", age);

    g_object_set(renderer, "foreground-set", FALSE, NULL); /* print this normal */
  }
  else
  {
    g_snprintf(buf, sizeof(buf), "Category");

    /* make red */
    g_object_set(renderer, "foreground", "Red", "foreground-set", TRUE, NULL);
  }

  g_object_set(renderer, "text", buf, NULL);
}

static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *view;
  GtkTreeModel        *model;

  view = gtk_tree_view_new();

  /* --- Column #1 --- */

  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Name");

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", 0);



  /* --- Column #3 --- */

  col = gtk_tree_view_column_new();

  gtk_tree_view_column_set_title(col, "Age");

  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();

  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);

  /* connect a cell data function */
  // gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);
  gtk_tree_view_column_add_attribute(col, renderer, "text", 1);

  // model = create_and_fill_model();
  model = scroll_fixture_mixed_tree_setup();

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

  g_object_unref(model); /* destroy model automatically with view */

  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                              GTK_SELECTION_NONE);

  return view;
}

static void
setup_listitem_cb (GtkListItemFactory *factory,
                   GtkListItem        *list_item)
{
  GtkWidget *image;

  image = gtk_image_new ();
  gtk_image_set_icon_size (GTK_IMAGE (image), GTK_ICON_SIZE_LARGE);
  gtk_list_item_set_child (list_item, image);
}

static void
bind_listitem_cb (GtkListItemFactory *factory,
                  GtkListItem        *list_item)
{
  GtkWidget *image;
  GAppInfo *app_info;

  image = gtk_list_item_get_child (list_item);
  app_info = gtk_list_item_get_item (list_item);
  gtk_image_set_from_gicon (GTK_IMAGE (image), g_app_info_get_icon (app_info));
}

static void
activate_cb (GtkListView  *list,
             guint         position,
             gpointer      unused)
{
  GAppInfo *app_info;

  app_info = g_list_model_get_item (G_LIST_MODEL (gtk_list_view_get_model (list)), position);
  g_app_info_launch (app_info, NULL, NULL, NULL);
  g_object_unref (app_info);
}


static void listviewer()
{
  model = create_application_list ();

  factory = gtk_signal_list_item_factory_new ();
  g_signal_connect (factory, "setup", G_CALLBACK (setup_listitem_cb), NULL);
  g_signal_connect (factory, "bind", G_CALLBACK (bind_listitem_cb), NULL);

  list = gtk_list_view_new (GTK_SELECTION_MODEL (gtk_single_selection_new (model)), factory);

  g_signal_connect (list, "activate", G_CALLBACK (activate_cb), NULL);

  gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (sw), list);
}

/*
int
main (int argc, char **argv)
{
  gtk_init (&argc, &argv);

  GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy", gtk_main_quit, NULL);

  GtkWidget *view = create_view_and_model ();

  gtk_container_add (GTK_CONTAINER (window), view);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
*/

void fill_length_vbox(){

	GtkWidget *label, *generic_box;
	label = gtk_label_new ("legth conv");
	generic_box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);


	gtk_container_add(GTK_CONTAINER(sidebar_vbox), label);

	// gtk_container_add(GTK_CONTAINER(sidebar_vbox), generic_box);

	GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	GtkWidget *child_widget = gtk_button_new ();

	// GtkButton is not a GtkScrollable, so GtkScrolledWindow will automatically
	// add a GtkViewport.
	gtk_container_add (GTK_CONTAINER (scrolled_window),generic_box);

	//gtk_container_add (GTK_CONTAINER (scrolled_window),child_widget);
	
	gtk_container_add(GTK_CONTAINER(sidebar_vbox), scrolled_window);

}





void create_sidebar_addition(){
    
    //GtkWidget 			*wid;
    GtkWidget *label;
    //toolbar2 = gtk_toolbar_new();
	sidebar_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	treeview_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	
    // gtk_toolbar_set_icon_size(GTK_TOOLBAR(sidebar_vbox), GTK_ICON_SIZE_MENU);
	// gtk_toolbar_set_style(GTK_TOOLBAR(sidebar_vbox), GTK_TOOLBAR_ICONS);
    
	label = gtk_label_new ("Unit Converter");
	gtk_container_add(GTK_CONTAINER(sidebar_vbox), label);
	/*
	fill_length_vbox();
	mass_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	force_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	torque_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	power_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	angularvel_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	inertia_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	*/

	GtkWidget *view = create_view_and_model ();

	label = gtk_label_new ("Input");

  // void gtk_box_pack_start(GtkBox* box, GtkWidget* child, gboolean expand, gboolean fill, guint padding )

  /*
  gtk_container_add(GTK_CONTAINER(treeview_vbox), view);
	gtk_container_add(GTK_CONTAINER(sidebar_vbox), label);
	gtk_container_add(GTK_CONTAINER(sidebar_vbox), treeview_vbox);
  */
  gtk_box_pack_start(GTK_BOX(treeview_vbox), view,1,1,0);
  gtk_box_pack_start(GTK_BOX(sidebar_vbox), label,0,0,0);
  gtk_box_pack_start(GTK_BOX(sidebar_vbox), treeview_vbox,1,1,0);


	/*
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