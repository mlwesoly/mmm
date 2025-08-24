/*
 *  mmm.c
 *
 *  Copyright 20 <>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <geanyplugin.h>

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <tinyexpr.h>
#include <locale.h>
#include <regex.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "mmm.h"
#include "print.h"
#include "matrixreader.h"
#include "converter.h"


GeanyPlugin	*geany_plugin;
GeanyData	*geany_data;

PLUGIN_VERSION_CHECK(224)

PLUGIN_SET_INFO("TVC tools for Geany", "Adds some helpful tools for doing TVCs with Geany.", "0.1", "Michael")

static GtkWidget *s_context_fdec_item; //, *s_context_sep_item;


//static GtkWidget *s_context_plotselection_item;

/* Keybinding(s) */
enum
{
	CALCULATE0_KB,
	CALCULATE1_KB,
	CALCULATE2_KB,
	CALCULATE3_KB,
	CALCULATE4_KB,
	CALCULATE5_KB,
	CALCULATE6_KB,
	CALCULATE7_KB,
	CALCULATE8_KB,
	CALCULATE9_KB,
	CALCULATE10_KB,
	OVERWRITE_BLOCK_SPACES_KB,
	COUNT_KB
};

#define sci_point_x_from_position(sci, position) \
	scintilla_send_message(sci, SCI_POINTXFROMPOSITION, 0, position)
#define sci_get_pos_at_line_sel_start(sci, line) \
	scintilla_send_message(sci, SCI_GETLINESELSTARTPOSITION, line, 0)
#define sci_get_pos_at_line_sel_end(sci, line) \
	scintilla_send_message(sci, SCI_GETLINESELENDPOSITION, line, 0)

static void update_display(void)
{
	while (gtk_events_pending())
		gtk_main_iteration();
}

static void overwrite_block_spaces(G_GNUC_UNUSED guint key_id)
{
	GeanyDocument *doc = document_get_current();
	ScintillaObject *sci = doc->editor->sci;

	gint start_pos = sci_get_selection_start(sci); // startpos der auswahl
	gint start_line = sci_get_line_from_position(sci, start_pos); //startzeile
	gint end_pos = sci_get_selection_end(sci); // endpos der auswahl
	gint end_line = sci_get_line_from_position(sci, end_pos); //endzeile

	gint xinsert = sci_point_x_from_position(sci, start_pos);
	gint xend = sci_point_x_from_position(sci, end_pos);
	gint insertcol = sci_get_pos_at_line_sel_start(sci, start_line);
	gint endcol = sci_get_pos_at_line_sel_end(sci, start_line);

	gint *line_pos = g_new(gint, end_line - start_line + 1);

	gint line, i;
	//gint64 start_value=1;
	//gint64 start = start_value;
	//gint64 value;
	unsigned count = 0;
	size_t length; //, lend;
	

	// ui_set_statusbar(TRUE,"startline %d %d",start_line,end_line);
	// ui_set_statusbar(TRUE,"xinsert %d %d => %d ",xinsert,xend,xend-xinsert-1);  // xinsert xend 
	// ui_set_statusbar(TRUE,"startpos %d %d",start_pos,end_pos);

	if (xend < xinsert)
		xinsert = xend;

	for (line = start_line, i = 0; line <= end_line; line++, i++)
	{
		if (sci_point_x_from_position(sci,
			scintilla_send_message(sci, SCI_GETLINEENDPOSITION, line, 0)) >= xinsert)
		{
			line_pos[i] = sci_get_pos_at_line_sel_start(sci, line) -
				sci_get_position_from_line(sci, line);
			count++;
		}
		else
			line_pos[i] = -1;
	}
	
	sci_start_undo_action(sci);
	sci_replace_sel(sci, "");

	gchar buffer[endcol-insertcol];
	memset(buffer,' ',(endcol-insertcol));
	length=(endcol-insertcol);
	buffer[length] = '\0';

	for (line = start_line, i = 0; line <= end_line; line++, i++)
	{
		gint insert_pos;

		if (line_pos[i] < 0)
			continue;

		insert_pos = sci_get_position_from_line(sci, line) + line_pos[i];
		sci_insert_text(sci, insert_pos, buffer);
	}

	sci_end_undo_action(sci);
	
	g_free(line_pos);
}

static void on_calculateX(G_GNUC_UNUSED guint Stellen)
{
	GeanyDocument *doc = document_get_current();
	GtkClipboard *clipboard, *primary;
	gchar *f_content; //, *f_content_tmp;
	int error;
	long double b=0.0;

	gint selection_end;
	gint selection_start;

	if (doc && !doc->readonly)
	{
		// get the current document and write selection to f_content
		ScintillaObject *sci = doc->editor->sci;
		f_content = sci_get_selection_contents(doc->editor->sci);

		b = te_interp(f_content, &error);
		
		if(!error)
		{
			char buf[sizeof(b)+1];
			switch (Stellen)
			{
				case 0:
					sprintf(buf, "%.0Lf", b);
					break;
				case 1:
					sprintf(buf, "%.1Lf", b);
					break;
				case 2:
					sprintf(buf, "%.2Lf", b);
					break;
				case 3:
					sprintf(buf, "%.3Lf", b);
					break;
				case 4:
					sprintf(buf, "%.4Lf", b);
					break;
				case 5:
					sprintf(buf, "%.5Lf", b);
					break;
				case 6:
					sprintf(buf, "%.6Lf", b);
					break;
				case 7:
					sprintf(buf, "%.7Lf", b);
					break;
				case 8:
					sprintf(buf, "%.8Lf", b);
					break;
				case 9:
					sprintf(buf, "%.9Lf", b);
					break;
				case 10:
					sprintf(buf, "%.10Lf", b);
					break;
			}
			selection_end = sci_get_selection_end(sci);
			selection_start = sci_get_selection_start(sci);
			sci_insert_text(sci, selection_end, " ");
			sci_insert_text(sci, selection_end+1, buf);
			ui_set_statusbar(TRUE, "result: %s", buf);

			sci_set_current_position (sci, selection_end+1, TRUE);
			sci_set_selection_start (sci,selection_start);
			sci_set_selection_end (sci,selection_end+1);
			
			clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
			primary = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
			gtk_clipboard_set_text(clipboard,buf,sizeof(buf));
			gtk_clipboard_set_text(primary, buf,sizeof(buf));
		}		
	}
}

/*
GtkWidget * plugin_configure (GtkDialog *dialog)
{
	// call_plugin_configure(dialog);
}
*/

void plugin_init(G_GNUC_UNUSED GeanyData *data)
{
	GeanyKeyGroup *plugin_key_group;
	//GeanyKeyGroup *key_group;
	setlocale(LC_NUMERIC, "C");
	setlocale(LC_ALL, "C");
	int contextmenu = 1;

	plugin_key_group = plugin_set_key_group(geany_plugin, "MMM_Group", COUNT_KB, NULL);

	/*
	main_menu_item = gtk_menu_item_new_with_mnemonic(_("test _TVC..."));
	gtk_widget_show(main_menu_item);

	gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), main_menu_item);

	g_signal_connect(main_menu_item, "activate", G_CALLBACK(on_calculate4),
		NULL);
	*/
	
	keybindings_set_item(plugin_key_group, CALCULATE0_KB, on_calculateX,
		0, 0, "calculate0", _("calculate0"), NULL);
	
	keybindings_set_item(plugin_key_group, CALCULATE1_KB, on_calculateX,
		0, 0, "calculate1", _("calculate1"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE2_KB, on_calculateX,
		0, 0, "calculate2", _("calculate2"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE3_KB, on_calculateX,
		0, 0, "calculate3", _("calculate3"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE4_KB, on_calculateX,
		0, 0, "calculate4", _("calculate4"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE5_KB, on_calculateX,
		0, 0, "calculate5", _("calculate5"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE6_KB, on_calculateX,
		0, 0, "calculate6", _("calculate6"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE7_KB, on_calculateX,
		0, 0, "calculate7", _("calculate7"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE8_KB, on_calculateX,
		0, 0, "calculate8", _("calculate8"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE9_KB, on_calculateX,
		0, 0, "calculate9", _("calculate9"), NULL);
	keybindings_set_item(plugin_key_group, CALCULATE10_KB, on_calculateX,
		0, 0, "calculate10", _("calculate10"), NULL);

	keybindings_set_item(plugin_key_group, OVERWRITE_BLOCK_SPACES_KB, overwrite_block_spaces, 
		0, 0, "overwrite_block_spaces", _("overwrite_block_spaces"), NULL);

	
    s_context_fdec_item = gtk_menu_item_new_with_mnemonic(_("2D graph printer"));
	gtk_widget_show(s_context_fdec_item);
	gtk_menu_shell_prepend(GTK_MENU_SHELL(geany->main_widgets->editor_menu), s_context_fdec_item);
	g_signal_connect((gpointer) s_context_fdec_item, "activate", G_CALLBACK(printer), NULL);
	
	initthegui();
	printf("hello");
	fflush(stdout);
	/*	
	if(contextmenu == 1){

		s_context_sep_item = gtk_separator_menu_item_new();
		gtk_widget_show(s_context_sep_item);
		gtk_menu_shell_prepend(GTK_MENU_SHELL(geany->main_widgets->editor_menu), s_context_sep_item);
		
		s_context_plotselection_item = gtk_menu_item_new_with_mnemonic(_("plot selection"));
		gtk_widget_show(s_context_plotselection_item);
		gtk_menu_shell_prepend(GTK_MENU_SHELL(geany->main_widgets->editor_menu), s_context_plotselection_item);
		g_signal_connect((gpointer) s_context_plotselection_item, "activate", G_CALLBACK(update_display), NULL);
		
	}

	s_context_sep_item = gtk_separator_menu_item_new();
	gtk_widget_show(s_context_sep_item);
	gtk_menu_shell_prepend(GTK_MENU_SHELL(geany->main_widgets->editor_menu), s_context_sep_item);
	*/


	//gtk_widget_set_sensitive(GTK_WIDGET(s_context_fdec_item), TRUE);
}

void plugin_cleanup(void)
{
	//gtk_widget_destroy(main_menu_item);
	gtk_widget_destroy(s_context_fdec_item);
	//gtk_widget_destroy(s_context_sep_item);
}
