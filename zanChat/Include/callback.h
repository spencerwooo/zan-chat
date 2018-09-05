#ifndef CALLBACK_H
#define CALLBACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "createSettings.h"

/**************************************************/
/*名称：		callback.h
/*描述：		回调函数汇总
/***************************************************/


void destroyAll();

void loadSettings();

void show_about(void);

void remove_all(void);

void lin_refresh(void);

void trayView(GtkMenuItem *item, gpointer window);

void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu);

void trayIconActivated(GObject *trayIcon, gpointer window);

gboolean window_state_event (GtkWidget *widget, GdkEventWindowState *event, gpointer data);

#endif

