#include "../../Include/chatRecord.h"

/**************************************************/
/*名称：chatRecord
/*描述：聊天文本交互相关函数
/*作成日期： 2018-09-03
/*作者： 李文煜、万伯阳
/***************************************************/

struct chatRecord *createRecordNode(char *username, char *date, char *ip, char *record)
{
	struct chatRecord *p;
	p = (struct chatRecord *)malloc(sizeof(struct chatRecord));
	strcpy(p->username, username);
	strcpy(p->date, date);
	strcpy(p->record, record);
	p->next = NULL;
	return p;
}

/**************************************************/
/*名称：saveRecord
/*描述：将一条消息保存至文件，以ip、时间分类
/*作成日期： 2018-09-02
/*参数：
/*      参数1：struct chatRecord *、head、记录聊天信息的头结点
/*		参数2：char *、ip、对方ip地址
/*		参数3：char *、date、聊天日期
/*返回值：void
/*作者： 邵子涵、程孟祺
/***************************************************/
void saveRecord(struct chatRecord *head, char *ip, char *date)
{
	FILE *fp;
	char fileName[SIZE];
	char record[RECORD_LEN];
	struct chatRecord *p = head;

	strcpy(fileName, ip);
	strcat(fileName, "_");
	strcat(fileName, date);
	strcat(fileName, ".txt");
	char path[80] = {0};
	getcwd(path, 80);
	strcat(path, "/ChatRecord/");
	strcat(path, fileName);

	fp = fopen(path, SAVE_MODE);
	if (NULL == fp)
	{
		return;
	}

	while (NULL != p)
	{
		nodeToRecord(p, FILE_FORMAT, record);
		//write into file
		fputs(record, fp);
		p = p->next;
	}
	fclose(fp);
	//close file
}

struct chatRecord *setOffRecord(struct chatRecord *head)
{
	struct chatRecord *tmp = head;
	while (tmp->next != NULL)
	{
		tmp = head->next;
		free(head);
		head = tmp;
	}
	free(head);
	return NULL;
}

/**************************************************/
/*名称：readRecord
/*描述：读取聊天记录
/*作成日期： 2018-09-02
/*参数：
/*		参数1：char *、ip、对方ip地址
/*		参数2：char *、date、聊天日期
/*返回值：struct chatRecord *、消息文本链表头结点
/*作者： 邵子涵、韩昌云
/***************************************************/
struct chatRecord *readRecord(char *ip, char *date)
{
	FILE *fp;
	char fileName[SIZE];
	struct chatRecord *head = NULL;
	char line[ONE_RECORD_SIZE];
	struct chatRecord *p = head;
	strcpy(fileName, ip);
	strcat(fileName, "_");
	strcat(fileName, date);
	strcat(fileName, ".txt");

	char path[80] = {0};
	getcwd(path, 80);
	strcat(path, "/ChatRecord/");
	strcat(path, fileName);

	fp = fopen(path, READ_MODE);
	if (NULL == fp)
	{
		return head;
	}
	while (NULL != fgets(line, ONE_RECORD_SIZE, fp))
	{
		p = recordParser(line);
		head = addRecordNode(head, p);
	}
	fclose(fp);
	return head;
}

void nodeToRecord(struct chatRecord *node, int type, char *string)
{
	//	char  string[200];
	switch (type)
	{
	case FILE_FORMAT:
		strcpy(string, node->username);
		strcat(string, "  ");
		strcat(string, "(");
		strcat(string, node->date);
		strcat(string, ")");
		strcat(string, "    ");
		strcat(string, node->record);
		strcat(string, "\n");
		break;
	case VIEW_FORMAT:
		strcpy(string, node->username);
		strcat(string, "  ");
		strcat(string, "(");
		strcat(string, node->date);
		strcat(string, ")");
		strcat(string, ":\n");
		strcat(string, node->record);
		break;
	}
	//	return string;
}

struct chatRecord *addRecordNode(struct chatRecord *head, struct chatRecord *node)
{
	struct chatRecord *p = head;
	//struct chatRecord *p1 = p;
	if (NULL == head)
	{
		head = node;
		node->next = NULL;
		return head;
	}
	//尾部插入法
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = node;
	return head;
}

/**************************************************/
/*名称：record_display
/*描述：显示一条消息记录
/*作成日期： 2018-09-03
/*参数：
/*      参数1：struct chatRecord *、head、记录聊天信息的头结点
/*返回值：void
/*作者： 韩昌云、程孟祺
/***************************************************/
void record_display(struct chatRecord *head)
{
	struct chatRecord *p = head;
	char record[RECORD_LEN];
	while (NULL != p)
	{
		nodeToRecord(p, FILE_FORMAT, record);
		p = p->next;
	}
}

struct chatRecord *recordParser(char *record)
{
	char *username;
	char *date;
	char *info;
	//解析数据，并没有出去字符串两端的空格符
	username = strtok(record, "()");
	date = strtok(NULL, "()");
	info = strtok(NULL, "()");

	//创建节点
	return createRecordNode(username, date, "", info);
}
