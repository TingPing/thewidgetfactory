/*
 * Copyright (C) 2007 RedFlag Ltd
 *
 * Author: Yang Hong <hongyang@redflag-linux.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <glib/gi18n.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "themes.h"

#define UI_FILE "twf.glade"

typedef struct {
	gchar *ui_file;
	GladeXML *gxml;
} AppData;

static AppData *d;

#define WID(n) glade_xml_get_widget (d->gxml, #n);

void on_scale_value_changed (GtkRange *range, gpointer user_data);
void on_button_refresh_clicked (GtkWidget *button, gpointer user_data);

static gchar *
get_ui_file (void)
{
	gchar *fn;

	fn = g_build_filename (".", UI_FILE, NULL);
	if (! g_file_test (fn, G_FILE_TEST_EXISTS)) {
		g_free (fn);
		fn = g_build_filename (TWF_DATA, UI_FILE, NULL);
		if (! g_file_test (fn, G_FILE_TEST_EXISTS)) {
			g_warning ("%s %s",
				_("Cann't load UI file. File not found:"), fn);
			g_free (fn);
			return NULL;
		}
	}

	return fn;
}

void create_ui (void)
{
	GtkWidget *window;
	GtkWidget *combobox1;
	GtkWidget *combobox2;
	GtkWidget *comboboxentry1;
	GtkWidget *comboboxentry2;
	GtkWidget *treeview;
	GtkWidget *switch_theme;

	gint i;
	GtkTreeIter iter;
	GtkListStore *store;
	GtkCellRenderer *renderer;

	d = g_new0 (AppData, 1);

	d->ui_file = get_ui_file ();
	d->gxml = glade_xml_new (d->ui_file, NULL, NULL);
	glade_xml_signal_autoconnect (d->gxml);

	window = WID(window_main);

	combobox1 = WID(combobox1);
	combobox2 = WID(combobox2);

	comboboxentry1 = WID(comboboxentry1);
	comboboxentry2 = WID(comboboxentry2);

	treeview = WID(treeview2);

	gtk_combo_box_set_active (GTK_COMBO_BOX (combobox1), 0);
	gtk_combo_box_set_active (GTK_COMBO_BOX (combobox2), 0);

	gtk_combo_box_set_active (GTK_COMBO_BOX (comboboxentry1), 0);
	gtk_combo_box_set_active (GTK_COMBO_BOX (comboboxentry2), 0);

	renderer = gtk_cell_renderer_text_new ();

	for (i = 0; i < 3; i ++) {
		gchar *name;
		GtkTreeViewColumn *column;

		name = g_strdup_printf ("Column%d", i);
		column = gtk_tree_view_column_new_with_attributes (name,
				renderer,
				"text", i,
				NULL);
		gtk_tree_view_column_set_reorderable (column, TRUE);
		gtk_tree_view_column_set_resizable (column, TRUE);
		gtk_tree_view_column_set_sort_column_id (column, i);
		gtk_tree_view_append_column (GTK_TREE_VIEW(treeview), column);

		g_free (name);
	}

	store = gtk_list_store_new (3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	for (i = 0; i < 41; i ++) {
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
				0, "Show",
				1, "ItemName",
				2, "Misc",
				-1);
	}

	gtk_tree_view_set_model (GTK_TREE_VIEW (treeview), GTK_TREE_MODEL (store));

	switch_theme = WID(switch_theme);

	create_themes_menu (switch_theme);

	gtk_widget_show_all (window);
}

void
on_scale_value_changed (GtkRange *range,
			gpointer user_data)
{
	GtkWidget *scales[4];
	GtkWidget *bars[4];
	GtkWidget *harmony;
	gdouble value;
	int i;

	scales[0] = WID(hscale1);
	scales[1] = WID(hscale2);
	scales[2] = WID(vscale1);
	scales[3] = WID(vscale2);

	bars[0]   = WID(progressbar1);
	bars[1]   = WID(progressbar2);
	bars[2]   = WID(progressbar3);
	bars[3]   = WID(progressbar4);

	value = gtk_range_get_value (range);

	harmony = WID(harmony);

	if (!gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (harmony)))
		return;

	for (i=0; i<4; i++)
	{
		if (scales[i] != (GtkWidget*)range)
			gtk_range_set_value (GTK_RANGE (scales[i]), value);

		gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (bars[i]), value/100.0);
	}
}

void
on_button_refresh_clicked (GtkWidget *button,
			   gpointer user_data)
{
	theme_refresh ();
}

int
main (int argc, char *argv[])
{

#ifdef ENABLE_NLS
	bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	textdomain (GETTEXT_PACKAGE);
#endif

	gtk_set_locale ();
	gtk_init (&argc, &argv);

	create_ui ();

	gtk_main ();

	return 0;
}

