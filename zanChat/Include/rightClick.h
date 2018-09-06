#ifndef GETMENU_RIGHT_H
#define GETMENU_RIGHT_H
#include <gtk/gtk.h>

/**********************************************/
/*
名称：	GtkWidget* getMenu_right(void);
描述：  右键功能
*/
/**********************************************/
GtkWidget *getMenu_right(void);

void look_item(GtkWidget *widget, gpointer entry);

void talk_item(GtkWidget *widget, gpointer entry);

void grouptalk(GtkWidget *widget, gpointer entry);


void right_response(gchar *string);

static gint right_button_press(GtkWidget *wid, GdkEvent *event);

void ga_add_popup_menu(GtkWidget *wid, GtkMenu *menu);

void get_usr_info(char *name, char *group, char *ip, char *sig, int img_code, int avatar_code);

extern GtkWidget *entry;

#endif
