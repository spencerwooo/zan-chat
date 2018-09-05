#include "../../Include/picConverter.h"
/**************************************************/
/*名称：picConverter.h
/*描述：定义图片-组件转换函数
/*作成日期： 2017-08-31
/*作者： 唐回峰
/***************************************************/

GdkPixbuf *filenameToPixbufConverter(gchar *filename)
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

/**************************************************/
/*名称：picToButtonConverter
/*描述：将指定路径的图片转换成button控件
/*作成日期： 2017-08-31
/*参数：
/*      参数1：gchar*、filename、图片路径
/*返回值：GtkWidget*、button控件指针
/*作者： 唐回峰
/***************************************************/
GtkWidget *picToButtonConverter(gchar *filename)
{
	GtkWidget *image;
	GtkWidget *button;

	button = gtk_button_new();
	image = gtk_image_new_from_file(filename);
	gtk_container_add(GTK_CONTAINER(button), image);
	gtk_container_set_border_width(GTK_CONTAINER(button), 0);
	return button;
}
