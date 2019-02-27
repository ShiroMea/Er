#ifndef ER_H
#define ER_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#include <getopt.h>
#include <stdbool.h>

#include <shiro.h>
#include <color.h>

#define ER_ERROR  (-1)

/* 数据大小 */
#define BYTE        1  /* 字节 */
#define WORD        2  /* 字   */
#define DOUBLE_WORD 3  /* 双字 */
#define QUAD_WORD   4  /* 四字 */

/* 数据类型 */
#define BINERY      5  /* 二进制 */
#define OCTAL       6  /* 八进制 */
#define DECIMAL     7  /* 十进制 */
#define HEXADECIMAL 8  /* 十六进制 */
#define ASCII       9  /* ascii字符 */

#define SET_NAME(argv) \
	do { \
		NAME = ((NAME = strrchr((argv)[0],'/')) == NULL)?(argv)[0]:NAME + 1; \
	} while (0)

#define CHECK_ARGUMENT(expr,ret) \
	do {\
		if ((expr)) { \
			errno = EINVAL; \
			return ((ret)); \
		} \
	} while (0)

extern bool TITLE;    /* 是否在结果中显示标题(默认关闭) */
extern bool UNSIGNED; /* 输出使用无符号(默认关闭) */
extern const char *NAME;


typedef union {
	char      byte;        /* 字节 8bit  */
	short     word;        /* 字   16bit */
	long      double_word; /* 双字 32bit */
	long long quad_word;   /* 四字 64bit */
} MULIT_TYPE;

enum {
	SEED_BINERY      = 1,
	SEED_OCTAL       = SEED_BINERY      * 2,
	SEED_DECIMAL     = SEED_OCTAL       * 2,
	SEED_HEXADECIMAL = SEED_DECIMAL     * 2,
	SEED_ASCII       = SEED_HEXADECIMAL * 2,

	BINERY_BIT       = 0,
	OCTAL_BIT,
	DECIMAL_BIT,
	HEXADECIMAL_BIT,
	ASCII_BIT
};
	
const char *er_binery(const MULIT_TYPE data,int size);
/* 将数据转换为二进制,二进制数值将以字符串返回
 * 其中data为即将要进行转换的数值
 * size指明转换数据类型的大小,应该是已下的几个值:
 * BYTE        字节 8bit
 * WORD        字   16bit
 * DOUBLE_WORD 双字 32bit
 * QUAD_WORD   四字 64bit
 * 执行成功返回指向存储二进制数据值的字符串
 * 如果出现错误返回NULL
 */

const MULIT_TYPE er_number(const char *binery);
/* 将存储二进制数值的字符串转换为数值
 * binery应该是er_binery函数的返回值
 * 执行成功返回相应的数值,出现错误返回0,并设置errno
 * (应该通过errno进行测试是否发生错误,因为0也可能是合法的数据)
 */

const char *er_format(int type,char doit,int size);
/* 用来生成printf和scanf函数的占位符
 * type为输出数据类型
 * BINERY      为二进制 (使用该值将会忽略size参数)
 * DECIMAL     为十进制
 * OCTAL       为八进制
 * HEXADECIMAL 为十六进制
 * ASCII       为ascii字符 (使用该值将会忽略size参数)
 *
 * doit参数指定为printf函数生成还是scanf函数
 * 字符i为scanf,字符o为printf
 * size为数据的大小(参考er_binery的size参数)
 */

const MULIT_TYPE er_input(const char *value,int size);
/* 将命令行参数的数据转换为存储在MULIT_TYPE类型
 * value 开头的第一个字符代表输入数据的进制类型
 * t  代表二进制
 * o  代表八进制
 * d  代表十进制
 * x  代表十六进制
 * c  代表输入是一个字符
 * size为输入数据的大小
 */

int er_output(const MULIT_TYPE data,int o_type,int size);
/* 将data转换为o_type指定的进制类型并输出
 * data应该为er_input函数的返回值
 * o_type为输出的进制类型 (参考er_format函数的type参数)
 * size为数据的大小
 */

int er_doit(const MULIT_TYPE data,unsigned seed,int size);
/* 通过设置的seed来多次调用er_output实现一次转换多种进制
 * data参数应该是er_input函数的返回值
 * size参数指定数据的大小
 */

int helper(int status);
void version(void);

#endif //ER_H
