#ifndef THEMES_H
#define THEMES_H

GList*    get_themes              (void);
void      create_themes_menu      (GtkWidget *menuitem);
void      theme_refresh (void);
	
#endif /* THEMES_H */
