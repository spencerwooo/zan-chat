#ifndef _CHATWINDOW_H_
#define _CHATWINDOW_H_

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include <gdk/gdkkeysyms.h>

/**************************************************/
/*名称：		chatWindow.h
/*描述：		聊天窗口的显示、交互功能及接口
/***************************************************/

extern struct userinfo *head;

typedef struct _ip_window
{
  char *ip;
  GtkWidget *window;
} ip_window;

typedef struct _textView
{
  char *ip;
  char *name;
  char *message;

  GtkWidget *textView1;
  GtkWidget *textView2;
  GtkTextBuffer *buffer1;
  GtkTextBuffer *buffer2;
} textView;

typedef struct _fontWindow_textView
{
  GtkWidget *window;
  textView *textViewAll;
} fontWindow_textView;

typedef struct _filename_textView
{
  char *filename;
  textView *textViewAll;

} filename_textView;

void pg_ChatWindow(char *ip, char *name, char *msg);
void createChatWindow(char *ip, char *name, char *msg, char *sig, int img_code, int avatar_code);
GtkWidget *xpm_label_box(gchar *xpm_filename, gchar *label_text);
GtkWidget *label_xmp_box(gchar *label_text, gchar *xpm_filename);
void closeApp(GtkWidget *window, ip_window *ip_window_main);
void undo(GtkWidget *window, textView *textViewAll);
void fontSelect(GtkWidget *button, textView *textViewAll);
GtkWidget *createFileSelect();
void fileSelect(GtkWidget *button, char *data);
void sendToTextView1(GtkWidget *button, textView *textViewAll);
void changeFace(GtkButton *button, GtkButton *window);
void changeImage(GtkButton *button, GtkWidget *window);
void addFace(GtkWidget *button, textView *textViewAll);
// void capture(GtkWidget *button, textView *textViewAll);
void getSystemTime(char buf[]);
void showMessage(char *message, textView *textViewAll);
void chatRecordOk(GtkWidget *button, gpointer data);
#endif
