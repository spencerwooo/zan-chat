#include "../../Include/dateConverter.h"
/**************************************************/
/*名称：		dateConverter.c
/*描述：		日期转换功能实现
/*作成日期： 2018-08-30
/*作者：		武上博、阿琪
/***************************************************/

/**************************************************/
/*名称：		gtkdateToDateConverter
/*描述：		将参数1表示的时间转换为字符串格式
/*作成日期： 2018-08-30
/*参数：
/*		参数1：calendar、GtkWidget* 、GTK内置日历变量，用来表示时间
/*返回值：ret、gchar*、字符型表示时间
/*
/*作者：		武上博
/***************************************************/
gchar *gtkdateToDateConverter(GtkWidget *calendar)
{
    gchar *ret;
    ret = (gchar *)malloc(10 * sizeof(char));
    memset(ret, 0, sizeof(ret));
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    month++;

    ret[0] = year / 1000 + '0';
    ret[1] = (year % 1000) / 100 + '0';
    ret[2] = (year % 100) / 10 + '0';
    ret[3] = year % 10 + '0';

    ret[4] = month / 10 + '0';
    ret[5] = month % 10 + '0';

    ret[6] = day / 10 + '0';
    ret[7] = day % 10 + '0';

    ret[8] = 0;

    return ret;
}
