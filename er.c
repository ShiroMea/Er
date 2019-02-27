#include "er.h"

bool TITLE = false;
bool UNSIGNED = false;
const char *NAME = NULL;

/* 根据size来设置printf和sscanf的占位符的大小
 * 如果size为QUAD_WORD,type 为 "od" （代表整型)
 * 其中第一个字符'o'代表输出用于printf,'i'字符用于scanf
 * 其中第二个字符'd'代表输出输出的数据类型
 * 'd' 代表十进制, 'o' 代表八进制, 'x' 代表十六进制数 'c' 代表字符
 * 's' 代表二进制字符串
 * 则会生成%lld
 */
const char *er_setsize(const char *type,int size)
{
	static char format[6] = "";
	char *format_str = NULL;
	switch (size) {
		case BYTE:
			if (*type == 'o' && (type[1] == 'o' || type[1] == 'x'))
				format_str = "%%#hh%c";
			else
				format_str = "%%hh%c";
			break;
		case WORD:
			if (*type == 'o' && (type[1] == 'o' || type[1] == 'x'))
				format_str = "%%#h%c";
			else
				format_str = "%%h%c";
			break;
		case DOUBLE_WORD:
			if (*type == 'o' && (type[1] == 'o' || type[1] == 'x'))
				format_str = "%%#l%c";
			else
				format_str = "%%l%c";
			break;
		case QUAD_WORD:
		default:
			if (*type == 'o' && (type[1] == 'o' || type[1] == 'x'))
				format_str = "%%#ll%c";
			else
				format_str = "%%ll%c";
			break;
	}

	if (UNSIGNED == true && *type == 'o' && type[1] == 'd')
		snprintf(format,sizeof(format),format_str,'u');
	else
		snprintf(format,sizeof(format),format_str,type[1]);

	return format;
}

/* See er.h */
const char *er_binery(const MULIT_TYPE data,int size)
{
	unsigned int seek = 0;
	unsigned int count = 0;
	long long process = 0;
	static char binery[sizeof(long long) * 8 + 1] = "";

	/* 指向binery数组的最后一个成员 */
	seek = sizeof(long long) * 8;
	binery[seek--] = '\0';

	switch (size) {
		case BYTE:
			process = data.byte;
			count = sizeof(char) * 8;
			break;
		case WORD:
			process = data.word;
			count = sizeof(short) * 8;
			break;
		case DOUBLE_WORD:
			process = data.double_word;
			count = sizeof(long) * 8;
			break;
		case QUAD_WORD:
			process = data.quad_word;
			count = sizeof(long long) * 8;
			break;
		default:
			errno = EINVAL;
			return NULL;
			break;
	}

	for (;count > 0; count--, process >>= 1) {
		if ((process & 1) == 1)
			binery[seek] = '1';
		else
			binery[seek] = '0';
		seek--;
	}

	seek++;
	return binery + seek;
}

/* See er.h */
const MULIT_TYPE er_number(const char *binery)
{
	MULIT_TYPE data;
	data.quad_word = 0;

	CHECK_ARGUMENT(binery == NULL,data);

	unsigned int seek = 0;

	for (seek = 0;binery[seek] != '\0';seek++, data.quad_word <<= 1) {
		if (binery[seek] == '1')
			data.quad_word |= 1;
		else if (binery[seek] == '0')
			data.quad_word |= 0;
		else {
			fprintf(stderr,"Invalid char:%c\n",binery[seek]);
			data.quad_word >>= 1;
		}
	}

	data.quad_word >>= 1;

	return data;
}

/* See er.h */
const char *er_format(int type,char doit,int size)
{
	const char *format = NULL;
	char control[3] = "";

	switch (type) {
		case BINERY: /* 二进制 */
			snprintf(control,sizeof(control),"%c%c",doit,'s');
			format = er_setsize(control,BYTE);
			break;
		case OCTAL: /*  八进制 */
			snprintf(control,sizeof(control),"%c%c",doit,'o');
			format = er_setsize(control,size);
			break;
		case HEXADECIMAL: /* 十六进制 */
			snprintf(control,sizeof(control),"%c%c",doit,'x');
			format = er_setsize(control,size);
			break;
		case ASCII:   /* ascii字符 */
			snprintf(control,sizeof(control),"%c%c",doit,'c');
			format = er_setsize(control,BYTE);
			break;
		case DECIMAL: /* 十进制 */
		default:
			snprintf(control,sizeof(control),"%c%c",doit,'d');
			format = er_setsize(control,size);
			break;
	}

	return format;
}

/* See er.h */
const MULIT_TYPE er_input(const char *value,int size)
{
	int ret = 0;
	MULIT_TYPE data;
	data.quad_word = 0;
	const char *format = NULL;

	switch (*value) {
		case 't':  /* 输入是二进制 */
			data = er_number(value + 1);
			return data;
			//break;
		case 'o':  /* 输入是八进制 */
			format = er_format(OCTAL,'i',size);
			break;
		case 'd':  /* 输入是十进制 */
			format = er_format(DECIMAL,'i',size);
			break;
		case 'x':  /* 输入是十六进制 */
			format = er_format(HEXADECIMAL,'i',size);
			break;
		case 'c':  /* 输入是一个字符 */
			format = er_format(ASCII,'i',size);
			break;
		default:  /* 未知的类型 */
			errno = EINVAL;
			return data;
			break;
	}

	if ((ret = sscanf(value + 1,format,&data.quad_word)) <= 0)
		errno = EINVAL;

	return data;
}

/* See er.h */
int er_output(const MULIT_TYPE data,int o_type,int size)
{
	char *title = NULL;
	const char *format = NULL;

	switch (o_type) {
		case BINERY:
			printf("%s",(TITLE == true) ? "二进制  :" : "");
			printf(er_format(BINERY,'o',size),er_binery(data,size));
			putchar('\n');
			return 0;
			//break;
		case OCTAL:
			title = (TITLE == true) ? "八进制  :" : "";
			format = er_format(OCTAL,'o',size);
			break;
		case DECIMAL:
			title = (TITLE == true) ? "十进制  :" : "";
			format = er_format(DECIMAL,'o',size);
			break;
		case HEXADECIMAL:
			title = (TITLE == true) ? "十六进制:" : "";
			format = er_format(HEXADECIMAL,'o',size);
			break;
		case ASCII:
			title = (TITLE == true) ? "Ascii   :" : "";
			format = er_format(ASCII,'o',size);
			break;
		default:
			errno = EINVAL;
			return EXIT_FAILURE;
	}

	printf("%s",title);
	switch (size) {
		case BYTE:
			printf(format,data.byte);
			break;
		case WORD:
			printf(format,data.word);
			break;
		case DOUBLE_WORD:
			printf(format,data.double_word);
			break;
		case QUAD_WORD:
		default:
			printf(format,data.quad_word);
	}
	putchar('\n');
	fflush(stdout);

	return 0;
}

/* See er.h */
int er_doit(const MULIT_TYPE data,unsigned seed,int size)
{
	int ret = 0;
	unsigned int count = 0;

	for (;count <= ASCII_BIT;count++) {
		if ((seed >> count) & 01) {
			switch (count) {
				case BINERY_BIT:
					ret = er_output(data,BINERY,size);
					break;
				case OCTAL_BIT:
					ret = er_output(data,OCTAL,size);
					break;
				case DECIMAL_BIT:
					ret = er_output(data,DECIMAL,size);
					break;
				case HEXADECIMAL_BIT:
					ret = er_output(data,HEXADECIMAL,size);
					break;
				case ASCII_BIT:
					ret = er_output(data,ASCII,size);
					break;
				default:
					fprintf(stderr,"Invalid bit %d\n",count);
					return EXIT_FAILURE;
					break;
			}
		}
	}

	return ret;
}

int helper(int status)
{
	if (status == EXIT_SUCCESS) {
		printf("Usage %s <options> <<flag>data>\n" \
			   "输出控制(options):\n" \
			   "-t      输出二进制\n" \
			   "-o      输出八进制\n" \
			   "-d      输出十进制\n" \
			   "-x      输出十六进制\n" \
			   "-c      输出Ascii字符\n" \
			   "输入控制(flag):\n" \
			   "t       输入是二进制\n" \
			   "o       输入是八进制\n" \
			   "d       输入是十进制\n" \
			   "x       输入是十六进制\n" \
			   "c       输入是Ascii字符\n" \
			   "数据大小控制:\n" \
			   "-b      输出与输入是 8bit大小\n" \
			   "-w      输出与输入是16bit大小 <=\n" \
			   "-l      输出与输入是32bit大小\n" \
			   "-q      输出与输入是64bit大小\n" \
			   "数据符号控制:\n"\
			   "-s      输出带符号 <=\n"\
			   "-u      输出无符号\n"\
			   "其他:\n" \
			   "-e      显示标题\n" \
			   "-h      显示这个帮助并退出\n" \
			   "-v      显示版本信息并退出\n" \
			   "Example:\n" \
			   " %s -tb d8 :将十进制8转换为二进制\n" \
			   , NAME,NAME);
	} else  {
		fprintf(stderr,"%s 试试 %s -h 来获取帮助\n",NAME,NAME);
	}

	return status;
}

void version(void)
{
	sro_printf("#S5Shiro#S0\n");
#ifdef VERSION
	sro_printf("%s Version #F7%s#F0\n",NAME,VERSION);
#else
	sro_printf("%s Version #F2%s#F0\n",NAME,"Not defined");
#endif
	return ;
}
