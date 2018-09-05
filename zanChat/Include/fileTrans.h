#ifndef __FILETANSFER_H
#define __FILETANSFER_H

#include <sys/socket.h>

/**************************************************/
/*名称：		fileTrans.h
/*描述：		文件传输中的socket、多线程等功能接口
/***************************************************/

typedef struct file_argu
{
	char *path;
	struct sockaddr_in client; //本地网络信息
} file_pthread_t;

void *file_send(file_pthread_t *file_info);

void file_recv(struct sockaddr_in client, char *file_name);

#endif
