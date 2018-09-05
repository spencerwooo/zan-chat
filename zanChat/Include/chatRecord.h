#ifndef __CHAT_RECORD_H
#define __CHAT_RECORD_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**************************************************/
/*名称：		chatRecord.h
/*描述：		聊天信息记录相关函数，如生成节点、储存、读取记录
/***************************************************/

#define SIZE 50
#define RECORD_LEN 200
#define FILE_FORMAT 0
#define VIEW_FORMAT 1
#define SAVE_MODE "a+"
#define READ_MODE "r"
#define ONE_RECORD_SIZE 4096
#define STRUCT_ RECORD_LEN sizeof(struct chatRecord)

struct chatRecord
{
	char username[SIZE];
	char date[SIZE];
	char ip[SIZE];
	char record[RECORD_LEN];
	struct chatRecord *next;
};

struct chatRecord *createRecordNode(char *username, char *date, char *ip, char *record);

struct chatRecord *createRecordNodeTest();

void saveRecord(struct chatRecord *head, char *ip, char *date);

struct chatRecord *readRecord(char *ip, char *date);

void nodeToRecord(struct chatRecord *node, int type, char *record);

struct chatRecord *addRecordNode(struct chatRecord *head, struct chatRecord *node);

void record_display(struct chatRecord *head);

struct chatRecord *recordParser(char *record);

struct chatRecord *setOffRecord(struct chatRecord *head);

#endif
