#include "../../Include/timeHelper.h"
/**************************************************/
/*名称：timeHelper.c
/*描述：时间相关函数
/*作成日期： 2018-08-31
/*作者： 邵子涵
/***************************************************/

void getCurrentDay(char buf[], int size)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf, size, "%Y%m%d", p);
}

void getCurrentTime(char buf[], int size)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	strftime(buf, size, "%Y-%m-%d    %H:%M:%S", p);
	printf("%s\n", buf);
}
