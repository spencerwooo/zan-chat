#include "../../Include/screenShot.h"
/**************************************************/
/*名称：screenShot.c
/*描述：截屏功能的相关函数和接口
/*作成日期： 2018-09-01
/*作者：刘嘉伟、韩昌云
/***************************************************/

/**************************************************/
/*名称：		getScreenShot
/*描述：		截屏并保存到文件
/*作成日期：	2018-08-31
/*参数：		
/*		参数1：char *、file、文件路径
/*		参数2：int、sec、等待时间（用于保存）
/*返回值：	VOID
/*作者：		韩昌云、刘嘉伟
/***************************************************/
void getScreenShot(char *file, int sec)
{
	gint x, y;
	GdkScreen *cur_screen = NULL;
	GdkWindow *window = NULL;
	GdkPixbuf *pixbuf_screenshot = NULL;
	GdkRectangle rect;
	GdkRectangle screen_rect;
	g_printf("ready to capture.....\r\n");
	sleep(sec);
	if (cur_screen == NULL)
		cur_screen = gdk_screen_get_default(); //get screen
	screen_rect.x = 0;
	screen_rect.y = 0;
	screen_rect.width = gdk_screen_get_width(cur_screen);   //get screen width
	screen_rect.height = gdk_screen_get_height(cur_screen); //get screen height
	g_print("screen_rect: x=%d,y=%d,w=%d,h=%d\n", screen_rect.x, screen_rect.y, screen_rect.width, screen_rect.height);
	window = gdk_screen_get_root_window(cur_screen); //get window by screen
	//Window xwin = GDK_WINDOW_XID(window);  //get window id
	//g_print("xwindow id is %x\n",xwin);
	gdk_window_get_origin(window, &x, &y); //get origin point
	rect.x = x;
	rect.y = y;
	gdk_drawable_get_size(GDK_DRAWABLE(window), &rect.width, &rect.height); //get drawable size
	g_print("rect: x=%d,y=%d,w=%d,h=%d\n", rect.x, rect.y, rect.width, rect.height);
	if (!gdk_rectangle_intersect(&rect, &screen_rect, &rect))
	{ //obtain the intersect area from rect and screen_rect
		g_print("Capture failed!...\r\n");
		return;
	}
	g_print("capture rect: x=%d,y=%d,w=%d,h=%d\n", rect.x, rect.y, rect.width, rect.height);
	pixbuf_screenshot = gdk_pixbuf_get_from_drawable(NULL, window,
													 NULL, rect.x - x, rect.y - y, 0, 0,
													 rect.width, rect.height);		//get pixbuf from drawable widget
	gdk_pixbuf_save(pixbuf_screenshot, file, "jpeg", NULL, "quality", "100", NULL); //save pixbuf to jpeg format file
	g_object_unref(pixbuf_screenshot);												//free pixbuf
	g_printf("Capture saved!!!\r\n");
	return;
}

/*获取时间戳函数，网上找的 = = */
/*周育聪ZYC  = = */
void getTimeStr(char *file)
{
	int cmd, sec;
	time_t tt;
	struct tm *tm_ptr = NULL;
	//file = (char *)malloc(32);
	time(&tt);
	tm_ptr = localtime(&tt);
	strftime(file, 32, "%Y%m%d%H%M%S", tm_ptr);
}

void cb_screenShot(GtkWidget *button, gpointer data)
{ //回调函数
	char file[50] = "./Local/Screenshot/";
	getTimeStr(file + strlen(file));
	strcat(file, ".jpg");
	g_printf("%s\n", file);
	getScreenShot(file, 3);
}
