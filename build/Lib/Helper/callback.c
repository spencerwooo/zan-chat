#include "../../Include/callback.h"
#include "../../Include/createSettings.h"
#include "../../Include/msg_list.h"
#include "../../Include/userInfo.h"

/**************************************************/
/*名称：strToPixbufConverter
/*描述：回调函数汇总
/*作成日期： 2017-08-31
/***************************************************/

extern GtkStatusIcon *trayIcon;
extern struct Msg *Msghead;
extern struct userinfo *head;
enum
{
	COLUMN = 0,
	NUM_COLS
};
void destroyAll()
{
	gtk_main_quit();
	exit(0);
}

void loadSettings()
{
	createSettings();
}
void lin_refresh(void)
{

	GtkTreeStore *store;
	GtkTreeIter toplevel, child;
	struct userinfo a[50];
	struct userinfo *temp;
	char *p;
	char q[50];
	extern GtkWidget *view;
	temp = head;
	int b[50] = {0}, top = 0, num, i, j;
	printf("before remove all\n");
	remove_all();
	store = GTK_TREE_STORE(gtk_tree_view_get_model(
		GTK_TREE_VIEW(view)));
	while (temp != NULL)
	{
		a[top++] = *temp;
		temp = temp->next;
	}
	num = top;
	while (num != 0)
	{
		for (j = 0; j < top; j++)
			if (b[j] == 0)
			{
				p = a[j].group;
				gtk_tree_store_append(store, &toplevel, NULL);
				gtk_tree_store_set(store, &toplevel, COLUMN, p, -1);
				break;
			}
		for (i = j; i < top; i++)
		{
			if (b[i] == 0)
				if (0 == strcmp(p, a[i].group))
				{
					gtk_tree_store_append(store, &child, &toplevel);
					strcpy(q, a[i].name);
					strcat(q, "(");
					strcat(q, a[i].ip);
					strcat(q, ")");
					gtk_tree_store_set(store, &child, COLUMN, q, -1);
					b[i] = 1;
					num--;
				}
		}
	}
	printf("列表已刷新！\n\n");
}

void trayView(GtkMenuItem *item, gpointer window)
{
	gtk_widget_show(GTK_WIDGET(window));
	gtk_window_deiconify(GTK_WINDOW(window));
}
void show_about(void)
{
	const gchar const *rc_authors[] = {"team7", NULL, NULL};
	const gchar const *rc_documenters[] = {"team7", NULL, NULL};
	const gchar const *rc_artists[] = {"team7", NULL, NULL};
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "mushRoom");
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "mushRoom");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "V-1.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "copyright@team7");
	gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(dialog), "BY team 7.\n");
	gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(dialog), (const gchar **)rc_authors);
	gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(dialog), (const gchar **)rc_documenters);
	gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(dialog), (const gchar **)rc_artists);
	gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(dialog), "team7");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "hello - - - -");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

gboolean window_state_event(GtkWidget *widget, GdkEventWindowState *event, gpointer data)
{
	if (event->changed_mask == GDK_WINDOW_STATE_ICONIFIED && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
	{
		gtk_widget_hide(GTK_WIDGET(widget));
		gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
	}
	else if (event->changed_mask == GDK_WINDOW_STATE_WITHDRAWN && (event->new_window_state == GDK_WINDOW_STATE_ICONIFIED || event->new_window_state == (GDK_WINDOW_STATE_ICONIFIED | GDK_WINDOW_STATE_MAXIMIZED)))
	{
		gtk_status_icon_set_visible(GTK_STATUS_ICON(trayIcon), TRUE);
	}
	return TRUE;
}
/*********************************************/
//任务栏图标－单击
/*********************************************/
void trayIconActivated(GObject *trayIcon, gpointer window)
{
	if (gtk_status_icon_get_blinking(GTK_STATUS_ICON(trayIcon)) == FALSE)
	{
		gtk_widget_show(GTK_WIDGET(window));
		gtk_window_deiconify(GTK_WINDOW(window));
	}
	else
	{
		printf("-------------------[click to create window start]----------------\n");
		//if(Msghead != NULL)
		char ip[20];
		strcpy(ip, Msghead->ip);
		printf("ip:%s\n", ip);
		char name[20];
		strcpy(name, Msghead->name);
		printf("name:%s\n", name);
		char msg[1024];
		bzero(msg, 1024);
		getMsg(ip, msg);
		printf("msg:%s\n", msg);
		if (Msghead == NULL)
			gtk_status_icon_set_blinking(trayIcon, FALSE);

		createChatWindow(ip, name, msg, "hello", 1, 1);
		//showMessage(msg.message,p->textViewAll);

		printf("-------------------[click to create window end ]----------------\n");
	}
}
/*********************************************/
//任务栏图标－弹出菜单
/*********************************************/
void trayIconPopup(GtkStatusIcon *status_icon, guint button, guint32 activate_time, gpointer popUpMenu)
{
	gtk_menu_popup(GTK_MENU(popUpMenu), NULL, NULL, gtk_status_icon_position_menu, status_icon, button, activate_time);
}
