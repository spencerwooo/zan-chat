#include "../../Include/message.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <time.h>
#include <gdk/gdkkeysyms.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include "../../Include/linpop.h"

/********************************************/
/*名称：sendMsg.c
/*描述：定义发送函数及相关功能函数
/*作者：唐回峰
/*日期：2017-09-06
/********************************************/

extern int s;
void getSystemTime(char buf[])
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf, 30, "%H:%M:%S", p);
	//printf("%s\n",buf);
}

/**************************************************/
/*名称:sendToTextView1
/*描述：发送到文本框
/*作成日期：
/*参数：
/*		参数1：GtkWidget *、button
/*		参数2：textView *、textViewAll、显示文本框
/*返回值：VOID
/*作者：	唐回峰
/***************************************************/
void sendToTextView1(GtkWidget *button, textView *textViewAll)
{
	GtkTextIter start, end;

	char *text;
	char *ip;
	char *name;
	ip = textViewAll->ip;
	gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(textViewAll->buffer2), &start, &end);
	text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textViewAll->buffer2), &start, &end, FALSE);

	if (strcmp(textViewAll->message, "\0") != 0)
	{
		printf("strcmp!\n");
		strcat(text, "#");
		strcat(text, textViewAll->message);
	}

	textViewAll->message = "\0";
	//send message start
	struct sockaddr_in addr;
	bzero(&addr, sizeof(struct sockaddr_in));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(LINPOP_PORT);
	inet_pton(AF_INET, ip, &addr.sin_addr.s_addr);
	if (sendMsg(LINPOP_SENDMSG, text, (struct sockaddr *)&addr, s) != 0)
		die("send error!");
	//send message end

	// showMessage(text, textViewAll);
	showMessageFromLocal(text, textViewAll);
	gtk_text_buffer_delete(GTK_TEXT_BUFFER(textViewAll->buffer2), &start, &end);
}
