#include "../../Include/msg_list.h"

/**************************************************/
/*名称：addMsg
/*描述：消息交互中的显示等函数，处理消息
/*作成日期： 2018-09-05
/*作者： 韩昌云、刘嘉伟、程孟祺
/***************************************************/

extern struct Msg *Msghead;
void addMsg(char *ip, char *name, char *message)
{
	struct Msg *p, *q;
	p = (struct Msg *)malloc(LEN);
	strcpy(p->ip, ip);
	strcpy(p->name, name);
	strcpy(p->message, message);
	p->next = NULL;
	//	getCurrentTime(p->time,sizeof(p->time));
	getChatTime(p->time);
	//如果用户信息为NULL，则中止插入操作，返回原来的头指针{
	if (NULL == Msghead)
	{
		Msghead = p;
		return;
	}

	q = Msghead;
	while (NULL != q->next)
		q = q->next;
	//尾部插入
	q->next = p;
}

/****************************************************/
//删除一个用户
/***************************************************/
struct Msg *deleteMsg(struct Msg *p)
{
	struct Msg *q;
	if (Msghead == p && NULL == Msghead->next)
	{
		Msghead = NULL;
		free(p);
		return NULL;
	}
	else if (Msghead == p && NULL != Msghead->next)
	{
		Msghead = p->next;
		free(p);
		return Msghead;
	}
	else
	{
		q = Msghead;
		while (q->next != p)
			q = q->next;
		q->next = p->next;
		free(p);
		return q->next;
	}
}

/****************************************************/
//得到msg
/***************************************************/
//char* getMsg(char * ip,char arr[])
void getMsg(char *ip, char arr[])
{
	//char arr[1000];
	struct Msg *p;
	p = Msghead;
	//找到IP为ip的用户节点，如果当前节点指针！＝NULL，且ip不同
	while (NULL != p)
	{
		printf("get Msg ---\n");
		if (strcmp(p->ip, ip) == 0)
		{
			printf("get Msg if  ---\n");
			strcat(arr, p->name);
			strcat(arr, " ");
			strcat(arr, p->time);
			strcat(arr, ": \n");
			strcat(arr, p->message);
			strcat(arr, "\n");
			p = deleteMsg(p);
		}
		else
			p = p->next;
	}
	//	return arr;
}

void getChatTime(char buf[])
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf, 30, "%H:%M:%S", p);
	//printf("%s\n",buf);
}
