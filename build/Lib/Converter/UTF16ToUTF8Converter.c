#include "../../Include/converter.h"
#include <iconv.h>

/**************************************************/
/*名称：UTF16ToUTF8Converter.c
/*描述：编码转换函数
/*作成日期： 2017-09-05
/*作者： 唐回峰、周育聪
/***************************************************/

int u16_to_u8(short * utf_16, char * outbuf) {
	int i;
	for(i = 0; utf_16[i] != '\0'; i++){}
	return code_convert("utf-16", "utf-8", utf_16, i, outbuf, 255);
}

int code_convert(char *from_charset, char *to_charset, short *inbuf, int inlen,
char *outbuf, int outlen) {
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset, from_charset);
	if(cd == 0) return -1;
	//memset(outbuf, 0, outlen);
	if(iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	iconv_close(cd);
	return 0;
}
