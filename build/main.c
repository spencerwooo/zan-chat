#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>
#include <gdk/gdkkeysyms.h>

#include "./Include/converter.h"
#include "./Include/callback.h"
#include "./Include/models.h"
#include "./Include/createStatusMenu.h"
#include "./Include/chatWindow.h"
#include "./Include/userInfo.h"
#include "./Include/message.h"
#include "./Include/rightClick.h"
#include "./Include/linpop.h"

/**************************************************/
/*名称：		main.c
/*描述：		主界面窗口，定义主调函数、初始化函数等功能主函数
/*作成日期： 2017-09-06
/*作者：		唐回峰、周育聪
/***************************************************/

#define FILEPATH_SIZE 80

/*全局变量*/

GtkWidget *vbox_main;	//主窗口容器
GtkWidget *window;		 //主窗口
GtkWidget *name, *photo; //个人信息组件

GtkWidget *usr_state_buffer; //签名
GtkWidget *usr_state;
int my_img_code = 0;
int my_avatar_code = 0;

char user_name[100];
char user_group[100];
char sigh[100];

char img_code_str[255];
char avatar_code_str[255];
char date_global[10];
char sex[10] = "男";
int s;

GtkWidget *entry; //好友列表中获取选项的值

GtkWidget *view; //显示好友列表
// struct userinfo * head;		//好友列表链表头结点
GtkWidget *vbox_mid;

GtkStatusIcon *trayIcon;

struct sockaddr_in addr_server;

//存储用户信息链表
struct userinfo *head;

//两个线程描述符号
pthread_t ui_thread, chat_thread; //chat_thread专门接受数据包，然后进行处理

struct Msg *Msghead = NULL;

/**************************************************/
/*名称：create_file_dir
/*描述：创建程序所需文件夹
/*作成日期： 2017-08-31
/*参数：void
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void create_file_dir()
{
	char file_path[80] = {0};
	getcwd(file_path, FILEPATH_SIZE);
	char file_dir[20] = "/FileRecv";
	strcat(file_path, file_dir);
	if (access(file_path, F_OK) == 0)
	{
		return;
	}

	mkdir(file_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
void create_chat_file_dir()
{
	char file_path[80] = {0};
	getcwd(file_path, 80);
	char chat_file_dir[20] = "/ChatRecord";
	strcat(file_path, chat_file_dir);
	if (access(file_path, F_OK) == 0)
	{
		return;
	}
	mkdir(file_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

/**************************************************/
/*名称：enter_ip
/*描述：回调函数，显示ip查询窗口
/*作成日期： 2017-09-02
/*参数：
/*		参数1：GtkWidget *、widget、点击按钮控件
/*		参数2：GtkWidget *、entry、输入窗口
/*返回值：void
/*作者： 周育聪
/***************************************************/
void enter_ip(GtkWidget *widget, GtkWidget *entry)
{
	const gchar *userip;
	struct userinfo *temp;
	gtk_editable_delete_text(GTK_EDITABLE(entry), 0, 3);
	userip = gtk_entry_get_text(GTK_ENTRY(entry));
	temp = head;
	while (temp != NULL)
	{
		if (0 == strcmp(userip, temp->ip))
		{
			createChatWindow(temp->ip, temp->name, NULL, temp->signature, temp->image_code, temp->avatar_code);
		}
		temp = temp->next;
	}
	gtk_entry_set_text(GTK_ENTRY(entry), "IP:");
}

/**************************************************/
/*名称：createUserStaff
/*描述：初始化用户信息显示区域控件
/*作成日期： 2017-09-02
/*参数：void
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void createUserStaff()
{
	GtkWidget *hbox_out;
	hbox_out = gtk_hbox_new(FALSE, 8);
	GtkWidget *hbox_user;
	hbox_user = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_main), hbox_out, FALSE, TRUE, 10);
	gtk_box_pack_start(GTK_BOX(hbox_out), hbox_user, FALSE, TRUE, 10);

	// user icon
	photo = NULL;
	photo = picToButtonConverter("./Source/Icon/User/4.png");
	gtk_button_set_relief(GTK_BUTTON(photo), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(photo), FALSE);
	gtk_widget_set_size_request(GTK_BUTTON(photo), 80, 80);
	gtk_box_pack_start(GTK_BOX(hbox_user), photo, FALSE, TRUE, 0);
	g_signal_connect(G_OBJECT(photo), "clicked", G_CALLBACK(loadSettings), NULL);

	GtkWidget *vbox_name = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox_user), vbox_name, FALSE, TRUE, 10);
	GtkWidget *hbox_name = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_name), hbox_name, FALSE, TRUE, 10);
	usr_state_buffer = gtk_entry_buffer_new(sigh, -1);
	usr_state = gtk_entry_new_with_buffer(usr_state_buffer);
	gtk_box_pack_start(GTK_BOX(vbox_name), usr_state, FALSE, TRUE, 10);

	// user name
	name = gtk_label_new(NULL);
	gtk_label_set_text(GTK_LABEL(name), user_name);
	gtk_box_pack_start(GTK_BOX(hbox_name), name, FALSE, TRUE, 10);

	// online status
	onlineStatusLabel = gtk_label_new("在线");
	onlineStatusButton = picToButtonConverter("./Source/Icon/OnlineStatus/available.svg");
	status_menu = createStatusMenu();
	gtk_box_pack_start(GTK_BOX(hbox_name), onlineStatusButton, FALSE, TRUE, 0);
	g_signal_connect_swapped(G_OBJECT(onlineStatusButton), "event", G_CALLBACK(statePress), G_OBJECT(status_menu));
	gtk_box_pack_start(GTK_BOX(hbox_name), onlineStatusLabel, FALSE, TRUE, 0);
}

void on_changed(GtkWidget *widget, gpointer entry)
{
	enum
	{
		COLUMN = 0,
		NUM_COLS
	};
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter))
	{
		gtk_tree_model_get(model, &iter, COLUMN, &value, -1);
		gtk_entry_set_text(GTK_ENTRY(entry), value);

		talk_item(widget, entry);

		g_free(value);
		talk_item(widget, entry);
	}
}
GtkWidget *view;

/**************************************************/
/*名称：remove_all
/*描述：清除主窗口列表中信息
/*作成日期： 2017-09-01
/*参数：void
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void remove_all(void)
{

	GtkTreeStore *store;
	GtkTreeModel *model;
	GtkTreeIter iter;

	store = GTK_TREE_STORE(gtk_tree_view_get_model(
		GTK_TREE_VIEW(view)));
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));

	if (gtk_tree_model_get_iter_first(model, &iter) == FALSE)
		return;
	gtk_tree_store_clear(store);
}

/**************************************************/
/*名称：getVboxMid
/*描述：生成主界面中好友列表，调用全局变量head
/*		注意：head并非在本函数初始化
/*作成日期： 2017-09-02
/*参数：void
/*返回值：void
/*作者： 周育聪
/***************************************************/
void getVboxMid()
{
	enum
	{
		COLUMN = 0,
		NUM_COLS
	};
	GtkWidget *hbox_mid;
	GtkWidget *talk;
	GtkWidget *look;
	GtkWidget *group_talk;
	GtkWidget *sw;
	GtkTreeSelection *selection;
	GtkWidget *menu_right;

	vbox_mid = gtk_vbox_new(FALSE, 0);
	gtk_widget_set_size_request(GTK_WIDGET(vbox_mid), 220, 470);

	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;

	GtkTreeModel *model;

	view = gtk_tree_view_new();
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);

	GtkTreeStore *treestore;
	GtkTreeIter toplevel, child;
	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING);
	struct userinfo a[50];
	char *p;
	char q[50];
	int b[50] = {0}, top = 0, num, i, j;
	struct userinfo *temp;
	temp = head;

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
				gtk_tree_store_append(treestore, &toplevel, NULL);
				gtk_tree_store_set(treestore, &toplevel, COLUMN, p, -1);
				break;
			}
		for (i = j; i < top; i++)
		{
			if (b[i] == 0)
				if (!strcmp(p, a[i].group))
				{
					gtk_tree_store_append(treestore, &child, &toplevel);
					strcpy(q, a[i].name);
					strcat(q, "(");
					strcat(q, a[i].ip);
					strcat(q, ")");
					gtk_tree_store_set(treestore, &child, COLUMN, q, -1);
					b[i] = 1;
					num--;
				}
		}
	}
	model = GTK_TREE_MODEL(treestore);
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);
	sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
	gtk_box_pack_start(GTK_BOX(vbox_mid), sw, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(sw), view);

	entry = gtk_entry_new();
	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), entry);

	g_signal_connect(selection, "changed", G_CALLBACK(talk_item), entry);
	menu_right = getMenu_right();
	ga_add_popup_menu(view, GTK_MENU(menu_right));

	return NULL;
}

/**************************************************/
/*名称：create
/*描述：生成主窗口界面，初始化
/*作成日期： 2017-09-02
/*参数：void
/*返回值：void
/*作者： 唐回峰、周育聪
/***************************************************/
void *create(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	setlocale(LC_ALL, "zh_CN.UTF-16");
	create_file_dir();
	create_chat_file_dir();
	// Level1-window framework
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(GTK_WIDGET(window), 300, 700);
	gtk_window_move(GTK_WINDOW(window), 1200, 0);
	//gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	//gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window), TRUE);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroyAll), NULL);
	g_signal_connect_swapped(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_widget_destroy), G_OBJECT(window));
	gtk_window_set_title(GTK_WINDOW(window), "zanChat");
	gtk_window_set_icon(GTK_WINDOW(window), strToPixbufConverter("./Source/Icon/mushRoomlogo.jpg"));

	// skin settings
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_set_app_paintable(GTK_WIDGET(window), TRUE);
	gtk_widget_realize(GTK_WIDGET(window));
	pixbuf = gdk_pixbuf_new_from_file("Source/Skins/1.jpg", NULL);
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);
	gtk_widget_shape_combine_mask(GTK_WIDGET(window), bitmap, 0, 0);
	gdk_window_set_back_pixmap(window->window, pixmap, FALSE);
	g_object_unref(pixbuf);
	g_object_unref(pixmap);
	g_object_unref(bitmap);

	GtkWidget *menu, *menuItemView, *menuItemExit, *menuItemHelp;
	menu = gtk_menu_new();
	menuItemExit = gtk_menu_item_new_with_label("退出");
	menuItemHelp = gtk_menu_item_new_with_label("帮助");
	menuItemView = gtk_menu_item_new_with_label("主面板");
	g_signal_connect(G_OBJECT(menuItemView), "activate", G_CALLBACK(trayView), window);
	g_signal_connect(G_OBJECT(menuItemExit), "activate", G_CALLBACK(destroyAll), NULL);
	g_signal_connect(G_OBJECT(menuItemHelp), "activate", G_CALLBACK(show_about), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItemView);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItemHelp);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItemExit);
	gtk_widget_show_all(menu);

	trayIcon = gtk_status_icon_new_from_file("Icon/1.png");
	gtk_status_icon_set_tooltip(trayIcon, "mushRoom");
	g_signal_connect(GTK_STATUS_ICON(trayIcon), "activate", GTK_SIGNAL_FUNC(trayIconActivated), window);
	g_signal_connect(GTK_STATUS_ICON(trayIcon), "popup-menu", GTK_SIGNAL_FUNC(trayIconPopup), menu);
	gtk_status_icon_set_visible(trayIcon, TRUE);
	gtk_status_icon_set_blinking(trayIcon, FALSE);
	g_signal_connect(G_OBJECT(window), "window-state-event", G_CALLBACK(window_state_event), NULL);

	vbox_main = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox_main);

	// level 1, user icon, signature, status
	createUserStaff();

	// ip
	GtkWidget *entry_ip = gtk_entry_new();
	gtk_widget_set_tooltip_text(entry_ip, "搜索好友");
	gtk_entry_set_max_length(GTK_ENTRY(entry_ip), 20);
	gtk_widget_set_size_request(GTK_WIDGET(entry_ip), 195, 25);
	g_signal_connect(G_OBJECT(entry_ip), "activate", G_CALLBACK(enter_ip), (gpointer)entry_ip);
	gtk_entry_set_icon_from_pixbuf(GTK_ENTRY(entry_ip), GTK_ENTRY_ICON_SECONDARY, create_pixbuf("Source/Icon/find.svg"));
	gtk_entry_set_icon_tooltip_text(GTK_ENTRY(entry_ip), GTK_ENTRY_ICON_SECONDARY, "搜索好友");
	gtk_entry_set_text(GTK_ENTRY(entry_ip), "IP:");
	gtk_box_pack_start(GTK_BOX(vbox_main), entry_ip, FALSE, FALSE, 0);

	getVboxMid();
	gtk_box_pack_start(GTK_BOX(vbox_main), vbox_mid, FALSE, FALSE, 0);

	gtk_widget_show_all(window);

	//createChatWindow("127.0.0.1", "Someone's name", "2017-09-06-10:03\nhello - -\0", "Hello it's zyc -- ", 1, 1);

	gtk_main();
}

/**************************************************/
/*名称：init_socket
/*描述：初始化socket连接等信息
/*作成日期： 2017-09-04
/*参数：
        参数1：int*、sock、套接字
        参数2：sockaddr_in*、addr、本地socket信息
/*返回值：void
/*作者： 唐回峰
/***************************************************/
int init_socket(int *sock, struct sockaddr_in *addr)
{
	int rc;
	*sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (*sock < 0)
		die("socket");

	memset(addr, 0, sizeof(struct sockaddr_in));

	(*addr).sin_family = AF_INET;
	(*addr).sin_addr.s_addr = htonl(INADDR_ANY);
	(*addr).sin_port = htons(LINPOP_PORT);

	if (bind(*sock, (struct sockaddr *)addr, sizeof(struct sockaddr_in)))
		die("bind");
	return 0;
}
void *ui_start()
{
	create(0, NULL);
}

/**************************************************/
/*名称：main
/*描述：主函数，程序开始的接口
/*作成日期： 2017-09-02
/*参数：
        参数1：GtkWidget *、 button
        参数2：filename_textView *filename_textViewAll
/*返回值：void
/*作者： 唐回峰
/***************************************************/
int main(int argc, char *argv[])
{

	char *str = "Spencer";
	strcpy(user_name, str);
	strcpy(user_group, "my friend");
	strcpy(sigh, "ahh");
	init_socket(&s, &addr_server);
	head = NULL;
	pthread_create(&ui_thread, NULL, (void *)ui_start, NULL);
	pthread_create(&chat_thread, NULL, (void *)chat_start, NULL);

	pthread_join(chat_thread, NULL);
	pthread_join(ui_thread, NULL);
	//
	return 0;
}
