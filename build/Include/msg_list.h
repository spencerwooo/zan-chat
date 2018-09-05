#ifndef __MSG_LIST_H
#define __MSG_LIST_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

/**************************************************/
/*名称：		msg_list.h
/*描述：		定义消息列表、消息交互相关函数及数据结构
/***************************************************/

#define LEN sizeof(struct Msg)
#define SIZE_1 20
#define SIZE_2 100

struct Msg
{
    char  ip[SIZE_1];
    char  name[SIZE_1];
    char  time[SIZE_1];
    char  message[SIZE_2];
    struct Msg * next;
};

void getMsg(char * ip,char arr[]);

void addMsg(char *ip,char *name ,char *message);

struct Msg * deleteMsg(struct Msg * p);

void getChatTime(char buf[]);

#endif

