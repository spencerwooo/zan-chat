
#include "./../../Include/calendar.h"

/**************************************************/
/*名称：calendar.c
/*描述：创建日历构件
/*作成日期： 2017-09-02
/*作者：周育聪
/***************************************************/
/*GtkCalendar 日历构件
*/
void calendar_date_to_string(CalendarData *data,
                             char *buffer,
                             gint buff_len)
{
  struct tm tm;
  time_t time;

  memset(&tm, 0, sizeof(tm));
  gtk_calendar_get_date(GTK_CALENDAR(data->window),
                        &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
  tm.tm_year -= TM_YEAR_BASE;
  time = mktime(&tm);
  strftime(buffer, buff_len - 1, "%x", gmtime(&time));
}

void calendar_set_signal_strings(char *sig_str,
                                 CalendarData *data)
{
  const gchar *prev_sig;

  prev_sig = gtk_label_get_text(GTK_LABEL(data->prev_sig));
  gtk_label_set_text(GTK_LABEL(data->prev2_sig), prev_sig);

  prev_sig = gtk_label_get_text(GTK_LABEL(data->last_sig));
  gtk_label_set_text(GTK_LABEL(data->prev_sig), prev_sig);
  gtk_label_set_text(GTK_LABEL(data->last_sig), sig_str);
}

void calendar_month_changed(GtkWidget *widget,
                            CalendarData *data)
{
  char buffer[256] = "month_changed: ";

  calendar_date_to_string(data, buffer + 15, 256 - 15);
  calendar_set_signal_strings(buffer, data);
}

void calendar_day_selected(GtkWidget *widget,
                           CalendarData *data)
{
  char buffer[256] = "day_selected: ";

  calendar_date_to_string(data, buffer + 14, 256 - 14);
  calendar_set_signal_strings(buffer, data);
}

void calendar_day_selected_double_click(GtkWidget *widget,
                                        CalendarData *data)
{
  struct tm tm;
  char buffer[256] = "day_selected_double_click: ";

  calendar_date_to_string(data, buffer + 27, 256 - 27);
  calendar_set_signal_strings(buffer, data);

  memset(&tm, 0, sizeof(tm));
  gtk_calendar_get_date(GTK_CALENDAR(data->window),
                        &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
  tm.tm_year -= TM_YEAR_BASE;

  if (GTK_CALENDAR(data->window)->marked_date[tm.tm_mday - 1] == 0)
  {
    gtk_calendar_mark_day(GTK_CALENDAR(data->window), tm.tm_mday);
  }
  else
  {
    gtk_calendar_unmark_day(GTK_CALENDAR(data->window), tm.tm_mday);
  }
}

void calendar_prev_month(GtkWidget *widget,
                         CalendarData *data)
{
  char buffer[256] = "prev_month: ";

  calendar_date_to_string(data, buffer + 12, 256 - 12);
  calendar_set_signal_strings(buffer, data);
}

void calendar_next_month(GtkWidget *widget,
                         CalendarData *data)
{
  char buffer[256] = "next_month: ";

  calendar_date_to_string(data, buffer + 12, 256 - 12);
  calendar_set_signal_strings(buffer, data);
}

void calendar_prev_year(GtkWidget *widget,
                        CalendarData *data)
{
  char buffer[256] = "prev_year: ";

  calendar_date_to_string(data, buffer + 11, 256 - 11);
  calendar_set_signal_strings(buffer, data);
}

void calendar_next_year(GtkWidget *widget,
                        CalendarData *data)
{
  char buffer[256] = "next_year: ";

  calendar_date_to_string(data, buffer + 11, 256 - 11);
  calendar_set_signal_strings(buffer, data);
}

void calendar_set_flags(CalendarData *calendar)
{
  gint i;
  gint options = 0;
  for (i = 0; i < 5; i++)
    if (calendar->settings[i])
    {
      options = options + (1 << i);
    }
  if (calendar->window)
    gtk_calendar_display_options(GTK_CALENDAR(calendar->window), options);
}

void calendar_toggle_flag(GtkWidget *toggle,
                          CalendarData *calendar)
{
  gint i;
  gint j;
  j = 0;
  for (i = 0; i < 5; i++)
    if (calendar->flag_checkboxes[i] == toggle)
      j = i;

  calendar->settings[j] = !calendar->settings[j];
  calendar_set_flags(calendar);
}

/*绘制日历构件，并且添加回调函数
*/

void create_calendar()
{
  GtkWidget *window;
  GtkWidget *vbox, *vbox2, *vbox3;
  GtkWidget *hbox;
  GtkWidget *hbbox;
  GtkWidget *calendar;
  GtkWidget *toggle;
  GtkWidget *button;
  GtkWidget *frame;
  GtkWidget *separator;
  GtkWidget *label;
  GtkWidget *bbox;
  static CalendarData calendar_data;
  gint i;

  struct
  {
    char *label;
  } flags[] =
      {
          {"Show Heading"},
          {"Show Day Names"},
          {"No Month Change"},
          {"Show Week Numbers"},
          {"Week Start Monday"}};

  calendar_data.window = NULL;
  calendar_data.font = NULL;
  calendar_data.font_dialog = NULL;

  for (i = 0; i < 5; i++)
  {
    calendar_data.settings[i] = 0;
  }

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  // gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_widget_destroyed),
                   NULL);

  gtk_window_set_title(GTK_WINDOW(window), "Calendar - zanChat");
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);

  g_signal_connect(G_OBJECT(window), "delete-event",
                   G_CALLBACK(gtk_false),
                   NULL);

  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

  vbox = gtk_vbox_new(FALSE, DEF_PAD);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  /*
   * 顶级窗口，其中包含日历构件，设置日历各参数的复选按钮和设置字体的按钮
   */

  hbox = gtk_hbox_new(FALSE, DEF_PAD);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, DEF_PAD);
  hbbox = gtk_hbutton_box_new();
  gtk_box_pack_start(GTK_BOX(hbox), hbbox, FALSE, FALSE, DEF_PAD);
  gtk_button_box_set_layout(GTK_BUTTON_BOX(hbbox), GTK_BUTTONBOX_SPREAD);
  gtk_box_set_spacing(GTK_BOX(hbbox), 5);

  frame = gtk_frame_new("Calendar");
  gtk_box_pack_start(GTK_BOX(hbbox), frame, FALSE, TRUE, DEF_PAD);
  calendar = gtk_calendar_new();
  calendar_data.window = calendar;
  calendar_set_flags(&calendar_data);
  gtk_calendar_mark_day(GTK_CALENDAR(calendar), 19);
  gtk_container_add(GTK_CONTAINER(frame), calendar);
  g_signal_connect(G_OBJECT(calendar), "month_changed",
                   G_CALLBACK(calendar_month_changed),
                   &calendar_data);
  g_signal_connect(G_OBJECT(calendar), "day_selected",
                   G_CALLBACK(calendar_day_selected),
                   &calendar_data);
  g_signal_connect(G_OBJECT(calendar), "day_selected_double_click",
                   G_CALLBACK(calendar_day_selected_double_click),
                   &calendar_data);
  g_signal_connect(G_OBJECT(calendar), "prev_month",
                   G_CALLBACK(calendar_prev_month),
                   &calendar_data);
  g_signal_connect(G_OBJECT(calendar), "next_month",
                   G_CALLBACK(calendar_next_month),
                   &calendar_data);
  g_signal_connect(G_OBJECT(calendar), "prev_year",
                   G_CALLBACK(calendar_prev_year),
                   &calendar_data);
  g_signal_connect(G_OBJECT(calendar), "next_year",
                   G_CALLBACK(calendar_next_year),
                   &calendar_data);

  separator = gtk_vseparator_new();
  gtk_box_pack_start(GTK_BOX(hbox), separator, FALSE, TRUE, 0);

  vbox2 = gtk_vbox_new(FALSE, DEF_PAD);
  gtk_box_pack_start(GTK_BOX(hbox), vbox2, FALSE, FALSE, DEF_PAD);

  /* 创建一个框架，放入设置各种参数的复选按钮 */

  frame = gtk_frame_new("Flags");
  gtk_box_pack_start(GTK_BOX(vbox2), frame, TRUE, TRUE, DEF_PAD);
  vbox3 = gtk_vbox_new(TRUE, DEF_PAD_SMALL);
  gtk_container_add(GTK_CONTAINER(frame), vbox3);

  for (i = 0; i < 5; i++)
  {
    toggle = gtk_check_button_new_with_label(flags[i].label);
    g_signal_connect(G_OBJECT(toggle),
                     "toggled",
                     G_CALLBACK(calendar_toggle_flag),
                     &calendar_data);
    gtk_box_pack_start(GTK_BOX(vbox3), toggle, TRUE, TRUE, 0);
    calendar_data.flag_checkboxes[i] = toggle;
  }

  bbox = gtk_hbutton_box_new();
  gtk_box_pack_start(GTK_BOX(vbox), bbox, FALSE, FALSE, 0);
  gtk_button_box_set_layout(GTK_BUTTON_BOX(bbox), GTK_BUTTONBOX_END);

  button = gtk_button_new_with_label("Close");

  gtk_signal_connect_object(GTK_OBJECT(button),
                            "clicked",
                            GTK_SIGNAL_FUNC(gtk_widget_destroy),
                            GTK_OBJECT(window));

  gtk_container_add(GTK_CONTAINER(bbox), button);
  GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
  gtk_widget_grab_default(button);

  gtk_widget_show_all(window);
}
void calendar(GtkWidget *button, gpointer data)
{
  create_calendar();
}
