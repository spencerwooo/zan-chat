#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <gtk/gtk.h>
#include <errno.h>

#include "chatWindow.h"

/**************************************************/
/*名称：		message.h
/*描述：		信息的传输、解析、交互等功能实现
/***************************************************/

#define SND_BUF_LEN  1024

typedef struct _msg_data{
    int magic;
    int version;
    int pkt_seq_no;	    //包的序列号
    int command;
    int command_opts;
    int image_code;	    //形象代码
    char *username;	    //用户名
    char *hostname;	    //主机名称
    char *extstring;	//群组名称
  
    struct timeval tv;
    char *message;
} msg_data_t;

typedef struct _user_info{
	char *u_name;
	char *u_group;
	int u_pic_id;
	char *u_qianming;
	int u_img_code;     //形象代码
	int u_avatar_code;
} u_info_t;

typedef struct argu_pthread{
	char buf[1024];
	int len;
	struct sockaddr_in client;//本地网络信息
}argu_pthread_t;


int parseMessage(msg_data_t *msg, char *msg_str, size_t len);

int parseUserInfo(u_info_t *u_info, char *str, size_t len);

int sendMsg(int mode, char *msg, struct sockaddr_in *p, int fd);

void *chat_start();

void setUserString();

int *handle(argu_pthread_t * argu);

void sendToTextView1(GtkWidget *button, textView *textViewAll);

#endif
