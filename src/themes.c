/*
 * Based on code from gtk-theme-switch2.
 * Copyright Maher Awamy <muhri@muhri.net>
 *           Aaron Lehman <aaronl@vitelus.com>
 */

#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include <glib.h>
#include <gtk/gtk.h>

#include "themes.h"

GtkSettings *settings;

/* Sets rc_file to the rc_file of the theme if the result is true.
 * It is the caller's repsonsibility to free rc_file */
static gboolean
is_themedir (gchar *path)
{       
	gboolean is_gtk_theme = FALSE;
	gchar *test_rc_file;
	struct stat info;

	test_rc_file = g_strdup_printf ("%s/gtk-2.0/gtkrc",path);
	if (stat(test_rc_file, &info) == 0 && (S_ISREG(info.st_mode) || S_ISLNK(info.st_mode)))
		is_gtk_theme = TRUE;

	g_free (test_rc_file);

	return is_gtk_theme;
}

GList*
get_themes (void)
{
	gchar *homedir;
	gchar *dirname;
	DIR *dir;
	struct dirent *dent;
	struct stat stats;
	gchar *origdir=g_get_current_dir(); /* back up cwd */
	GList *list=0;
	gchar *full_path;

	homedir = (gchar*)getenv ("HOME");
	settings = gtk_settings_get_default ();
	
	dirname = g_strconcat(homedir,"/.themes",NULL);
	chdir (dirname);
	dir = opendir (dirname);
	if (dir)
	{
		while ((dent = readdir (dir)))
		{
			stat(dent->d_name,&stats);
			if (!S_ISDIR(stats.st_mode)) continue;
			if (strcmp(dent->d_name,"." ) == 0) continue;
			if (strcmp(dent->d_name,"..") == 0) continue;

			full_path = g_strconcat (homedir, "/.themes/", dent->d_name, NULL);
			if (!is_themedir (full_path))
			{
				g_free (full_path);
				continue;
			}
			list = g_list_insert_sorted(list, dent->d_name, (GCompareFunc)strcmp);
		}
	}
	
	g_free (dirname);
	
	dirname = (gchar*)gtk_rc_get_theme_dir();
	chdir (dirname);
	dir = opendir (dirname);
	if (dir)
	{
	        while ((dent = readdir (dir)))
	        {
			stat(dent->d_name,&stats);
			if (!S_ISDIR(stats.st_mode)) continue;
			if (strcmp(dent->d_name, "." ) == 0) continue;
			if (strcmp(dent->d_name, "..") == 0) continue;

			full_path = g_strconcat (dirname, "/", dent->d_name, NULL);
			if (!is_themedir (full_path))
			{
				g_free (full_path);
				continue;
			}
			list = g_list_insert_sorted(list, dent->d_name, (GCompareFunc)strcmp);
	        }
	}

	chdir (origdir); /* Go back to where we were */
	g_free (dirname);
	g_free (origdir);

	return list;
}

static void
select_theme (gpointer data, gpointer userdata)
{
	GList       *children = gtk_container_get_children (GTK_CONTAINER (data));
	GtkWidget   *label    = g_list_first (children)->data;
	const gchar *theme    = gtk_label_get_label (GTK_LABEL (label));

	gtk_settings_set_string_property (settings, "gtk-theme-name", theme, "gtkrc:0");
}

static void
add_theme (gpointer data, gpointer user_data)
{
	GtkWidget *menu = (GtkWidget*)user_data;
	GtkWidget *item = gtk_menu_item_new_with_label ((gchar*)data);

	gtk_menu_append (menu, item);
	g_signal_connect (G_OBJECT (item), "activate", (GCallback)select_theme, NULL);
}

void
create_themes_menu (GtkWidget *menuitem)
{
	GtkWidget *menu   = gtk_menu_new ();
	GList     *themes = get_themes ();

	g_list_foreach (themes, add_theme, menu);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (menuitem), menu);
	gtk_widget_show_all (menu);
}

void
theme_refresh (void)
{
	GtkSettings *sets;
	gchar *theme;
	
	sets = gtk_settings_get_default ();

	g_object_get (G_OBJECT (sets), "gtk-theme-name", &theme, NULL);

	gtk_settings_set_string_property (settings, "gtk-theme-name", "Raleigh", "gtkrc:0");
	gtk_settings_set_string_property (settings, "gtk-theme-name", theme, "gtkrc:0");
}

