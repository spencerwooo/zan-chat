#ifndef CONVERTER_H
#define CONVERTER_H

#include "strToPixbufConverter.h"
#include "dateConverter.h"
#include "picConverter.h"

/**************************************************/
/*名称：		converter.h
/*描述：		一些编码转换函数
/***************************************************/

int u16_to_u8(short *utf_16, char *outbuf);

int code_convert(char *from_charset, char *to_charset, short *inbuf, int inlen, char *outbuf, int outlen);
#endif
