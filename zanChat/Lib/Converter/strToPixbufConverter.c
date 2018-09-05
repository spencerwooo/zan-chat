#include "../../Include/strToPixbufConverter.h"

/**************************************************/
/*名称：strToPixbufConverter
/*描述：将文件转换weiGdkPixbuf
/*作成日期： 2017-08-31
/*参数：
/*      参数1：gchar*、filename、文件路径
/*返回值：GdkPixbuf*
/*作者： 唐回峰
/***************************************************/
GdkPixbuf* strToPixbufConverter(gchar *filename) {
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	if(!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	return pixbuf;
}
