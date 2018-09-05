#ifndef PICCONVERTER_H
#define PICCONVERTER_H

#include <gtk/gtk.h>

/**************************************************/
/*名称：		picConverter.h
/*描述：		将图片文件转换为gtk控件
/***************************************************/

GdkPixbuf *filenameToPixbufConverter(gchar *filename);

GtkWidget *picToButtonConverter(gchar *filename);

#endif
