#ifndef __USERINFO_H
#define __USERINFO_H
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <gtk/gtk.h>
#include "converter.h"
#include "chatWindow.h"

#define LEN sizeof(struct userinfo)
#define SIZE_1 20
#define SIZE_2 100

/**************************************************/
/*名称：		userinfo.h
/*描述：		定义联系人数据结构，在聊天过程中传输的联系人信息，联系人链表 *head的
			初始化、维护等操作
/***************************************************/

struct userinfo
{
    char  name[SIZE_1];
    char  group[SIZE_1];
    char  ip[SIZE_1];
    char  signature[SIZE_2];
    int picture;
    int image_code;//用户形象代码
    int avatar_code;//avatar code
    textView *textViewAll; //add by wanglong 2010-07-08
    struct chatRecord * record;
    struct userinfo * next;
};

struct userinfo *  addUser(struct userinfo * head,struct userinfo * user);

struct userinfo * deleteUser(struct userinfo * head,char * ip);

struct userinfo * seekUser(struct userinfo * head,char * ip);

struct userinfo * refreshUser(struct userinfo * head,struct userinfo * user);

void displayAll(struct userinfo * head);

void display(struct userinfo * user);

struct userinfo * createOneUser(char * name, char * group,char * ip,char * signature,int img_code, int avatar_code);

int setOnTextView(char* ip,textView * tv);

void setOffTextView(char* ip);

#endif

