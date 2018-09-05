
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include<sys/types.h>  // socket 
#include<gtk/gtk.h>

#include "../../Include/fileTrans.h"
#include "../../Include/linpop.h"

/********************************************/
/*名称：filetansfer.c
/*描述：此文件定义了文件传输相关的函数
/*作者：唐回峰、周育聪
/*日期：2017-09-05
/********************************************/



#define FILE_PATH_SIZE 80
#define SERVER_PORT LINPOP_PORT 
#define BUFFER_SIZE 1024 
#define FILE_NAME_MAX_SIZE 512 
#define LENGTH_OF_LISTEN_QUEUE 20 

/**************************************************/
/*名称：file_send
/*描述：发送文件函数
/*参数：
         参数1：file_info、file_pthread_t *、输入参数、传输文件的相关信息，其中包括文件名称和接收方的地址信息
/*返回值：void
/*作者：唐回峰
/*日期：2017-09-05

/***************************************************/
void *file_send(file_pthread_t * file_info)
{	
	// 声明并初始化一个服务器端的socket地址结构 
  struct sockaddr_in server_addr; 
  bzero(&server_addr, sizeof(server_addr)); 
  server_addr.sin_family = AF_INET; 
  server_addr.sin_addr.s_addr = htons(INADDR_ANY); 
  server_addr.sin_port = htons(SERVER_PORT); 
  
  // 创建socket，若成功，返回socket描述符 
  int server_socket_fd = socket(PF_INET, SOCK_STREAM, 0); 
  if(server_socket_fd < 0) 
  { 
    perror("Create Socket Failed:"); 
    exit(1); 
  } 
  int opt = 1; 
  setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
  
  // 绑定socket和socket地址结构 
  if(-1 == (bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)))) 
  { 
    perror("Server Bind Failed:"); 
    exit(1); 
  } 
    
  // socket监听 
  if(-1 == (listen(server_socket_fd, LENGTH_OF_LISTEN_QUEUE))) 
  { 
    perror("Server Listen Failed:"); 
    exit(1); 
  } 
  
  while(1) 
  { 
    // 定义客户端的socket地址结构 
    struct sockaddr_in client_addr; 
	//client_addr = file_info->client;
    socklen_t client_addr_length = sizeof(client_addr); 
  
    // 接受连接请求，返回一个新的socket(描述符)，这个新socket用于同连接的客户端通信 
    // accept函数会把连接到的客户端信息写到client_addr中 
    int new_server_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length); 
    if(new_server_socket_fd < 0) 
    { 
      perror("Server Accept Failed:"); 
      break; 
    } 
  
    // recv函数接收数据到缓冲区buffer中 
    char buffer[BUFFER_SIZE]; 
    bzero(buffer, BUFFER_SIZE); 
    //if(recv(new_server_socket_fd, buffer, BUFFER_SIZE, 0) < 0) 
    //{ 
    //  perror("Server Recieve Data Failed:"); 
    //  break; 
    //} 
  
    // 然后从buffer(缓冲区)拷贝到file_name中 
    char file_name[FILE_NAME_MAX_SIZE+1]; 
    bzero(file_name, FILE_NAME_MAX_SIZE+1); 
    strncpy(file_name, file_info->path , strlen(file_info->path )>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(file_info->path )); 
    printf("%s\n", file_name); 
  
    // 打开文件并读取文件数据 
    FILE *fp = fopen(file_name, "r"); 
    if(NULL == fp) 
    { 
      printf("File:%s Not Found\n", file_name); 
    } 
    else
    { 
      bzero(buffer, BUFFER_SIZE); 
      int length = 0; 
      // 每读取一段数据，便将其发送给客户端，循环直到文件读完为止 
      while((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) 
      { 
		 // printf("debug--buffer--%s\n",buffer);
        if(send(new_server_socket_fd, buffer, length, 0) < 0) 
        { 
          printf("Send File:%s Failed./n", file_name); 
          break; 
        } 
        bzero(buffer, BUFFER_SIZE); 
      } 
  
      // 关闭文件 
      fclose(fp); 
      printf("File:%s Transfer Successful!\n", file_name); 
    } 
    // 关闭与客户端的连接 
    close(new_server_socket_fd); 
	break;
  } 
  // 关闭监听用的socket 
  close(server_socket_fd); 
  	printf("debug--end--file_send\n");

}

void destroy_confirm(GtkWidget * a, gpointer data) {

	gtk_widget_destroy(a);
}
void destroy_confirm_win(GtkWidget * a, GtkWidget * window) {

	gtk_widget_destroy(window);
}
/**************************************************/
/*名称：createFileConfirmWindow
/*描述：接收文件确认按钮
/*参数：
         参数1：char *file_name,文件名
/*返回值：void
/*作者：周育聪
/*日期：2017-09-04
/************************************************/
void createFileConfirmWindow(char *file_name)
{	printf("confirem file name%s\n", file_name);
	GtkWidget *window;
	gchar *title = "File Receievd Confirm";
	GtkWidget *button;
	//gchar *button_word = "GET!";
	GtkWidget *vbox;
	GtkWidget *label;
	gchar word[250] = "file has recived, ";
	printf("confirem 1%s\n", file_name);
	strcat(word,file_name);
	printf("confirem 2%s\n", file_name);
	strcat(word," \nfile has been saved under FileRecv dirctory.\n");

	//gtk_init(&argc,&argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(destroy_confirm),NULL);
	gtk_window_set_title(GTK_WINDOW(window),title);
	gtk_window_set_default_size(GTK_WINDOW(window),300,200);
	gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),20);

	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);

	label = gtk_label_new(word);
	gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);
	gtk_box_pack_start(GTK_BOX(vbox),label,FALSE,FALSE,5);

	button = gtk_button_new_from_stock(GTK_STOCK_OK);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(destroy_confirm_win),window);
	gtk_box_pack_start(GTK_BOX(vbox),button,FALSE,FALSE,5);

	gtk_widget_show_all(window);
	gtk_main();
}


/**************************************************/
/*名称：file_recv
/*描述：发送文件函数
/*参数：
         参数1：client、struct sockaddr_in 、输入参数、发送文件方的网络地址信息
         参数2：file_name、char *、输入参数、文件名称
/*返回值：void
/*作者：唐回峰、周育聪
/*日期：2017-09-05
/************************************************/
void file_recv(struct sockaddr_in client,char *fileName)
{
	char path[80] = {0};
	char filepath[80] = {0};
	getcwd(filepath ,FILE_PATH_SIZE);
	printf("file absolute path%s\n", filepath);	
	strcat(path, filepath);
	strcat(path, "/FileRecv/");
	strcat(path, fileName);
	printf("file path %s\n", path);
	 struct sockaddr_in client_addr; 
  bzero(&client_addr, sizeof(client_addr)); 
  client_addr.sin_family = AF_INET; 
  client_addr.sin_addr.s_addr = htons(INADDR_ANY); 
  client_addr.sin_port = htons(0); 
  

  char *client_ip;

  
  //printf("debug-------%ul----\n",client.sin_addr.s_addr);
  printf("debug-------------------3-------------------------msgrecv\n");
  // 创建socket，若成功，返回socket描述符 
  int client_socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
  if(client_socket_fd < 0) { 
    perror("Create Socket Failed:"); 
    exit(1); 
  } 
  
  // 绑定客户端的socket和客户端的socket地址结构 非必需 
  if(-1 == (bind(client_socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)))) 
  { 
    perror("Client Bind Failed:"); 
    exit(1); 
  } 
  // 声明一个服务器端的socket地址结构，并用服务器那边的IP地址及端口对其进行初始化，用于后面的连接 
  struct sockaddr_in server_addr; 
  bzero(&server_addr, sizeof(server_addr)); 
  server_addr.sin_family = AF_INET; 
  printf("debug-------------------3.13-------------------------msgrecv\n");
  printf("");
  //if(inet_pton(AF_INET, client.sin_addr.s_addr, &server_addr.sin_addr) == 0)
  //client_addr.sin_addr.s_addr = "192.168.200.241";

  client_ip = inet_ntoa(client.sin_addr);
  printf("debug-------%s----\n",client_ip);
  if(inet_pton(AF_INET, client_ip, &server_addr.sin_addr) == 0)
  { 
    perror("Server IP Address Error:"); 
    exit(1); 
  } 
  server_addr.sin_port = htons(SERVER_PORT); 
  socklen_t server_addr_length = sizeof(server_addr); 
  
  // 向服务器发起连接，连接成功后client_socket_fd代表了客户端和服务器的一个socket连接 
  if(connect(client_socket_fd, (struct sockaddr*)&server_addr, server_addr_length) < 0) 
  { 
    perror("Can Not Connect To Server IP:"); 
    exit(0); 
  } 
  
  // 输入文件名 并放到缓冲区buffer中等待发送 
  char file_name[FILE_NAME_MAX_SIZE+1]; 
  bzero(file_name, FILE_NAME_MAX_SIZE+1); 
  strcpy(file_name,path);  
  char buffer[BUFFER_SIZE]; 
  bzero(buffer, BUFFER_SIZE); 
  strncpy(buffer, file_name, strlen(file_name)>BUFFER_SIZE?BUFFER_SIZE:strlen(file_name)); 
    
  // 向服务器发送buffer中的数据 
  if(send(client_socket_fd, buffer, BUFFER_SIZE, 0) < 0) 
  { 
    perror("Send File Name Failed:"); 
    exit(1); 
  } 
  
  // 打开文件，准备写入 

  FILE *fp = fopen(file_name, "w"); 
  printf("------------------------%s\n",file_name);
  printf("debug-------------------6-------------------------msgrecv\n");
  if(NULL == fp) 
  { 
    printf("File:\t%s Can Not Open To Write\n", file_name); 
    exit(1); 
  } 
  
  // 从服务器接收数据到buffer中 
  // 每接收一段数据，便将其写入文件中，循环直到文件接收完并写完为止 
  bzero(buffer, BUFFER_SIZE); 
  int length = 0; 
  while((length = recv(client_socket_fd, buffer, BUFFER_SIZE, 0)) > 0) 
  { 
//	  printf("debug--buffer--%s\n",buffer);
	  //printf("lenth:---------------%d--------%d\n",length,);
	  int tmp_len;
	  tmp_len = fwrite(buffer, sizeof(char), length, fp);
	  fflush(fp);
	  printf("lenth:---------------%d--------%d\n",length, tmp_len);
    if(tmp_len < length) 
    { 
      printf("File:\t%s Write Failed\n", file_name); 
      break; 
    } 
    bzero(buffer, BUFFER_SIZE); 
  } 
  
  // 接收成功后，关闭文件，关闭socket 
  printf("Receive File:\t%s From Server IP Successful!\n", file_name); 
  close(fp); 
  close(client_socket_fd); 
  //createFileConfirmWindow(fileName);

printf("file recv end\n");
}


