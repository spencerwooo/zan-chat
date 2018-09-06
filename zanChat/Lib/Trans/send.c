#include "../../Include/message.h"
#include "../../Include/models.h"
#include "../../Include/linpop.h"
#include "../../Include/chatWindow.h"
#include "../../Include/userInfo.h"
#include "../../Include/fileTrans.h"

/********************************************/
/*名称：send.c
/*描述：定义发送信息的函数及socket相关功能
/*作者：邵子涵、程孟祺
/*日期：2018-09-05
/********************************************/

extern int my_img_code;
extern int my_avatar_code;
extern char user_name[100];
extern char user_group[100];
extern char sigh[100];
extern char file_send_path[80];
extern int s;
file_pthread_t file_argu;

/**************************************************/
/*名称：		sendMsg
/*描述：		创建进程，向对方ip发送信息
/*作成日期：	2018-09-05
/*参数：		
/*		参数1：int、mode、发送时的选项，如回复等
/*		参数2：char *、msg、信息
/*		参数3：struct sockaddr_in *、p、新进程
/*		参数4：int、fd、socket套接字
/*返回值：	int
/*作者：		邵子涵、程孟祺
/***************************************************/
int sendMsg(int mode, char *msg, struct sockaddr_in *p, int fd)
{
	int udp_fd = fd;
	int broadcast_en = 1;
	char msg_buf[SND_BUF_LEN];
	char *use = user_name;	//发送者姓名 (可由全局变量指明)
	char *group = user_group; //发送者组名(可由全局变量指明)
	socklen_t broadcast_len = sizeof(broadcast_en);
	long int msg_id = time((time_t *)NULL); // 填充包编号
	pthread_t file_thread;					//文件传输线程标识符

	struct sockaddr_in udp_addr;
	struct sockaddr_in client;
	//清空数据缓冲区和网络信息
	bzero(msg_buf, SND_BUF_LEN);
	bzero(&udp_addr, sizeof(struct sockaddr_in));
	udp_addr.sin_family = AF_INET;
	udp_addr.sin_port = htons(LINPOP_PORT);
	inet_pton(AF_INET, "255.255.255.255", &udp_addr.sin_addr.s_addr);
	//对于上线通告 下线等使用广播地址,其他的则不用广播
	if ((p == NULL) && (mode != LINPOP_BR_ABSENCE) && (mode != LINPOP_BR_EXIT) && (mode != LINPOP_NOOPERATION) && (mode != LINPOP_BR_ENTRY))
	{

		die("p isNULL,only mode LINPOP_BR_EXIT  LINPOP_NOOPERATION LINPOP_NOOPERATION LINPOP_BR_ABSENCE is allowed p=NULL\n");
	}
	else if ((p != NULL) && (mode != LINPOP_BR_ABSENCE) && (mode != LINPOP_BR_EXIT) && (mode != LINPOP_NOOPERATION) && (mode != LINPOP_BR_ENTRY))
	{

		client = *p;
	}
	//打开广播
	if (setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &broadcast_en, broadcast_len) < 0)
		//(udp_fd,SOL_SOCKET,SO_REUSEADDR,&broadcast_en,broadcast_len)<0 )
		die("setsockopt error");
	switch (mode)
	{
	case LINPOP_NOOPERATION: //空操作
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, NULL);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr));
		break;
	case LINPOP_BR_ENTRY: //用户上线发送的消息
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr));
		break;

	case LINPOP_BR_ABSENCE: //更改信息后发送的信息
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr));
		break;

	case LINPOP_BR_EXIT: //退出时发送的消息
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&udp_addr, sizeof(struct sockaddr));
		break;
	case LINPOP_ANSENTRY: //确认在线，即应答在线
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&client, sizeof(struct sockaddr));
		break;
	case LINPOP_SENDMSG: //发送消息
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&client, sizeof(struct sockaddr));
		//将本人发的消息加入消息记录
		if (msg != NULL)
		{
			char myTime[30];
			char recordName[20];
			char recordMessage[200];
			struct chatRecord *node = NULL;
			strcpy(recordName, user_name);
			strcpy(recordMessage, msg);
			getCurrentTime(myTime, 30);
			printf("-------------------%s------------%s\n", recordName, recordMessage);
			//创建新的记录节点
			node = createRecordNode(recordName, myTime, NULL, recordMessage);
			struct userinfo *u_info;
			char *ip = inet_ntoa(client.sin_addr);
			u_info = seekUser(head, ip);
		}

		break;

	case LINPOP_RECVMSG: //确认收到信息
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&client, sizeof(struct sockaddr));
		break;
	case LINPOP_GETFILEDATA:
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&client, sizeof(struct sockaddr));
		break;
	case LINPOP_RELEASEFILES:
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);
		sendto(udp_fd, msg_buf, strlen(msg_buf), 0, (struct sockaddr *)&client, sizeof(struct sockaddr));
		break;
	case LINPOP_SENDFILEREQUEST:
		sprintf(msg_buf, "1:%d:%s:%s:%d:%s:", msg_id, use, group, mode, msg);

		//创建文件传输线程

		break;
	default:
		printf("no match mode !\n");
		break;
	}

	broadcast_en = 0;
	if (setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &broadcast_en, broadcast_len) < 0)
		die("setsockopt error");

	return 0;
}
