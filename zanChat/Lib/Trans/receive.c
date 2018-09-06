#include "../../Include/message.h"
#include "../../Include/models.h"
#include "../../Include/linpop.h"
#include "../../Include/chatWindow.h"
#include "../../Include/chatRecord.h"
#include "../../Include/userInfo.h"
#include "../../Include/fileTrans.h"
/********************************************/
/*名称：receive.c
/*描述：定义接受信息的函数及socket相关功能
/*作者：韩昌云
/*日期：2018-09-05
/********************************************/

#define STR_SIZE 512

extern struct userinfo *head;
extern GtkStatusIcon *trayIcon;
//has been declared in global.h

extern int my_img_code;
extern int my_avatar_code;
extern char user_name[100];
extern char user_group[100];
extern char sigh[100];
extern int s;
char img_code_str[255];
char avatar_code_str[255];
char userString[STR_SIZE];

char temp[255];

/**************************************************/
/*名称：		die
/*描述：		抛出异常
/*作成日期：	2018-09-03
/*参数：		
/*		参数1：char *、str、异常信息
/*返回值：	VOID
/*作者：		韩昌云
/***************************************************/
void die(char *str)
{
	perror(str);
	exit(0);
}

/**************************************************/
/*名称：		chat_start
/*描述：		创建会话，定义初始化socket套接字，与聊天对方receive函数链接
/*作成日期：	2018-09-04
/*参数：		void
/*返回值：	void
/*作者：		韩昌云
/***************************************************/
void *chat_start()
{
	int i;
	char buf[1024];
	int len = 0;
	//setUserString();
	struct sockaddr_in addr;
	struct sockaddr_in client; //本地网络信息
	argu_pthread_t argu;

	int addrLen = sizeof(struct sockaddr_in);
	//sleep(2);

	printf("user name %s\n", user_name);
	if (sendMsg(LINPOP_BR_ENTRY, user_name, NULL, s) != 0)
		die("sendMsg error!");

	while (1)
	{
		len = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&client, &addrLen);
		printf("received: %s \n", buf);

		memcpy(argu.buf, buf, 1024);
		argu.client = (struct sockaddr_in)client;
		argu.len = len;

		pthread_t operator_thread;
		if (pthread_create(&operator_thread, NULL, (int *)handle, &argu) != 0)
			die("pthread_create operator failed");
	}
}
void setUserString()
{
	memset(userString, 0, STR_SIZE);
	strcat(userString, "\0");
	int img_code = my_img_code;
	int avatar_code = my_avatar_code;
	char img[5] = {0};
	int2str(my_img_code, img);
	char avatar[5] = {0};
	int2str(my_avatar_code, avatar);
	strcat(userString, user_name);
	strcat(userString, "_");
	strcat(userString, user_group);
	strcat(userString, "_");
	//pic id 3
	char pic_id[5] = {0};
	int2str(3, pic_id);
	strcat(userString, pic_id);
	strcat(userString, "_");
	strcat(userString, sigh);
	strcat(userString, "_");
	strcat(userString, img);
	strcat(userString, "_");
	strcat(userString, avatar);
	strcat(userString, "_");
	strcat(userString, "\0");
	//return userString;
}

/**************************************************/
/*名称：		handle
/*描述：		多线程，传入句柄构建会话receive线程
/*作成日期：	2018-09-05
/*参数：		
/*		参数1：argu_pthread_t*、argu、会话句柄参数
/*返回值：	VOID
/*作者：		韩昌云
/***************************************************/
int *handle(argu_pthread_t *argu)
{
	msg_data_t msg;
	u_info_t u_info;
	struct userinfo *user;
	char buf[1024];
	struct sockaddr_in client;
	int len = 0;

	bzero(buf, 1024);
	bzero(&client, sizeof(struct sockaddr_in));

	client = argu->client;
	memcpy(buf, argu->buf, 1024);
	len = argu->len;

	if (parseMessage(&msg, buf, len) != 0)
		die("parseMessage");

	switch (msg.command)
	{
	case LINPOP_NOOPERATION: //不进行任何操作
		break;
	case LINPOP_BR_ENTRY: //用户上线
		setUserString();
		//if(sendMsg(LINPOP_ANSENTRY,"linpop_linpop_2_i love you",&client,s) != 0)
		if (sendMsg(LINPOP_ANSENTRY, userString, &client, s) != 0)
			die("sendMsg error!");
		break;
	case LINPOP_BR_ABSENCE: //修改信息
		break;
	case LINPOP_BR_EXIT: //用户退出
		break;
	case LINPOP_ANSENTRY: //应答用户在线
		if (msg.command_opts == LINPOP_NOOPERATION)
		{
			memcpy(buf, msg.message, 1024);
			if (parseUserInfo(&u_info, buf, strlen(buf)) != 0)
				die("parseUserInfo");
			user = createOneUser(u_info.u_name, u_info.u_group, inet_ntoa(client.sin_addr), u_info.u_qianming, u_info.u_img_code, u_info.u_avatar_code);
		}
		else
		{
			// break;
			user = createOneUser(msg.username, "我的好友", inet_ntoa(client.sin_addr), msg.message, 1, 1);
			//head=addUser(head,&user);
		}
		head = addUser(head, user);
		display(head);
		lin_refresh();
		break;
	case LINPOP_SENDMSG: //发送消息

		if (sendMsg(LINPOP_RECVMSG, " ", &client, s) != 0)
			die("sendMsg error!");
		struct userinfo *p;
		char *ip = inet_ntoa(client.sin_addr);
		p = seekUser(head, ip);

		if (p != NULL && p->textViewAll == NULL) //创建新的窗体
		{
			gtk_status_icon_set_blinking(trayIcon, TRUE);
			addMsg(ip, msg.username, msg.message);
		}
		else
		{
			if (p != NULL && p->textViewAll != NULL) //已经有了窗体
			{
				showMessage(msg.message, p->textViewAll);
			}
		}

		char myTime[30];
		char recordName[20];
		char recordMessage[200];
		struct chatRecord *node = NULL;
		strcpy(recordName, msg.username);
		strcpy(recordMessage, msg.message);
		getCurrentTime(myTime, 30);
		node = createRecordNode(recordName, myTime, NULL, recordMessage);

		p->record = addRecordNode(p->record, node);

		break;
	case LINPOP_RECVMSG: //通报收到消息

		if (sendMsg(LINPOP_NOOPERATION, "i have received you message !", &client, s) != 0)
			die("sendMsg error!");
		break;
	case LINPOP_SENDFILEREQUEST: //请求发送文件
		printf("*************************message %s\n", msg.message);
		file_recv(client, msg.message);
		break;

	default:
		break;
	}
	return 0;
}
