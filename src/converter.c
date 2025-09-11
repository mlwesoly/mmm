#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <geanyplugin.h>
#include "mmm.h"
#include <glib-2.0/glib.h>
#include <gtk-3.0/gtk/gtk.h>

static GtkWidget *sidebar_vbox, *treeview_vbox;
//static GtkWidget *length_vbox, *mass_vbox, *force_vbox, *torque_vbox, *power_vbox, *angularvel_vbox, *inertia_vbox;
static gint page_number = 0;

static gint groupselection = 0;

static GtkWidget *viewinput, *viewoutput;
//static GtkTreeModel *modelinput, *modeloutput;
static GtkTreeIter inputiter, outputiter;
static GtkTreeIter child_iter;
static GtkTreeStore *modelinput;
static GtkTreeIter iterh ;
static GtkTreeStore *modeloutput;

static GtkWidget *create_view_and_model_out (void);
  
// TODO : checking GTK Demo for more info on this topic

typedef struct _TreeItem TreeItem;
struct _TreeItem
{
  const gchar     *label;
  const gfloat     factor01;
  const gfloat     factor02;
  const gfloat     factor03;
  const gfloat     factor04;
  const gint       group;
  TreeItem        *children;
};

typedef struct _TreeItemOUT TreeItemOUT;
struct _TreeItemOUT
{
  const gchar     *label;
  const gint    factor01;
  TreeItemOUT  *children;
};

static TreeItem Power[] =
{
  {"kW", 1.34, 0.02, 0.0, 0.0, 0, NULL },
  {"hp", 0.76, 0.03, 0.0, 0.0, 0, NULL },
  { NULL }
};

static TreeItem Torque[] =
{
  {"Nm"   , 1.1, 1.2, 1.0, 1.0, 1, NULL },
  {"lb*in", 1.5, 1.1, 1.0, 1.0, 1, NULL },
  { NULL }
};

static TreeItem Inertia[] =
{
  {"Nm"   , 1.1, 1.2, 1.0, 1.0, 1, NULL },
  {"lb*in", 1.5, 1.1, 1.0, 1.0, 1, NULL },
  { NULL }
};


static TreeItem toplevelin[] =
{
  {"Power", 0.0 , 0.0, 0.0, 0.0, 1, Power},
  {"Torque",0.0 , 0.0, 0.0, 0.0, 1, Torque},
  {"Inertia",0.0 , 0.0, 0.0, 0.0, 1, Inertia},
  {NULL}
};


static TreeItemOUT Powerout[] =
{
  {"Power" , 0 , NULL},
  {"Torque", 1 , NULL},
  {"Torq02", 1 , NULL},
  {"Torq04", 1 , NULL},
  {NULL}
};

static TreeItemOUT torqueout[] =
{
  {"gooo" , 0 , NULL },
  {"some" , 1 , NULL },
  {"where", 2 , NULL},
  {NULL}
};
static TreeItemOUT inertiaout[] =
{
  {"tuurn" , 0 , NULL },
  {"more " , 1 , NULL },
  {"around", 2 , NULL},
  {NULL}
};

static TreeItemOUT toplevelout[] =
{
  {"Power", 0, Powerout},
  {"Torque", 0, torqueout},
  {"Inertia", 0, inertiaout},
  {NULL}
};


static GtkTreeModel *
input_tree_setup (void)
{
	//GtkTreeIter iter;
  TreeItem *month = toplevelin;
	int i;

	modelinput = gtk_tree_store_new (6, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT , G_TYPE_FLOAT, G_TYPE_FLOAT, G_TYPE_INT);
  while (month->label)
  {
    TreeItem *holiday = month->children;
    
    gtk_tree_store_append (modelinput, &inputiter, NULL);
    gtk_tree_store_set (modelinput, &inputiter, 0, month->label, 5, month->group, -1);    
    // add children 
    while (holiday->label)
    {
      // GtkTreeIter child_iter;
      gtk_tree_store_append (modelinput, &child_iter, &inputiter);
      gtk_tree_store_set (modelinput, &child_iter, 0, holiday->label, 1, holiday->factor01, 2, holiday->factor02, 3, holiday->factor03, 4, holiday->factor04,  5, holiday->group , -1); //1, "Child node",
      holiday++;
    }
    month++;
  }
  return GTK_TREE_MODEL (modelinput);
}

static GtkTreeModel *
output_tree_setup (int outputindex)
{
  TreeItemOUT *month = toplevelout;
	int i;
  month = month + outputindex;

	modeloutput = gtk_tree_store_new (3, G_TYPE_STRING, G_TYPE_FLOAT, G_TYPE_FLOAT);

  //while (month->label)
  //{
    TreeItem *holiday = month->children;
    // gtk_tree_store_append (modeloutput, &iter, NULL);
    //gtk_tree_store_set (modeloutput, &iter, 0, month->label, -1);    
    //add children 
    
    while (holiday->label)
      {
        GtkTreeIter child_iter;
        //gtk_tree_store_append (model, &child_iter, &iter);
        //gtk_tree_store_set (model, &child_iter, 0, holiday->label,  -1); //1, "Child node",
        gtk_tree_store_append (modeloutput, &iterh, NULL);
        gtk_tree_store_set (modeloutput, &iterh, 0, holiday->label,  -1); //1, "Child node",

        //ui_set_statusbar(TRUE,"selected row is: %d", holiday->factor01);
        holiday++;
      }
      
  //  month++;
  //}
  //return GTK_TREE_MODEL (modeloutput);
}

/*
	gtk_tree_store_append (store, &iter, NULL);
	gtk_tree_store_set (store, &iter, 0, "TORQUE", -1);

	for (i = 0; i < 4; i++) {
		gtk_tree_store_append (store, &child, &iter);
		if (i % 2 != 0)
			gtk_tree_store_set (store, &child, 0, "Child node",1, "Child node",  -1);
		else
			gtk_tree_store_set (store, &child, 0, "Child,1, "Child node",  -1);
	}

  
	gtk_tree_store_append (store, &iter, NULL);
	gtk_tree_store_set (store, &iter, 0, "POWER", -1);

	for (i = 0; i < 4; i++) {
		gtk_tree_store_append (store, &child2, &iter);
		if (i % 2 == 0)
			gtk_tree_store_set (store, &child2, 0, "Other node",1, "Child node", -1);
		else
			gtk_tree_store_set (store, &child2, 0, "Other",1, "Child node", -1);
	}

	// The teardown will also destroy the model
	// scroll_fixture_setup (fixture, GTK_TREE_MODEL (store), test_data);

  return GTK_TREE_MODEL(store);
}*/


// gboolean view

/*
void
age_cell_data_func (GtkTreeViewColumn *col,
                    GtkCellRenderer   *renderer,
                    GtkTreeModel      *model,
                    GtkTreeIter       *iter,
                    gpointer           user_data)
{
  guint  year_born;
  guint  year_now = 0; // to save code not relevant for the example 
  gchar  buf[64];

  gtk_tree_model_get(model, iter, COL_YEAR_BORN, &year_born, -1);

  if (year_born <= year_now && year_born > 0)
  {
    guint age = year_now - year_born;

    g_snprintf(buf, sizeof(buf), "%u years old", age);

    g_object_set(renderer, "foreground-set", FALSE, NULL); // print this normal
  }
  else
  {
    g_snprintf(buf, sizeof(buf), "Category");

    // make red 
    g_object_set(renderer, "foreground", "Red", "foreground-set", TRUE, NULL);
  }

  g_object_set(renderer, "text", buf, NULL);
}
*/

static void 
printrownumber (GtkTreePath* path)
{
    GtkTreeIter child_iter2;
    gchar  *namer;
    gint   *numberer;
    gfloat *numberf;
    GtkTreeIter* iterp;
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(viewinput));

    if(gtk_tree_selection_get_selected(selection, &modelinput, &child_iter2))
      {
        //GtkTreePath* path = gtk_tree_model_get_path(modelinput, &child_iter2);
        gint* pathindex = gtk_tree_path_get_indices (path);
        
        //gtk_tree_model_get (modelinput, &child_iter2, 0, &namer, 1, &numberf, 5, &numberer, -1);
        // gtk_tree_model_get (modelinput, &child_iter2, 5, &numberer, -1);
        //ui_set_statusbar(TRUE,"selected row is: %s %f %d", namer, numberf, &numberer); //  %d
        ui_set_statusbar(TRUE,"selected row is: %d", *pathindex);
        //g_printf("");
        //fflush(stdout);
        
        output_tree_setup(*pathindex);
        create_view_and_model_out();
        
        //g_free(namer);
        //g_free(numberer);
      }
    else
      {
        g_print ("no row selected.\n");
    }
}

static void 
printrownumberout (
  GtkTreeView* view,
  GtkTreePath* path,
  GtkTreeViewColumn* column,
  gpointer user_data)
{

    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(viewoutput));

    if(gtk_tree_selection_get_selected(selection, &modeloutput, &outputiter))
      {
        gchar *name;
        gtk_tree_model_get (modeloutput, &outputiter, 0, &name, -1);
        // g_print ("selected row is: %s\n", name);
        ui_set_statusbar(TRUE,"selected row is: %s", name);

        g_free(name);
        
      }
    else
      {
        g_print ("no row selected.\n");
    }
}


static GtkWidget *
create_view_and_model (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;

  viewinput = gtk_tree_view_new();
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection((GTK_TREE_VIEW (viewinput))), GTK_SELECTION_SINGLE);
  gtk_tree_view_set_activate_on_single_click(viewinput,TRUE);
  /* --- Column #1 --- */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Name");
  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewinput), col);
  renderer = gtk_cell_renderer_text_new();
  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  // connect 'text' property of the cell renderer to
  //  model column that contains the first name 
  gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

  /* --- Column #3 --- */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Age");
  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewinput), col);
  renderer = gtk_cell_renderer_text_new();
  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  /* connect a cell data function */
  // gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);
  gtk_tree_view_column_add_attribute(col, renderer, "text", 1);

  modelinput = input_tree_setup();
  
  gtk_tree_view_set_model(GTK_TREE_VIEW(viewinput), modelinput);
  g_object_unref(modelinput); /* destroy model automatically with view */
  //gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),GTK_SELECTION_NONE);
  return viewinput;
}


static GtkWidget *
create_view_and_model_out (void)
{
  GtkTreeViewColumn   *col;
  GtkCellRenderer     *renderer;
  GtkWidget           *viewoutput;

  viewoutput = gtk_tree_view_new();
  gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(viewoutput)),
                              GTK_SELECTION_SINGLE);
  gtk_tree_view_set_activate_on_single_click(viewoutput,TRUE);

  /* --- Column #1 --- */

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Name");
  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewoutput), col);
  renderer = gtk_cell_renderer_text_new();
  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  /* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
  gtk_tree_view_column_add_attribute(col, renderer, "text", 0);

  /* --- Column #3 --- */
  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "factor");
  /* pack tree view column into tree view */
  gtk_tree_view_append_column(GTK_TREE_VIEW(viewoutput), col);
  renderer = gtk_cell_renderer_text_new();
  /* pack cell renderer into tree view column */
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  /* connect a cell data function */
  // gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func, NULL, NULL);
  gtk_tree_view_column_add_attribute(col, renderer, "text", 1);

  gtk_tree_view_set_model(GTK_TREE_VIEW(viewoutput), modeloutput);
  g_object_unref(modeloutput); /* destroy model automatically with view */

  //g_signal_connect(viewoutput, "row-activated", G_CALLBACK(printrownumberout),NULL);


  return viewoutput;
}


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

void on_clicked_row(
  GtkTreeView* view,
  GtkTreePath* path,
  GtkTreeViewColumn* column,
  gpointer user_data)
{
  printrownumber(path);

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
  //output_tree_setup(0);
	GtkWidget *viewoutput = create_view_and_model_out ();


	label = gtk_label_new ("Input");

  // void gtk_box_pack_start(GtkBox* box, GtkWidget* child, gboolean expand, gboolean fill, guint padding )

  /*
  gtk_container_add(GTK_CONTAINER(treeview_vbox), view);
	gtk_container_add(GTK_CONTAINER(sidebar_vbox), label);
	gtk_container_add(GTK_CONTAINER(sidebar_vbox), treeview_vbox);
  */
  gtk_box_pack_start(GTK_BOX(treeview_vbox), view,1,1,0);
  gtk_box_pack_end(GTK_BOX(treeview_vbox), viewoutput,1,0,0);
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
    output_tree_setup(1);
    create_sidebar_addition();
    // gtk_notebook_set_current_page(GTK_NOTEBOOK(geany->main_widgets->sidebar_notebook), page_number);

    g_signal_connect(viewinput, "row-activated", G_CALLBACK(on_clicked_row),NULL);
}