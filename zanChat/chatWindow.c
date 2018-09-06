#include "./Include/message.h"
#include "./Include/createSettings.h"
#include "./Include/chatWindow.h"
#include "./Include/calculator.h"
#include "./Include/calendar.h"
#include "./Include/screenShot.h"
#include "./Include/userInfo.h"
#include "./Include/chatRecord.h"
#include "./Include/createSettings.h"
#include "./Include/linpop.h"

/**************************************************/
/*名称：chatWindow.c
/*描述：聊天窗口及与聊天功能、小功能接口的定义
/*作成日期： 2017-09-05
/*作者： 周育聪、唐回峰、孙明君
/***************************************************/
textView *textViewAll;

char date_global[10];

/**************************************************/
/*名称：get_system_date
/*描述：获取当前时间
/*作成日期： 2017-09-02
/*参数：void
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void get_system_date()
{
	struct tm *prt;
	time_t It;

	It = time(NULL);
	prt = localtime(&It);
	int year, month, day;
	year = prt->tm_year + 1900;
	month = prt->tm_mon + 1;
	day = prt->tm_mday;
	date_global[0] = year / 1000 + 48;
	date_global[1] = (year % 1000) / 100 + 48;
	date_global[2] = (year % 100) / 10 + 48;
	date_global[3] = (year % 10) + 48;
	date_global[4] = (month / 10) + 48;
	date_global[5] = (month % 10) + 48;
	date_global[6] = (day / 10) + 48;
	date_global[7] = (day % 10) + 48;
	date_global[8] = '\0';
}
GdkPixbuf *create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if (!pixbuf)
	{
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}
void closeApp(GtkWidget *window, ip_window *ip_window_main)
{
	struct userinfo *u_info;
	u_info = seekUser(head, ip_window_main->ip);
	//saveRecord(u_info->record, u_info->ip, date_global);
	u_info->record = NULL;
	setOffTextView(ip_window_main->ip);
	gtk_widget_destroy(ip_window_main->window);
}
void int2str(int a, char *str)
{
	sprintf(str, "%d", a);
}

/**************************************************/
/*名称：xpm_label_box
/*描述：设置图片标签
/*作成日期： 2017-09-01
/*参数：
        参数1：gchar*、xpm_filename、文件名
        参数2：gchar*、label_text、文本控件
/*返回值：void
/*作者： 唐回峰
/***************************************************/
GtkWidget *xpm_label_box(gchar *xpm_filename, gchar *label_text)
{
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *image;

	box = gtk_hbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(box), 2);

	image = gtk_image_new_from_file(xpm_filename);

	label = gtk_label_new(label_text);

	gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 3);

	gtk_widget_show(image);
	gtk_widget_show(label);

	return box;
}

GtkWidget *label_xmp_box(gchar *label_text, gchar *xpm_filename)
{
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *image;

	box = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(box), 2);

	image = gtk_image_new_from_file(xpm_filename);

	label = gtk_label_new(label_text);

	gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 3);
	gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 3);

	gtk_widget_show(image);
	gtk_widget_show(label);

	return box;
}
/**************************************************/
/*名称：showMessage
/*描述：将接受的消息显示在消息窗口
/*作成日期： 2017-09-03
/*参数：
        参数1：char*、message、消息字符串
        参数2：textView*、textViewAll、消息显示控件
/*返回值：void
/*作者： 周育聪
/***************************************************/
void showMessage(char *message, textView *textViewAll)
{

	GtkTextIter start, end;
	GdkPixbuf *pixbuf;

	char content[1024];
	char time[30];
	char *name;
	char *delim = "#";
	char *ptr;
	char *text;
	char *filename;
	ptr = strstr(message, delim);
	text = strtok(message, delim);
	filename = strtok(NULL, delim);
	name = textViewAll->name;
	
	FILE *chatee;
	chatee = fopen("/tmp/zanChatLog", "r");
	
	char chateeName[50];
	memset(chateeName, 0, sizeof(chateeName));
	fgets(chateeName, 50, (FILE*)chatee);

	g_print("#########老狗币: %s\n", chateeName);
	getSystemTime(time);
	if (ptr == NULL)
	{
		printf("ptr == NULL\n");
		sprintf(content, "%s %s %s\n%s\n", chateeName, time, ":", text);
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, content, -1);
		memset(chateeName, 0, sizeof(chateeName));
	}
	else
	{
		if (strcmp(ptr, message) == 0)
		{
			sprintf(content, "%s %s %s\n", chateeName, time, ":");

			g_print("%s\n", content);

			gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, content, -1);

			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			pixbuf = gdk_pixbuf_new_from_file(text, NULL);

			gtk_text_buffer_insert_pixbuf(textViewAll->buffer1, &end, pixbuf);
			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, "\n", -1);

			g_print("%s\n", "RECEIVE");
			memset(chateeName, 0, sizeof(chateeName));
		}
		//printf("ptr != NULL\n");
		else
		{
			sprintf(content, "%s %s %s\n%s", chateeName, time, ":", text);

			g_print("%s\n", content);

			gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, content, -1);

			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

			gtk_text_buffer_insert_pixbuf(textViewAll->buffer1, &end, pixbuf);
			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, "\n", -1);

			g_print("%s\n", "RECEIVE");
			memset(chateeName, 0, sizeof(chateeName));
		}
	}
}


void showMessageFromLocal(char *message, textView *textViewAll)
{

	GtkTextIter start, end;
	GdkPixbuf *pixbuf;

	char content[1024];
	char time[30];
	char *name;
	char *delim = "#";
	char *ptr;
	char *text;
	char *filename;
	ptr = strstr(message, delim);
	text = strtok(message, delim);
	filename = strtok(NULL, delim);
	// name = textViewAll->name;

	FILE *un;
	un = fopen("/tmp/zanUserNameLog", "r");
	char userNameArray[50];
	memset(userNameArray, 0, sizeof(userNameArray));
	fgets(userNameArray, 50, (FILE*)un);
	
	// FILE *chatee;
	// chatee = fopen("/tmp/zanChatLog", "r");
	
	// char chateeName[50];
	// memset(chateeName, 0, sizeof(chateeName));
	// fgets(chateeName, 50, (FILE*)chatee);

	// g_print("#########老狗币: %s\n", chateeName);
	getSystemTime(time);
	if (ptr == NULL)
	{
		printf("ptr == NULL\n");
		sprintf(content, "%s %s %s\n%s\n", userNameArray, time, ":", text);
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, content, -1);
		// memset(chateeName, 0, sizeof(chateeName));
	}
	else
	{
		if (strcmp(ptr, message) == 0)
		{
			sprintf(content, "%s %s %s\n", userNameArray, time, ":");

			g_print("%s\n", content);

			gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, content, -1);

			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			pixbuf = gdk_pixbuf_new_from_file(text, NULL);

			gtk_text_buffer_insert_pixbuf(textViewAll->buffer1, &end, pixbuf);
			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, "\n", -1);

			g_print("%s\n", "RECEIVE");
			// memset(chateeName, 0, sizeof(chateeName));
		}
		//printf("ptr != NULL\n");
		else
		{
			sprintf(content, "%s %s %s\n%s", userNameArray, time, ":", text);

			g_print("%s\n", content);

			gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, content, -1);

			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			pixbuf = gdk_pixbuf_new_from_file(filename, NULL);

			gtk_text_buffer_insert_pixbuf(textViewAll->buffer1, &end, pixbuf);
			gtk_text_buffer_get_end_iter(textViewAll->buffer1, &end);
			gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, "\n", -1);

			g_print("%s\n", "RECEIVE");
			// memset(chateeName, 0, sizeof(chateeName));
		}
	}
}

/**************************************************/
/*名称：parse_image_path
/*描述：对聊天中引用的本地表情进行解析
/*作成日期： 2017-09-02
/*参数：
/*		参数1：int、num、待解析的表情代码
/*		参数2：char *、path、将对应表情图片路径在该字符串中
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void parse_image_path(int num, char *path)
{
	num++;
	char tmp_num[20] = {0};
	int2str(num, tmp_num);
	path[0] = '\0';
	strcat(path, "Source/Icon/Chat/");
	strcat(path, tmp_num);
	strcat(path, ".jpg");
	strcat(path, "\0");
}

/**************************************************/
/*名称：parse_avatar_path
/*描述：对聊天中引用的形象进行解析
/*作成日期： 2017-09-02
/*参数：
/*		参数1：int、num、待解析的形象代码
/*		参数2：char *、path、将对应形象图片路径在该字符串中
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void parse_avatar_path(int num, char *path)
{
	num++;
	char tmp_num[20] = {0};
	int2str(num, tmp_num);
	path[0] = '\0';
	strcat(path, "Source/Icon/User/");
	strcat(path, tmp_num);
	strcat(path, ".png");
	strcat(path, "\0");
}
void undo(GtkWidget *window, textView *textViewAll)
{
	gtk_text_buffer_set_text(textViewAll->buffer2, "", -1);
}
void font_selection_ok(GtkWidget *button, fontWindow_textView *fontWindow_textViewAll)
{

	GtkStyle *style;
	gchar *font;
	PangoFontDescription *font_desc;

	font = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(fontWindow_textViewAll->window));

	font_desc = pango_font_description_from_string(font);

	style = gtk_style_copy(gtk_widget_get_style(fontWindow_textViewAll->textViewAll->textView2));

	style->font_desc = font_desc;

	gtk_widget_set_style(fontWindow_textViewAll->textViewAll->textView2, style);
	gtk_widget_set_style(fontWindow_textViewAll->textViewAll->textView1, style);

	gtk_widget_destroy(GTK_OBJECT(fontWindow_textViewAll->window));
	g_print("font_ok");
}

/**************************************************/
/*名称：fontSelect
/*描述：回调函数，点击后弹出字体选择窗口，进行字体选择
/*作成日期： 2017-09-03
/*参数：
        参数1：GtkWidget *、 button、字体选择按钮
        参数2：textView *、textViewAll、文本控件
/*返回值：void
/*作者： 周育聪
/***************************************************/
void fontSelect(GtkWidget *button, textView *textViewAll)
{

	GtkWidget *window;

	window = gtk_font_selection_dialog_new("字体选择");

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_MOUSE);

	fontWindow_textView *fontWindow_textViewAll = (fontWindow_textView *)malloc(sizeof(fontWindow_textView));
	fontWindow_textViewAll->window = window;
	fontWindow_textViewAll->textViewAll = textViewAll;

	g_signal_connect(G_OBJECT(GTK_FONT_SELECTION_DIALOG(window)->ok_button),
					 "clicked", G_CALLBACK(font_selection_ok),
					 fontWindow_textViewAll);

	g_signal_connect(G_OBJECT(window), "destroy",
					 G_CALLBACK(gtk_widget_destroyed),
					 NULL);

	gtk_signal_connect_object(GTK_OBJECT(GTK_FONT_SELECTION_DIALOG(window)->cancel_button),
							  "clicked",
							  GTK_SIGNAL_FUNC(gtk_widget_destroy),
							  GTK_OBJECT(window));

	gtk_widget_show(window);

	g_print("font");
}

GtkWidget *icon;

/**************************************************/
/*名称：picButton_callback
/*描述：回调函数，点选表情图标后选中表情
/*作成日期： 2017-09-02
/*参数：
/*		参数1：GtkWidget *、button、对应表情按钮控件
/*		参数2：filename_textView *、filename_textViewAll、将表情存入此控件中
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void picButton_callback(GtkWidget *button, filename_textView *filename_textViewAll)
{
	GtkTextIter end;
	GdkPixbuf *pixbuf;
	filename_textViewAll->textViewAll->message = filename_textViewAll->filename;
	gtk_text_buffer_get_end_iter(filename_textViewAll->textViewAll->buffer2, &end);
	pixbuf = gdk_pixbuf_new_from_file(filename_textViewAll->filename, NULL);
	g_print("%s was pressed\n", (char *)filename_textViewAll->filename);
	gtk_text_buffer_insert_pixbuf(filename_textViewAll->textViewAll->buffer2, &end, pixbuf);
	gtk_widget_destroy(icon);
}

/**************************************************/
/*名称：face_button
/*描述：生成表情选择窗口，调用face_button函数，在框内加入各表情选择按键，并设置回调函数
/*作成日期： 2017-09-02
/*参数：
/*      参数1：gchar *filename，文本信息文件名
/*		参数2：textView*、textViewAll、文本信息
/*返回值：表情选择框的GtkWidget*指针
/*作者： 周育聪
/***************************************************/
GtkWidget *face_button(gchar *filename, textView *textViewAll)
{
	GtkWidget *image;
	GtkWidget *button;

	filename_textView *filename_textViewAll = (filename_textView *)malloc(sizeof(filename_textView));
	filename_textViewAll->textViewAll = textViewAll;
	filename_textViewAll->filename = filename;

	button = gtk_button_new();
	image = gtk_image_new_from_file(filename);

	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(picButton_callback), filename_textViewAll);

	gtk_container_add(GTK_CONTAINER(button), image);
	gtk_container_set_border_width(GTK_CONTAINER(button), 0);
	return button;
}

/**************************************************/
/*名称：create_bbox
/*描述：生成表情选择窗口
/*作成日期： 2017-09-02
/*参数：
/*      参数1：filename_textView *filename_textViewAll，文本信息，用于函数调用传递
/*返回值：表情选择框的GtkWidget*指针
/*作者： 周育聪
/***************************************************/
GtkWidget *create_bbox(filename_textView *filename_textViewAll)
{
	GtkWidget *vbox;
	GtkWidget *bbox;
	GtkWidget *button;
	vbox = gtk_vbox_new(FALSE, 0);
	/////////
	bbox = gtk_hbox_new(FALSE, 0);
	button = face_button("Source/emoji/1.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/2.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/3.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/4.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/5.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/6.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/7.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/8.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/9.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/10.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), bbox);
	/////////

	bbox = gtk_hbox_new(FALSE, 0);
	button = face_button("Source/emoji/11.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/12.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/13.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/14.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/15.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/16.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/17.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/18.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/19.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/20.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), bbox);
	////////
	bbox = gtk_hbox_new(FALSE, 0);
	button = face_button("Source/emoji/21.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/22.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/23.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/24.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/25.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/26.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/27.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/28.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/29.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/30.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), bbox);
	////////
	bbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/31.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/32.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/33.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/34.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/35.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/36.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/37.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/38.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/39.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/40.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), bbox);
	////////
	bbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/41.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/42.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/43.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/44.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/45.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/46.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/47.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/48.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/49.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	button = face_button("Source/emoji/50.png", filename_textViewAll);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), bbox);
	return vbox;
}

void addFace(GtkWidget *button, textView *textViewAll)
{

	GtkWidget *vbox;

	icon = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(icon), GTK_WIN_POS_CENTER);
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW(icon), TRUE);
	gtk_window_set_title(GTK_WINDOW(icon), "表情");

	g_signal_connect(G_OBJECT(icon), "destroy", G_CALLBACK(gtk_widget_destroy), icon);

	vbox = create_bbox(textViewAll);
	gtk_container_add(GTK_CONTAINER(icon), vbox);
	gtk_widget_show_all(icon);
}

/**************************************************/
/*名称：changeFace
/*描述：回调函数，更改皮肤
/*作成日期： 2017-09-04
/*参数：
/*      参数1：GtkButton *、button、按钮控件
/*		参数2：GtkButton *、button、按钮控件
/*返回值：void
/*作者： 唐回峰
/***************************************************/
void changeFace(GtkButton *button, GtkButton *window)
{
	GtkWidget *dialog;
	GtkColorSelection *colorsel;
	gint response;
	GdkColor color;

	dialog = gtk_color_selection_dialog_new("选择背景颜色");
	gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
	colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(dialog)->colorsel);
	gtk_color_selection_set_has_opacity_control(colorsel, TRUE);
	gtk_color_selection_set_has_palette(colorsel, TRUE);

	gtk_signal_connect(GTK_OBJECT(dialog), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroy), &dialog);
	gtk_color_selection_set_previous_color(colorsel, &color);
	gtk_color_selection_set_current_color(colorsel, &color);
	response = gtk_dialog_run(GTK_DIALOG(dialog));
	if (response == GTK_RESPONSE_OK)
	{
		gtk_color_selection_get_current_color(colorsel, &color);
		gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
	}

	gtk_widget_destroy(dialog);
}

char file_send_path[80];
extern int s;
char my_ip[30];
GtkWidget *createFileSelect()
{
	GtkWidget *filew;

	filew = gtk_file_selection_new("选择文件");

	gtk_window_set_position(GTK_WINDOW(filew), GTK_WIN_POS_MOUSE);

	/* 为cancel_button设置回调函数，销毁构件 */
	gtk_signal_connect_object(GTK_OBJECT(GTK_FILE_SELECTION(filew)->cancel_button),
							  "clicked",
							  GTK_SIGNAL_FUNC(gtk_widget_destroy),
							  GTK_OBJECT(filew));

	/* 设置文件名，比如这个一个文件保存对话框，我们给了一个缺省文件名 */
	gtk_file_selection_set_filename(GTK_FILE_SELECTION(filew),
									"penguin.png");
	gtk_widget_show(filew);
	return filew;
}

void file_ok_sel(GtkWidget *button, GtkFileSelection *fs)
{
	GFile *gfile = g_file_new_for_path(gtk_file_selection_get_filename(GTK_FILE_SELECTION(fs)));
	char *temp = g_file_get_basename(gfile);
	memset(file_send_path, 0, 80);
	strcpy(file_send_path, temp);

	struct sockaddr_in addr;
	bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(LINPOP_PORT);
	inet_pton(AF_INET, my_ip, &addr.sin_addr.s_addr);
	if (sendMsg(LINPOP_SENDFILEREQUEST, file_send_path, (struct sockaddr_in *)&addr, s) != 0)
	{
		perror("sendMsg file error\n");
		exit(0);
	}
	gtk_widget_destroy(fs);
}

/**************************************************/
/*名称：fileSelect
/*描述：显示确认窗口，文件发送功能
/*作成日期： 2017-09-03
/*参数：
/*      参数1：GtkButton *、button、按钮控件
/*		参数2：char*、data、文件路径信息
/*返回值：void
/*作者： 孙明君
/***************************************************/
void fileSelect(GtkWidget *button, char *data)
{
	strcpy(my_ip, data);
	GtkWidget *filew;
	filew = createFileSelect();
	/* 为ok_button按钮设置回调函数，连接到file_ok_sel function函数 */
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(filew)->ok_button),
					 "clicked",
					 G_CALLBACK(file_ok_sel), filew);
}
void chatRecordOk(GtkWidget *button, gpointer data)
{
	return;
}

/**************************************************/
/*名称：createChatWindow
/*描述：生成聊天窗口界面，聊天窗口主要函数
/*作成日期： 2017-09-04
/*参数：
/*      参数1：char *、ip、对方ip
/*		参数2：char *、name、对方用户名
/*		参数3：char *、msg、消息文本
/*		参数4：char *、sig、对方签名文本
/*		参数5 6：int、img_code avatar_code、对方形象代码
/*返回值：void
/*作者： 周育聪
/***************************************************/
void createChatWindow(char *ip, char *name, char *msg, char *sig, int img_code, int avatar_code)
{
	get_system_date();
	char title[50];

	GtkWidget *scrolled_window1, *scrolled_window2;
	GtkAccelGroup *gag;
	GtkWidget *image1, //个人形象图片
		*image2,	   //对方形象图片
		*image_letter,
		*image_emoj,
		*image_cut,
		*image_cld,
		*image_record,
		*image_file,
		*image_skin,
		*image_calc;
	GtkWidget *image_peo;
	GtkWidget *label;
	GtkWidget *label_name;
	//按钮
	GtkWidget *button_img,
		*button_skin,
		*button_file,
		*button_font,
		*button_emoj,
		*button_close,
		*button_send,
		*button_cld,
		*button_record,
		*button_cut,
		*button_calc;
	//盒子
	GtkWidget *hbox_main,
		*hbox_top_tool,
		*hbox_mid_tool,
		*hbox_bot_tool;

	GtkWidget *vbox_all,
		*vbox_chat,
		*vbox_img,
		*vbox_textview1,
		*vbox_textview2;
	GtkWidget *vbox_top;

	//对方用户名、ip及头像
	ip_window *ip_window_main = (ip_window *)malloc(sizeof(ip_window));
	ip_window_main->ip = ip;
	ip_window_main->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_icon(GTK_WINDOW(ip_window_main->window), create_pixbuf("Source/Icon/logo.png"));
	//printf("IN CREATECHATWINDOW\n");
	strcpy(title, name);
	strcat(title, "(");
	strcat(title, ip);
	strcat(title, ")");

	gtk_window_set_resizable(GTK_WINDOW(ip_window_main->window), FALSE);
	//设置聊天窗口位置、大小及title
	gtk_window_set_position(GTK_WINDOW(ip_window_main->window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(ip_window_main->window), 600, 500);
	gtk_window_set_title(GTK_WINDOW(ip_window_main->window), title);

	scrolled_window1 = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_set_border_width(GTK_CONTAINER(scrolled_window1), 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
	//发送消息框滚动条设置
	scrolled_window2 = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_set_border_width(GTK_CONTAINER(scrolled_window2), 5);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_container_set_border_width(GTK_CONTAINER(ip_window_main->window), 2);
	//聊天窗口关闭
	gtk_signal_connect(GTK_OBJECT(ip_window_main->window), "destroy", GTK_SIGNAL_FUNC(closeApp), ip_window_main);

	vbox_chat = gtk_vbox_new(FALSE, 0);
	vbox_img = gtk_vbox_new(FALSE, 0);
	vbox_all = gtk_vbox_new(FALSE, 0);
	vbox_textview1 = gtk_vbox_new(FALSE, 0);
	vbox_textview2 = gtk_vbox_new(FALSE, 0);

	vbox_top = gtk_vbox_new(FALSE, 0);

	hbox_top_tool = gtk_hbox_new(FALSE, 0);
	hbox_mid_tool = gtk_hbox_new(FALSE, 0);
	hbox_bot_tool = gtk_hbox_new(FALSE, 0);
	hbox_main = gtk_hbox_new(FALSE, 0);

	//给image赋图片
	image1 = gtk_image_new_from_file("Source/Icon/Chat/ahri.jpeg");
	image2 = gtk_image_new_from_file("Source/Icon/Chat/lisin.jpeg");
	image_skin = gtk_image_new_from_file("Source/Icon/skin.png");
	image_file = gtk_image_new_from_file("Source/Icon/file.png");
	image_letter = gtk_image_new_from_file("Source/Icon/font.png");
	image_emoj = gtk_image_new_from_file("Source/Icon/emoji.png");
	image_cld = gtk_image_new_from_file("Source/Icon/calendar.png");
	image_record = gtk_image_new_from_file("Source/Icon/Chat/chatRecord.png");
	image_cut = gtk_image_new_from_file("Source/Icon/screenshot.png");
	image_calc = gtk_image_new_from_file("Source/Icon/calculator.png");

	//创建按钮
	button_img = gtk_button_new();
	button_skin = gtk_button_new();
	button_file = gtk_button_new();
	button_font = gtk_button_new();
	button_emoj = gtk_button_new();
	button_cld = gtk_button_new();
	button_record = gtk_button_new();
	button_cut = gtk_button_new();
	button_calc = gtk_button_new();

	char img_path[50] = {0};
	parse_avatar_path(avatar_code, img_path);
	//添加头像
	image_peo = gtk_image_new_from_file(img_path);

	label_name = gtk_label_new(NULL);
	char before[50] = "<span foreground='black' font_desc='30'>";
	strcat(before, name);
	char after[20] = "</span>";
	strcat(before, after);
	gtk_label_set_markup(GTK_LABEL(label_name), before);
	char before2[50] = "<span foreground='black' font_desc='10'>";
	strcat(before2, sig);
	char after2[20] = "</span>";
	strcat(before2, after);
	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), before2);
	//	printf("here done.\n");

	//设置图片按钮
	gtk_button_set_image(button_img, image2);
	gtk_button_set_image(button_skin, image_skin);
	gtk_button_set_image(button_file, image_file);
	gtk_button_set_image(button_font, image_letter);
	gtk_button_set_image(button_emoj, image_emoj);
	gtk_button_set_image(button_cld, image_cld);
	gtk_button_set_image(button_record, image_record);
	gtk_button_set_image(button_cut, image_cut);
	gtk_button_set_image(button_calc, image_calc);

	//创建使用信息提示
	GtkTooltips *button_tips;
	button_tips = gtk_tooltips_new();
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_skin, "皮肤", "skin");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_file, "文件", "file");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_font, "字体", "letter");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_emoj, "表情", "emoj");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_cld, "日历", "cld");
	//gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_record, "记录", "record");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_cut, "截屏", "cut");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), button_calc, "计算器", "calc");
	gtk_tooltips_set_tip(GTK_TOOLTIPS(button_tips), image_peo, "头像", "peo");

	//底部功能按钮
	button_close = gtk_button_new_with_label("清空(C)");
	button_send = gtk_button_new_with_label("发送(S)");
	textView *textViewAll = (textView *)malloc(sizeof(textView));
	textViewAll->ip = ip;
	textViewAll->name = name;
	strcpy(textViewAll->message, "\0");

	textViewAll->textView1 = gtk_text_view_new(); //textView1;
	textViewAll->textView2 = gtk_text_view_new(); //textView2;
	//printf("here done.\n");
	textViewAll->buffer1 = gtk_text_buffer_new(NULL); //buffer1;
	//printf("here done.\n");
	textViewAll->buffer2 = gtk_text_buffer_new(NULL); //buffer2;
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textViewAll->textView1), FALSE);
	textViewAll->buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textViewAll->textView1));
	textViewAll->buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textViewAll->textView2));
	//printf("here done.\n");
	//添加背景
	GdkPixbuf *pixbuf = NULL;
	GdkBitmap *bitmap = NULL;
	GdkPixmap *pixmap = NULL;
	gtk_widget_set_app_paintable(GTK_WIDGET(ip_window_main->window), TRUE);
	gtk_widget_realize(GTK_WIDGET(ip_window_main->window));
	// gdk函数读取png文件
	pixbuf = gdk_pixbuf_new_from_file("Source/Icon/Chat/7.jpg", NULL);
	// alpha小于128认为透明
	gdk_pixbuf_render_pixmap_and_mask(pixbuf, &pixmap, &bitmap, 128);
	// 设置透明蒙板
	gtk_widget_shape_combine_mask(GTK_WIDGET(ip_window_main->window), bitmap, 0, 0);
	// 设置窗口背景
	gdk_window_set_back_pixmap(ip_window_main->window->window, pixmap, FALSE);
	g_object_unref(pixbuf);
	g_object_unref(bitmap);
	g_object_unref(pixmap);

	setOnTextView(ip, textViewAll);

	gtk_box_pack_start(GTK_BOX(vbox_textview1), scrolled_window1, TRUE, TRUE, 0);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window1), textViewAll->textView1);

	gtk_box_pack_start(GTK_BOX(vbox_textview2), scrolled_window2, TRUE, TRUE, 0);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window2), textViewAll->textView2);

	gag = gtk_accel_group_new();
	gtk_widget_add_accelerator(button_send, "clicked", gag, GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(button_close, "clicked", gag, GDK_c, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_window_add_accel_group(GTK_WINDOW(ip_window_main->window), gag);

	//点击发送按钮，发送内容
	gtk_signal_connect(GTK_OBJECT(button_send), "clicked", GTK_SIGNAL_FUNC(sendToTextView1), textViewAll);
	//点击字体按钮，弹出字体设置窗口
	gtk_signal_connect(G_OBJECT(button_font), "clicked", G_CALLBACK(fontSelect), textViewAll);
	//点击表情按钮，弹出表情列表
	gtk_signal_connect(G_OBJECT(button_emoj), "clicked", G_CALLBACK(addFace), textViewAll);
	//点击个人形象，弹出个人设置窗口
	gtk_signal_connect(G_OBJECT(button_img), "clicked", G_CALLBACK(createSettings), textViewAll);

	//点击更改皮肤按钮，弹出皮肤设置界面
	gtk_signal_connect(G_OBJECT(button_skin), "clicked", G_CALLBACK(changeFace), ip_window_main->window);
	//发送文件
	gtk_signal_connect(G_OBJECT(button_file), "clicked", G_CALLBACK(fileSelect), textViewAll->ip);
	//点击日历按钮，弹出日历窗口
	gtk_signal_connect(G_OBJECT(button_cld), "clicked", G_CALLBACK(calendar), NULL);
	//点击截屏按钮截屏
	gtk_signal_connect(G_OBJECT(button_cut), "clicked", G_CALLBACK(cb_screenShot), textViewAll);
	//点击计算器按钮，弹出计算器
	gtk_signal_connect(G_OBJECT(button_calc), "clicked", G_CALLBACK(createCalculator), NULL);

	//设置盒子大小
	gtk_widget_set_size_request(vbox_all, 555, 580);
	gtk_widget_set_size_request(hbox_mid_tool, 550, 40);
	gtk_widget_set_size_request(vbox_textview1, 550, 275);
	gtk_widget_set_size_request(vbox_textview2, 550, 120);
	gtk_widget_set_size_request(vbox_img, 200, 200);

	gtk_box_pack_start(GTK_BOX(vbox_all), hbox_top_tool, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_all), hbox_main, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_main), vbox_chat, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox_main), vbox_img, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_chat), vbox_textview1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_chat), hbox_mid_tool, 0, 0, 0); //1 1 0
	gtk_box_pack_start(GTK_BOX(vbox_chat), vbox_textview2, TRUE, TRUE, 0);
	gtk_box_pack_end(GTK_BOX(vbox_chat), hbox_bot_tool, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(vbox_img), image1, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox_img), button_img, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(hbox_top_tool), image_peo, FALSE, FALSE, 10);
	gtk_box_pack_start(GTK_BOX(hbox_top_tool), vbox_top, FALSE, FALSE, 10);

	gtk_box_pack_start(GTK_BOX(vbox_top), label_name, TRUE, FALSE, 1);
	gtk_box_pack_start(GTK_BOX(vbox_top), label, TRUE, FALSE, 1);

	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_font, 1, 1, 0); // 0 0 0
	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_emoj, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_cut, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_file, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_cld, 1, 1, 0);
	//gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_record, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_calc, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(hbox_mid_tool), button_skin, 1, 1, 0);

	gtk_box_pack_end(GTK_BOX(hbox_bot_tool), button_send, 1, 1, 0);
	gtk_box_pack_end(GTK_BOX(hbox_bot_tool), button_close, 1, 1, 0);

	//按钮
	gtk_button_set_relief(GTK_BUTTON(button_img), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_skin), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_file), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_font), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_emoj), GTK_RELIEF_NONE);
	//gtk_button_set_relief(GTK_BUTTON(button_close), GTK_RELIEF_NONE);
	//gtk_button_set_relief(GTK_BUTTON(button_send), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_cld), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_record), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_cut), GTK_RELIEF_NONE);
	gtk_button_set_relief(GTK_BUTTON(button_calc), GTK_RELIEF_NONE);

	/* Finally we add hbox to the window, and show all widgets */

	gtk_container_add(GTK_CONTAINER(ip_window_main->window), vbox_all);

	gtk_widget_show_all(ip_window_main->window);

	if (msg != NULL)
	{
		GtkTextIter start, end;
		gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer1), &start, &end);
		gtk_text_buffer_insert(GTK_TEXT_BUFFER(textViewAll->buffer1), &end, msg, -1);
	}

	gtk_main();
}
