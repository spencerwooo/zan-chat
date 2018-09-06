#include "./Include/createStatusMenu.h"

/**************************************************/
/*名称：createStatusMenu.c
/*描述：对当前状态选取对应图标路径
/*作成日期： 2018-09-01
/*作者： 李文煜、万伯阳、邵子涵
/***************************************************/

/**************************************************/
/*名称：		getpath
/*描述：		对于一个状态描述，获取它对应图片路径
/*作成日期： 2018-08-30
/*参数：
/*		参数1：char *、p、状态字符串文本
/*返回值：char *、图片路径
/*
/*作者：		邵子涵
/***************************************************/
char *getpath(char *p)
{
	int i;
	char *a[5] = {"我在线上", "请勿打扰", "离开", "忙碌", "隐身"};
	char *b[5] = {"./Source/Icon/OnlineStatus/available.svg", "./Source/Icon/OnlineStatus/away.svg", "./Source/Icon/OnlineStatus/busy.svg", "./Source/Icon/OnlineStatus/idle.svg", "./Source/Icon/OnlineStatus/invisible.svg"};
	for (i = 0; i < 5; i++)
		if (strcmp(p, a[i]) == 0)
			break;
	return b[i];
}
gboolean statePress(GtkWidget *widget, GdkEvent *event)
{
	if (event->type == GDK_BUTTON_PRESS)
	{
		GdkEventButton *bevent = (GdkEventButton *)event;
		gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL, bevent->button, bevent->time);
		return TRUE;
	}
	return FALSE;
}
void state_response(gchar *string)
{
	extern GtkWidget *onlineStatusButton;
	extern GtkWidget *onlineStatusLabel;
	GtkWidget *image;
	image = gtk_image_new_from_file(getpath(string));
	gtk_button_set_image(GTK_BUTTON(onlineStatusButton), image);
	gtk_label_set_text(GTK_LABEL(onlineStatusLabel), string);
}
/**************************************************/
/*名称：createStatusMenu
/*描述：生成状态选择栏，设置对应图片、文本
/*作成日期： 2018-09-04
/*参数： void
/*返回值：void
/*作者： 李文煜、万伯阳
/***************************************************/
GtkWidget *createStatusMenu()
{
	GtkWidget *menu_state, *menu_items_state;
	int i;
	char buf[128];
	menu_state = gtk_menu_new();
	sprintf(buf, "我在线上");
	for (i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0:
			sprintf(buf, "我在线上");
			break;
		case 1:
			sprintf(buf, "请勿打扰");
			break;
		case 2:
			sprintf(buf, "离开");
			break;
		case 3:
			sprintf(buf, "忙碌");
			break;
		case 4:
			sprintf(buf, "隐身");
			break;
		}
		menu_items_state = gtk_menu_item_new_with_label(buf);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu_state), menu_items_state);
		g_signal_connect_swapped(G_OBJECT(menu_items_state), "activate", G_CALLBACK(state_response), (gpointer)g_strdup(buf));
		gtk_widget_show(menu_items_state);
	}
	return menu_state;
}
