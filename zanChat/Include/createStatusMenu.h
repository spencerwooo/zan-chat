#ifndef CREATESTATUSMENU_H
#define CREATESTATUSMENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

/**************************************************/
/*名称：		createStatus.h
/*描述：		用户状态设置、显示
/***************************************************/


GtkWidget *onlineStatusLabel, *onlineStatusButton, *status_menu;

char* getpath(char* p);

gboolean statePress( GtkWidget *widget,GdkEvent *event );

void state_response(gchar *string);

GtkWidget* createStatusMenu();

#endif

