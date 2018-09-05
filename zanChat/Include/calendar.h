#ifndef CALENDAR_H
#define CALENDAR_H
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include <gdk/gdkkeysyms.h>
#include "chatWindow.h"
#define DEF_PAD 10
#define DEF_PAD_SMALL 5
#define TM_YEAR_BASE 1900

/**************************************************/
/*名称：		calendar.h
/*描述：		日历小功能相关函数实现
/***************************************************/

typedef struct _CalendarData
{
  GtkWidget *flag_checkboxes[5];
  gboolean settings[5];
  gchar *font;
  GtkWidget *font_dialog;
  GtkWidget *window;
  GtkWidget *prev2_sig;
  GtkWidget *prev_sig;
  GtkWidget *last_sig;
  GtkWidget *month;
} CalendarData;

/*函数接口*/
enum
{
  calendar_show_header,
  calendar_show_days,
  calendar_month_change,
  calendar_show_week,
  calendar_monday_first
};

void calendar(GtkWidget *button, gpointer data);

void create_calendar();
#endif
