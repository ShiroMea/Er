#include "er.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL,"");
	SET_NAME(argv);

	int ret = 0;
	MULIT_TYPE data;
	data.quad_word = 0;
	unsigned int seed = 0;
	unsigned int size = WORD; /* 默认大小为16bit */
	static const char *short_opt = "todxcbwlqsuehv";

	while ((ret = getopt(argc,argv,short_opt)) != -1)
		switch (ret) {
			case 't':
				seed |= SEED_BINERY;
				break;
			case 'o':
				seed |= SEED_OCTAL;
				break;
			case 'd':
				seed |= SEED_DECIMAL;
				break;
			case 'x':
				seed |= SEED_HEXADECIMAL;
				break;
			case 'c':
				seed |= SEED_ASCII;
				break;
			case 'b':
				size = BYTE;
				break;
			case 'w':
				size = WORD;
				break;
			case 'l':
				size = DOUBLE_WORD;
				break;
			case 'q':
				size = QUAD_WORD;
				break;
			case 's':
				UNSIGNED = false;
				break;
			case 'u':
				UNSIGNED = true;
				break;
			case 'e':
				TITLE = true;
				break;
			case 'h':
				return (helper(EXIT_SUCCESS));
				//break;
			case 'v':
				version();
				return EXIT_SUCCESS;
				//break;
			default:
				return (helper(EXIT_FAILURE));
		}

	if (seed == 0 || argc < 3) {
		fprintf(stderr,"%s 什么都没有做!\n",NAME);
		return EXIT_FAILURE;
	}

	for (int i = 1;i < argc;i++) {
		if (argv[i][0] != '-') {
			data = er_input(argv[i],size);
			if (errno != 0) {
				fprintf(stderr,"Error:%s:%s\n",strerror(errno),argv[i]);
				return EXIT_FAILURE;
			}
		}
	}

	return (er_doit(data,seed,size));
}
