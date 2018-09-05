#include "./Include/message.h"

/********************************************/
/*名称：messageParser.c
/*描述：定义消息及消息交互、显示处理的函数
/*作者：孙明君、刘静
/*日期：2017-09-05
/********************************************/


int parseMessage(msg_data_t *msg, char *message_buff, size_t len)
{
    long int_val;
	char *sp = NULL;
	char *ep = NULL;
	char *buffer;
	size_t remains;
	int rc = 0;

	if  ( (!message_buff) || (!msg)  )
	{
		printf("1\n");
		return -EINVAL;
	}
   

 	 buffer = malloc(len);
 	 if (!buffer)
	 {
		printf("2\n");
		return -ENOMEM;
	 }


	gettimeofday(&msg->tv, NULL);

	memcpy(buffer,message_buff,len);
	remains = len;
	
	// 版本号   
	sp = buffer;
	ep = memchr(sp, ':', remains);
	if (!ep) {
		rc = -EINVAL;
		goto err_out;
	}

	*ep='\0';
	remains = len - ((unsigned long)ep-(unsigned long)buffer);
	if (remains<=0) {
		printf("4\n");
		rc = -EINVAL;
		goto err_out;
	}
	++ep;
	int_val = strtol(sp, (char **)NULL, 10);
	msg->version = int_val;
	sp = ep;


	//包序列号
	ep=memchr(sp, ':', remains);
	if (!ep) {
	printf("5\n");
	rc=-EINVAL;
	goto err_out;
	}
  *ep='\0';
  remains =len - ((unsigned long)ep-(unsigned long)buffer);
  if (remains<=0) {
printf("6\n");
    rc=-EINVAL;
    goto err_out;
  }
  ++ep;
  int_val=strtol(sp, (char **)NULL, 10);
  msg->pkt_seq_no=int_val;
  printf("pkt_no:%d(%x)\n",msg->pkt_seq_no,msg->pkt_seq_no);
  sp=ep;

  /*
   * 用户名
   */
  ep=memchr(sp, ':', remains);
  if (!ep) {
printf("7\n");
    rc=-EINVAL;
    goto err_out;
  }
  *ep='\0';
  remains =len - ((unsigned long)ep-(unsigned long)buffer);
  if (remains<=0) {
printf("8\n");
    rc=-EINVAL;
    goto err_out;
  }
  ++ep;
  msg->username=strdup(sp);
  printf("Name:%s\n",msg->username);
  sp=ep;

  /*
   * 主机名称
   */
  ep=memchr(sp, ':', remains);
  if (!ep) {
printf("9\n");
    rc=-EINVAL;
    goto err_out;
  }
  *ep='\0';
  remains =len - ((unsigned long)ep-(unsigned long)buffer);
  if (remains<=0) {
printf("10\n");
    rc=-EINVAL;
    goto err_out;
  }
  ++ep;
  msg->hostname=strdup(sp);
  printf("Host:%s\n",msg->hostname);
  sp=ep;

  /*
   * 命令字和选项   
   */
  ep=memchr(sp, ':', remains);
  if (!ep) {
printf("11\n");
    rc=-EINVAL;
    goto err_out;
  }
  *ep='\0';
  ++ep;
  int_val=strtol(sp, (char **)NULL, 10);
  msg->command=(int_val & 0xff);
  msg->command_opts=(int_val & ~0xffUL);
  printf("Command:0x%x Option:0x%x\n",msg->command,msg->command_opts);
  sp=ep;
  /*
   *信息
   */
if(ep=memchr(sp, ':', remains) != NULL) {

 ep=memchr(sp, ':', remains);
  if (!ep) {
printf("9\n");
    rc=-EINVAL;
    goto err_out;
  }
  *ep='\0';
  remains =len - ((unsigned long)ep-(unsigned long)buffer);
  if (remains<=0) {
printf("10\n");
    rc=-EINVAL;
    goto err_out;
  }
  ++ep;
  msg->message=strdup(sp);
  printf("message:%s\n",msg->message);
  sp=ep;
  
  } else {
	 msg->message=strdup(sp);
  printf("message:%s\n",msg->message);
}
  /*
   *扩展信息
   */
/*
  ep=memchr(sp, '\0', remains);
  if (!ep) {
printf("12\n");
    rc=-EINVAL;
    goto err_out;
  }

  if ( ((unsigned long)ep - (unsigned long)buffer) < len) {
    ++ep;
    sp=ep;
    msg->extstring=strdup(sp);
   printf("extention:%s\n",msg->extstring);
  }*/
err_out:
  free(buffer);
  return rc;
}

int parseUserInfo(u_info_t *u_info, char *str, size_t len)
{
    long int_val;
    char *sp = NULL;
    char *ep = NULL;
    char *buffer;
    size_t remains;
    int rc = 0;

    if ((!str) || (!u_info))
    {
        return -EINVAL;
    }

    buffer = malloc(len);
    if (!buffer)
    {
        return -ENOMEM;
    }

    memcpy(buffer, str, len);
    remains = len;
    sp = buffer;
    ep = memchr(sp, '_', remains);
    if (!ep)
    {
        rc = -EINVAL;
        goto err_out;
    }

    *ep = '\0';
    remains = len - ((unsigned long)ep - (unsigned long)buffer);
    if (remains <= 0)
    {
        rc = -EINVAL;
        goto err_out;
    }
    ++ep;
    u_info->u_name = strdup(sp);
    sp = ep;
    ep = memchr(sp, '_', remains);
    if (!ep)
    {
        rc = -EINVAL;
        goto err_out;
    }
    *ep = '\0';
    remains = len - ((unsigned long)ep - (unsigned long)buffer);
    if (remains <= 0)
    {
        rc = -EINVAL;
        goto err_out;
    }
    ++ep;
    u_info->u_group = strdup(sp);
    sp = ep;
    ep = memchr(sp, '_', remains);
    if (!ep)
    {
        rc = -EINVAL;
        goto err_out;
    }
    *ep = '\0';
    remains = len - ((unsigned long)ep - (unsigned long)buffer);
    if (remains <= 0)
    {
        rc = -EINVAL;
        goto err_out;
    }
    ++ep;
    int_val = strtol(sp, (char **)NULL, 10);
    u_info->u_pic_id = int_val;
    sp = ep;
    if (memchr(sp, '_', remains) != NULL)
    {

        ep = memchr(sp, '_', remains);
        if (!ep)
        {
            rc = -EINVAL;
            goto err_out;
        }
        *ep = '\0';
        remains = len - ((unsigned long)ep - (unsigned long)buffer);
        if (remains <= 0)
        {
            rc = -EINVAL;
            goto err_out;
        }
        ++ep;
        int_val = strtol(sp, (char **)NULL, 10);

        u_info->u_qianming = strdup(sp);
        sp = ep;

        ep = memchr(sp, '_', remains);
        if (!ep)
        {
            rc = -EINVAL;
            goto err_out;
        }
        *ep = '\0';
        remains = len - ((unsigned long)ep - (unsigned long)buffer);
        if (remains <= 0)
        {
            rc = -EINVAL;
            goto err_out;
        }
        ++ep;
        int_val = strtol(sp, (char **)NULL, 10);

        u_info->u_img_code = int_val;
        sp = ep;
        ep = memchr(sp, '_', remains);
        if (!ep)
        {
            printf("11\n");
            rc = -EINVAL;
            goto err_out;
        }
        *ep = '\0';
        remains = len - ((unsigned long)ep - (unsigned long)buffer);
        if (remains <= 0)
        {
            rc = -EINVAL;
            goto err_out;
        }
        ++ep;
        int_val = strtol(sp, (char **)NULL, 10);

        u_info->u_avatar_code = int_val;
        sp = ep;
    }
    else
    {
        u_info->u_qianming = strdup(sp);
    }

err_out:
    free(buffer);
    return rc;
}
