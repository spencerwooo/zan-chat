#ifndef CREATESETTINGS_H
#define CREATESETTINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

/**************************************************/
/*名称：		createSettings.h
/*描述：		设置页面的功能实现及显示
/***************************************************/

enum {
	PIXBUF_COLUMN,
	TEXT_COLUMN,
	N_COLUMN
};
typedef struct _listitem ListItem;
struct _listitem {
	const gchar *pix;
	const gchar *text;
};

typedef struct _item_image ListItemImage;
struct _item_image{
	 gchar *fcname;
	 gchar *fcimage;
};
void send_info(GtkWidget *button, gpointer data);

GtkListStore* createListModel(void);

GtkWidget* createListView(GtkListStore *list_store);

GtkWidget *createItem(gint i);

void notebookOnChanged(GtkWidget *widget, gpointer notebook);

void entry_text(GtkWidget *widget,gpointer entry);

void createSettings();

void setUserName(GtkWidget *widget,gpointer entry);

void entry_group(GtkWidget *widget,gpointer entry);

void entry_sig(GtkWidget *widget,gpointer entry);

void set_soft_quit(GtkWidget *widget,GtkWidget *set_soft);

void my_callback_function(GtkWidget *widget,gpointer combo);


#endif
