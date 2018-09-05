#include "./Include/createSettings.h"
#include "./Include/converter.h"
#include "./Include/linpop.h"
#include "./Include/createStatusMenu.h"
#include "./Include/models.h"

/**************************************************/
/*名称：		createSetting.c
/*描述：		设置窗口的显示和功能实现
/*作成日期： 2017-09-04
/*作者：		唐回峰、周育聪、刘静、郭莹婷、邓竣麟、孙明君
/***************************************************/

#define STR_SIZE 1005

int hasOpened;
char userString[STR_SIZE];
GtkWidget *fcbutton;
GtkWidget *name_entry;
GtkWidget *sig_entry;

extern GtkWidget *window;
extern GtkWidget *photo;
extern GtkWidget *vbox_main;
extern char img_code_str[255];
extern char avatar_code_str[255];
extern char sex[10];
extern int my_img_code;
extern int my_avatar_code;
extern char user_name[100];
extern char user_group[100];
extern char sigh[100];
extern int s;

void send_info(GtkWidget *button, gpointer data)
{
}

// 定义选项图标等属性路径
ListItem t[5] = {
	{"Source/Icon/Settings/1.png", "基本设置"},
	{"Source/Icon/Settings/2.png", "系统设置"},
	{"Source/Icon/Settings/3.png", "功能设置"},
	{"Source/Icon/Settings/4.png", "网络设置"}};

ListItemImage g[11] = {
	{"头像一", "Source/Icon/User/1.png"},
	{"头像二", "Source/Icon/User/2.png"},
	{"头像三", "Source/Icon/User/3.png"},
	{"头像四", "Source/Icon/User/4.png"},
	{"头像五", "Source/Icon/User/5.png"},
	{"头像六", "Source/Icon/User/6.png"},
	{"头像七", "Source/Icon/User/7.png"},
	{"头像八", "Source/Icon/User/8.png"},
	{"头像九", "Source/Icon/User/9.png"},
	{"头像十", "Source/Icon/User/10.jpg"}};

/**************************************************/
/*名称：		createListModel
/*描述：		生成设置窗口中的GtkListStore控件
/*作成日期： 2017-08-31
/*参数：		void
/*返回值：	GtkListStore *
/*
/*作者：		唐回峰
/***************************************************/
GtkListStore *createListModel()
{
	GtkListStore *list_store;
	GtkTreeIter iter;
	gint i;
	GdkPixbuf *pixbuf;
	list_store = gtk_list_store_new(N_COLUMN, GDK_TYPE_PIXBUF, G_TYPE_STRING);
	for (i = 0; i < 4; i++)
	{
		pixbuf = gdk_pixbuf_new_from_file(t[i].pix, NULL);
		gtk_list_store_append(list_store, &iter);
		gtk_list_store_set(list_store, &iter, PIXBUF_COLUMN, pixbuf, TEXT_COLUMN, t[i].text, -1);
	}
	return list_store;
}

GtkWidget *createListView(GtkListStore *list_store)
{
	GtkWidget *view;
	GtkTreeModel *model;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	model = GTK_TREE_MODEL(list_store);
	view = gtk_tree_view_new_with_model(model);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(view), FALSE);
	renderer = gtk_cell_renderer_pixbuf_new();
	column = gtk_tree_view_column_new_with_attributes(
		"Icon", renderer,
		"pixbuf", PIXBUF_COLUMN,
		NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("wenben", renderer, "text", TEXT_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), column);
	return view;
}

/**************************************************/
/*名称：createItem
/*描述：生成头像选择下拉框中的单个表情选择标签
/*作成日期： 2017-09-04
/*参数：
/*      参数1：gint、i、选中的头像代码
/*返回值：表情选择框的GtkWidget*指针
/*作者： 周育聪
/***************************************************/
GtkWidget *createItem(gint i)
{
	GtkWidget *item;
	GtkWidget *image;
	GtkWidget *hbox;

	hbox = gtk_hbox_new(FALSE, 0);
	switch (i)
	{
	case 1:
		image = gtk_image_new_from_file(g[0].fcimage);
		break;
	case 2:
		image = gtk_image_new_from_file(g[1].fcimage);
		break;
	case 3:
		image = gtk_image_new_from_file(g[2].fcimage);
		break;
	case 4:
		image = gtk_image_new_from_file(g[3].fcimage);
		break;
	case 5:
		image = gtk_image_new_from_file(g[4].fcimage);
		break;
	case 7:
		image = gtk_image_new_from_file(g[6].fcimage);
		break;
	case 8:
		image = gtk_image_new_from_file(g[7].fcimage);
		break;
	case 9:
		image = gtk_image_new_from_file(g[8].fcimage);
		break;
	case 10:
		image = gtk_image_new_from_file(g[9].fcimage);
		break;
	case 6:
		image = gtk_image_new_from_file(g[5].fcimage);
		break;
	}
	gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 2);
	item = gtk_list_item_new();
	gtk_container_add(GTK_CONTAINER(item), hbox);
	gtk_widget_show_all(item);
	return item;
}

void notebookOnChanged(GtkWidget *widget, gpointer notebook)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;
	int i;

	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter))
	{
		gtk_tree_model_get(model, &iter, TEXT_COLUMN, &value, -1);
		for (i = 0; i < 4; i++)
		{
			if (0 == strcmp(t[i].text, value))
				gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), i);
		}
		g_free(value);
	}
}

/**************************************************/
/*名称：setUserName
/*描述：回调函数，设置并同步修改后的用户名
/*作成日期： 2017-09-04
/*参数：
/*      参数1：GtkWidget *、widget、点选按钮控件
/*		参数2：gpointer、entry、用户名显示文本框
/*返回值：void
/*作者： 刘静
/***************************************************/
void setUserName(GtkWidget *widget, gpointer entry)
{
	extern GtkWidget *name; //用户名
	const char *text;
	text = gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(user_name, text);
	strcpy(user_name, text);
	gtk_entry_set_text(GTK_ENTRY(entry), text);
	gtk_label_set_text(GTK_LABEL(name), text);
}

/**************************************************/
/*名称：entry_sig
/*描述：回调函数，设置并同步修改后的签名
/*作成日期： 2017-09-04
/*参数：
/*      参数1：GtkWidget *、widget、点选按钮控件
/*		参数2：gpointer、entry、签名显示文本框
/*返回值：void
/*作者： 周育聪、刘静
/***************************************************/
void entry_sig(GtkWidget *widget, gpointer entry)
{

	extern GtkWidget *usr_state; //签名
	gchar *text;
	text = gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(sigh, text);
	gtk_entry_set_text(GTK_ENTRY(entry), text);
	gtk_entry_set_text(GTK_ENTRY(usr_state), text);
}
void set_soft_quit(GtkWidget *widget, GtkWidget *set_soft)
{
	setUserName(NULL, name_entry);
	entry_sig(NULL, sig_entry);
	hasOpened = 0;
	gtk_widget_destroy(set_soft);
}

/**************************************************/
/*名称：setSkin1
/*描述：将聊天窗口设置为预置皮肤1，下同（对应四种风格皮肤）
/*作成日期： 2017-09-05
/*参数： void
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void setSkin1()
{
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_realize(GTK_WIDGET(vbox_main));
	pixbuf = gdk_pixbuf_new_from_file("Source/Skins/1.jpg", NULL);		// gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET(vbox_main), bitmap, 0, 0); // 设置透明蒙板
	gdk_window_set_back_pixmap(vbox_main->window, pixmap, FALSE);		// 设置窗口背景
	g_object_unref(pixbuf);
	g_object_unref(bitmap);
	g_object_unref(pixmap);
	gtk_widget_queue_draw(window);
}

void setSkin2()
{
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_realize(GTK_WIDGET(vbox_main));
	pixbuf = gdk_pixbuf_new_from_file("Source/Skins/2.jpg", NULL);		// gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET(vbox_main), bitmap, 0, 0); // 设置透明蒙板
	gdk_window_set_back_pixmap(vbox_main->window, pixmap, FALSE);		// 设置窗口背景
	g_object_unref(pixbuf);
	g_object_unref(bitmap);
	g_object_unref(pixmap);
	gtk_widget_queue_draw(window);
}

void setSkin3()
{
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_realize(GTK_WIDGET(vbox_main));
	pixbuf = gdk_pixbuf_new_from_file("Source/Skins/3.jpg", NULL);		// gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET(vbox_main), bitmap, 0, 0); // 设置透明蒙板
	gdk_window_set_back_pixmap(vbox_main->window, pixmap, FALSE);		// 设置窗口背景
	g_object_unref(pixbuf);
	g_object_unref(bitmap);
	g_object_unref(pixmap);
	gtk_widget_queue_draw(window);
}

void setSkin4()
{
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_realize(GTK_WIDGET(vbox_main));
	pixbuf = gdk_pixbuf_new_from_file("Source/Skins/4.jpg", NULL);		// gdk函数读取png文件
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);   // alpha小于128认为透明
	gtk_widget_shape_combine_mask(GTK_WIDGET(vbox_main), bitmap, 0, 0); // 设置透明蒙板
	gdk_window_set_back_pixmap(vbox_main->window, pixmap, FALSE);		// 设置窗口背景
	g_object_unref(pixbuf);
	g_object_unref(bitmap);
	g_object_unref(pixmap);
	gtk_widget_queue_draw(window);
}
void my_callback_function(GtkWidget *widget, gpointer combo)
{
	const gchar *string;
	GtkWidget *image;
	GtkWidget *image1;

	int i;
	string = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combo)->entry));
	for (i = 0; i < 10; i++)
	{
		if (0 == strcmp(g[i].fcname, string))
		{

			my_avatar_code = i;
			printf("Avatar changed %d\n", my_avatar_code);

			image = gtk_image_new_from_file(g[i].fcimage);
			image1 = gtk_image_new_from_file(g[i].fcimage);
			gtk_button_set_image(GTK_BUTTON(photo), image);
			gtk_button_set_image(GTK_BUTTON(fcbutton), image1);
			printf("%s\n", g[i].fcimage);
		}
	}
}

/**************************************************/
/*名称：createSettings
/*描述：调出设置窗口
/*作成日期： 2017-09-04
/*参数：void
/*返回值：void
/*作者： 唐回峰、郭莹婷
/***************************************************/
void createSettings()
{
	GtkWidget *soft_set;
	GtkWidget *hbox;
	GtkWidget *hbox1;
	GtkWidget *vbox;
	GtkWidget *notebook;
	GtkWidget *table;
	GtkWidget *table1;
	GtkWidget *view;
	GtkListStore *model;
	GtkWidget *frame;
	GtkWidget *label;
	// GtkWidget *label1;
	GtkWidget *button;
	GtkWidget *entry;

	GtkWidget *checkbutton;
	GtkWidget *item;
	GtkWidget *image;
	GtkWidget *combo;
	GtkWidget *radio;
	GtkWidget *vbox1;
	GtkWidget *notebook0;
	GtkWidget *button_1;
	GtkWidget *button_2;
	GtkWidget *button_3;
	GtkWidget *button_4;
	GdkColor blue = {0, 0, 0, 0xffff};
	GdkColor white = {0, 0xffff, 0xffff, 0xffff};
	GdkColor orange = {0, 0xffff, 0xAFAF, 0x2424};
	GdkColor grey = {0, 0xd300, 0xd300, 0xd300};

	GSList *group;
	gint i;
	GtkTreeSelection *selection;

	if (hasOpened == 0)
	{
		hasOpened = 1;
		soft_set = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(soft_set), "软件设置");
		gtk_window_set_position(GTK_WINDOW(soft_set), GTK_WIN_POS_CENTER);
		gtk_container_set_border_width(GTK_CONTAINER(soft_set), 5);
		gtk_widget_set_size_request(soft_set, 600, 490);
		gtk_window_set_icon(GTK_WINDOW(soft_set), filenameToPixbufConverter("Source/Icon/Settings/logo.jpg"));

		notebook0 = gtk_notebook_new();
		gtk_widget_modify_bg(notebook0, GTK_STATE_NORMAL, &white);
		gtk_widget_modify_bg(notebook0, GTK_STATE_SELECTED, &blue);
		gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook0), GTK_POS_TOP);

		hbox1 = gtk_hbox_new(FALSE, 0);
		hbox = gtk_hbox_new(FALSE, 0);
		vbox = gtk_vbox_new(FALSE, 0);
		gtk_container_add(GTK_CONTAINER(soft_set), vbox);
		gtk_container_add(GTK_CONTAINER(vbox), notebook0);
		gtk_container_add(GTK_CONTAINER(vbox), hbox1);

		model = createListModel();
		view = createListView(model);

		gtk_widget_modify_base(view, GTK_STATE_ACTIVE, &orange);
		gtk_widget_modify_base(view, GTK_STATE_SELECTED, &orange);

		gtk_box_pack_start(GTK_BOX(hbox), view, FALSE, FALSE, 2);
		//创建notebook
		notebook = gtk_notebook_new();
		gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
		gtk_notebook_set_show_tabs(GTK_NOTEBOOK(notebook), FALSE);
		gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), FALSE);
		gtk_box_pack_start(GTK_BOX(hbox), notebook, TRUE, TRUE, 2);

		for (i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
			{
				table = gtk_table_new(16, 4, TRUE);
				frame = gtk_frame_new("个人信息设置");
				gtk_table_attach_defaults(GTK_TABLE(table), frame, 0, 4, 0, 12);
				table1 = gtk_table_new(5, 4, FALSE);
				gtk_container_add(GTK_CONTAINER(frame), table1);
				label = gtk_label_new("用户名：");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 1, 0, 1);
				name_entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(name_entry), user_name);
				gtk_table_attach_defaults(GTK_TABLE(table1), name_entry, 1, 4, 0, 1);

				label = gtk_label_new("个性签名：");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 1, 1, 2);
				sig_entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(sig_entry), sigh);
				gtk_table_attach_defaults(GTK_TABLE(table1), sig_entry, 1, 4, 1, 2);

				label = gtk_label_new("性别");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 1, 2, 3);

				combo = gtk_combo_new();

				gtk_table_attach_defaults(GTK_TABLE(table1), combo, 1, 4, 2, 3);
				GList *items = NULL;

				//向列表里添加数据项 性别 - -
				items = g_list_append(items, "男");
				items = g_list_append(items, "女");
				items = g_list_append(items, "谜");
				gtk_combo_set_popdown_strings(GTK_COMBO(combo), items);

				label = gtk_label_new("头像：");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 1, 3, 4);

				combo = gtk_combo_new();
				//image = gtk_image_new_from_file("Icon/1.gif");
				item = createItem(1);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[0].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(2);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[1].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(3);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[2].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(4);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[3].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(5);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[4].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(6);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[5].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(7);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[6].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(8);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[7].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(9);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[8].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);

				item = createItem(10);
				gtk_combo_set_item_string(GTK_COMBO(combo), GTK_ITEM(item), g[9].fcname);
				gtk_container_add(GTK_CONTAINER(GTK_COMBO(combo)->list), item);
				gtk_table_attach_defaults(GTK_TABLE(table1), combo, 2, 3, 3, 4);

				fcbutton = picToButtonConverter("Source/Icon/User/1.png");
				gtk_button_set_relief(GTK_BUTTON(fcbutton), GTK_RELIEF_NONE);
				g_signal_connect(G_OBJECT(GTK_COMBO(combo)->entry), "changed", G_CALLBACK(my_callback_function), (gpointer)combo);
				gtk_table_attach_defaults(GTK_TABLE(table1), fcbutton, 1, 2, 3, 4);

				frame = gtk_frame_new("");
				gtk_container_add(GTK_CONTAINER(frame), table);

				label = gtk_label_new(t[i].text);
				gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, label);
			}
			break;
			//系统设置
			case 1:
			{
				table = gtk_table_new(11, 10, TRUE);
				checkbutton = gtk_check_button_new_with_label("开机自动运行");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 0, 1);

				checkbutton = gtk_check_button_new_with_label("开机自动运行时，程序自动最小化");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 1, 2);

				checkbutton = gtk_check_button_new_with_label("双击运行程序时，程序自动最小化");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 2, 3);

				checkbutton = gtk_check_button_new_with_label("左键单击托盘图标打开主界面");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 3, 4);

				checkbutton = gtk_check_button_new_with_label("程序运行过程中，占用内存自动最小化");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 4, 5);

				checkbutton = gtk_check_button_new_with_label("自动检查最新版本");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 5, 6);

				checkbutton = gtk_check_button_new_with_label("主面板自动隐藏");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 6, 7);

				checkbutton = gtk_check_button_new_with_label("始终保持在其他窗口前端");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 7, 8);

				checkbutton = gtk_check_button_new_with_label("保存聊天记录");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 8, 9);

				label = gtk_label_new("聊天记录保存路径");
				gtk_table_attach_defaults(GTK_TABLE(table), label, 1, 4, 9, 10);

				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "/home/neusoft/桌面");
				gtk_table_attach_defaults(GTK_TABLE(table), entry, 4, 10, 9, 10);

				frame = gtk_frame_new("");
				gtk_container_add(GTK_CONTAINER(frame), table);
				label = gtk_label_new(t[i].text);
				gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, label);
			}
			break;
				//功能设置
			case 2:
			{
				table = gtk_table_new(16, 10, TRUE);
				checkbutton = gtk_check_button_new_with_label("显示我的输入状态");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 0, 1);

				checkbutton = gtk_check_button_new_with_label("上线时通知其他用户");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 1, 2);

				checkbutton = gtk_check_button_new_with_label("下线时通知其他用户");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 2, 3);

				checkbutton = gtk_check_button_new_with_label("其他用户上线时，进行通知");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 3, 4);

				checkbutton = gtk_check_button_new_with_label("其他用户下线时，进行通知");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 4, 5);

				checkbutton = gtk_check_button_new_with_label("允许其他用户发出远程协助请求");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 5, 6);

				checkbutton = gtk_check_button_new_with_label("允许其他用户发出语音对话请求");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 6, 7);

				checkbutton = gtk_check_button_new_with_label("接收其他用户发出的表情和图片");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 7, 8);

				checkbutton = gtk_check_button_new_with_label("发送图片或表情时，压缩发送");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 8, 9);

				checkbutton = gtk_check_button_new_with_label("屏蔽垃圾信息");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 9, 10);

				checkbutton = gtk_check_button_new_with_label("允许其他用户发送闪屏振动");
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 10, 11);

				checkbutton = gtk_check_button_new_with_label("允许自动接收对方发送过来的文件");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 11, 12);

				checkbutton = gtk_check_button_new_with_label("不接收其他用户的所有消息(所有用户为黑名单)");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 12, 13);

				checkbutton = gtk_check_button_new_with_label("主界面用户列表中不显示已设置成黑名单的用户");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 13, 14);

				checkbutton = gtk_check_button_new_with_label("自动刷新");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 14, 15);

				checkbutton = gtk_check_button_new_with_label("在线状态时状态自动转换");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table), checkbutton, 1, 10, 15, 16);

				frame = gtk_frame_new("");
				gtk_container_add(GTK_CONTAINER(frame), table);

				label = gtk_label_new(t[i].text);
				gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, label);
			}
			break;
				//网络设置
			case 3:
			{
				table = gtk_table_new(16, 4, TRUE);
				frame = gtk_frame_new("多网卡电脑请指定");
				//gtk_widget_modify_fg(frame,GTK_STATE_NORMAL,&blue);
				gtk_table_attach_defaults(GTK_TABLE(table), frame, 0, 4, 0, 4);
				table1 = gtk_table_new(2, 6, TRUE);
				checkbutton = gtk_check_button_new_with_label("指定IP地址");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table1), checkbutton, 0, 2, 0, 1);
				checkbutton = gtk_check_button_new_with_label("指定MAC地址");
				gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
				gtk_table_attach_defaults(GTK_TABLE(table1), checkbutton, 0, 2, 1, 2);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "0 . 0 . 0 . 0");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 2, 4, 0, 1);
				entry = gtk_entry_new();
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 2, 4, 1, 2);
				label = gtk_label_new("修改后下次有效");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 4, 6, 0, 1);
				label = gtk_label_new("修改后下次有效");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 4, 6, 1, 2);
				gtk_container_add(GTK_CONTAINER(frame), table1);

				frame = gtk_frame_new("设置文件发送接收缓冲区大小(可用来限制文件传送速度)");
				gtk_table_attach_defaults(GTK_TABLE(table), frame, 0, 4, 4, 8);
				table1 = gtk_table_new(2, 8, TRUE);
				label = gtk_label_new("发送文件缓冲区大小:");
				gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 4, 0, 1);
				label = gtk_label_new("接收文件缓冲区大小:");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 4, 1, 2);
				gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "65536");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 4, 6, 0, 1);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "65536");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 4, 6, 1, 2);
				label = gtk_label_new("(字节)");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 6, 8, 0, 1);
				label = gtk_label_new("(字节)");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 6, 8, 1, 2);
				gtk_container_add(GTK_CONTAINER(frame), table1);

				frame = gtk_frame_new("设置图片发送参数");
				gtk_table_attach_defaults(GTK_TABLE(table), frame, 0, 4, 8, 12);
				table1 = gtk_table_new(2, 8, TRUE);
				label = gtk_label_new("UDP发送图片时，图片分段发送大小:");
				gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 4, 0, 1);
				label = gtk_label_new("重试次数:");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 2, 1, 2);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "512");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 4, 7, 0, 1);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "10");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 4, 7, 1, 2);
				label = gtk_label_new("(字节)");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 7, 8, 0, 1);
				gtk_container_add(GTK_CONTAINER(frame), table1);

				frame = gtk_frame_new("自定义端口和群聊组播地址---下次运行时才生效");
				gtk_table_attach_defaults(GTK_TABLE(table), frame, 0, 4, 12, 16);
				table1 = gtk_table_new(2, 6, TRUE);
				label = gtk_label_new("自定义端口:");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 2, 0, 1);
				label = gtk_label_new("群聊组播地址:");
				gtk_table_attach_defaults(GTK_TABLE(table1), label, 0, 2, 1, 2);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "2425");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 2, 5, 0, 1);
				entry = gtk_entry_new();
				gtk_entry_set_text(GTK_ENTRY(entry), "226. 81. 9 . 8");
				gtk_table_attach_defaults(GTK_TABLE(table1), entry, 2, 5, 1, 2);
				button = gtk_button_new_with_label("默认");
				gtk_table_attach_defaults(GTK_TABLE(table1), button, 5, 6, 0, 1);
				button = gtk_button_new_with_label("默认");
				gtk_table_attach_defaults(GTK_TABLE(table1), button, 5, 6, 1, 2);
				gtk_container_add(GTK_CONTAINER(frame), table1);

				frame = gtk_frame_new("");
				gtk_container_add(GTK_CONTAINER(frame), table);

				label = gtk_label_new(t[i].text);
				gtk_notebook_append_page(GTK_NOTEBOOK(notebook), frame, label);
			}
			break;
			}
		}

		GtkWidget *label = gtk_label_new("基本设置");
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook0), hbox, label);

		//7.第二个页面
		label = gtk_label_new("群组设置");
		hbox = gtk_hbox_new(FALSE, 0);
		GtkWidget *button1 = gtk_button_new_with_label("我是第二个页面");
		GtkWidget *button2 = gtk_button_new_with_label("我是第二个页面的按钮");
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook0), hbox, label);

		GtkWidget *table1 = gtk_table_new(9, 8, TRUE);
		gtk_container_add(GTK_CONTAINER(hbox), table1);
		vbox1 = gtk_vbox_new(FALSE, 0);
		frame = gtk_frame_new("群消息默认设置");
		gtk_container_add(GTK_CONTAINER(frame), vbox1);
		radio = gtk_radio_button_new_with_label(NULL, "接收并提示信息");
		gtk_widget_modify_bg(radio, GTK_STATE_SELECTED, &orange);
		gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
		group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

		radio = gtk_radio_button_new_with_label(group, "自动弹出消息");
		gtk_widget_modify_bg(radio, GTK_STATE_SELECTED, &orange);
		gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
		group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

		radio = gtk_radio_button_new_with_label(group, "消息来时只显示消息数目");
		gtk_widget_modify_bg(radio, GTK_STATE_SELECTED, &orange);
		gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
		group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

		radio = gtk_radio_button_new_with_label(group, "消息框提示信息内容");
		gtk_widget_modify_bg(radio, GTK_STATE_SELECTED, &orange);
		gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 5);
		group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

		radio = gtk_radio_button_new_with_label(group, "阻止一切该群的消息");
		gtk_widget_modify_bg(radio, GTK_STATE_SELECTED, &orange);
		gtk_box_pack_start(GTK_BOX(vbox1), radio, FALSE, FALSE, 10);
		group = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio));

		gtk_table_attach_defaults(GTK_TABLE(table1), frame, 0, 8, 0, 4);

		GtkWidget *table2 = gtk_table_new(5, 8, TRUE);
		frame = gtk_frame_new("群功能默认设置");
		gtk_table_attach_defaults(GTK_TABLE(table1), frame, 0, 8, 4, 8);
		gtk_container_add(GTK_CONTAINER(frame), table2);

		checkbutton = gtk_check_button_new_with_label("有用户加入群或退出群时，进行通知");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
		gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table1), checkbutton, 0, 8, 5, 6);

		checkbutton = gtk_check_button_new_with_label("不接收来自群内的图片");
		gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table1), checkbutton, 0, 8, 6, 7);

		checkbutton = gtk_check_button_new_with_label("收到消息时发出声音");
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton), TRUE);
		gtk_widget_modify_bg(checkbutton, GTK_STATE_SELECTED, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table1), checkbutton, 0, 8, 7, 8);
		gtk_container_add(GTK_CONTAINER(frame), table1);

		label = gtk_label_new("皮肤设置");
		hbox = gtk_hbox_new(TRUE, 5);
		frame = gtk_frame_new("皮肤设置");

		table = gtk_table_new(5, 1, TRUE);
		gtk_container_set_border_width(GTK_CONTAINER(table), 0);
		gtk_container_add(GTK_CONTAINER(frame), table);

		// GtkWidget *label1 = gtk_label_new("背景选择：");
		// gtk_table_attach_defaults(GTK_TABLE(table), label1, 0, 1, 0, 1);

		button_1 = gtk_button_new_with_label("素鼠");
		g_signal_connect(G_OBJECT(button_1), "clicked", G_CALLBACK(setSkin1), window);
		gtk_widget_modify_bg(button_1, GTK_STATE_PRELIGHT, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table), button_1, 0, 1, 0, 1);

		button_2 = gtk_button_new_with_label("利休白茶");
		g_signal_connect(G_OBJECT(button_2), "clicked", G_CALLBACK(setSkin2), window);
		gtk_widget_modify_bg(button_2, GTK_STATE_PRELIGHT, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table), button_2, 0, 1, 1, 2);

		button_3 = gtk_button_new_with_label("中红新橘");
		g_signal_connect(G_OBJECT(button_3), "clicked", G_CALLBACK(setSkin3), window);
		gtk_widget_modify_bg(button_3, GTK_STATE_PRELIGHT, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table), button_3, 0, 1, 2, 3);

		button_4 = gtk_button_new_with_label("露草");
		g_signal_connect(G_OBJECT(button_4), "clicked", G_CALLBACK(setSkin4), window);
		gtk_widget_modify_bg(button_4, GTK_STATE_PRELIGHT, &orange);
		gtk_table_attach_defaults(GTK_TABLE(table), button_4, 0, 1, 3, 4);

		gtk_notebook_append_page(GTK_NOTEBOOK(notebook0), frame, label);

		button = gtk_button_new_with_label("应用");
		gtk_widget_set_size_request(button, 70, 28);

		g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(setUserName), (gpointer)name_entry);
		g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(entry_sig), (gpointer)sig_entry);
		g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(send_info), NULL);
		gtk_box_pack_end(GTK_BOX(hbox1), button, FALSE, FALSE, 10);

		button = gtk_button_new_with_label("关闭");
		gtk_widget_set_size_request(button, 70, 28);
		g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(set_soft_quit), soft_set);
		gtk_box_pack_end(GTK_BOX(hbox1), button, FALSE, FALSE, 10);

		g_signal_connect(G_OBJECT(soft_set), "destroy", G_CALLBACK(set_soft_quit), soft_set);
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
		g_signal_connect(selection, "changed", G_CALLBACK(notebookOnChanged), notebook);

		gtk_widget_show_all(soft_set);
	}
}
