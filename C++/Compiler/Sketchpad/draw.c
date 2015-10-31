#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "parser.h"

int main(int argc, char * argv[])
{
	int i;

	if(argc == 1){
		fprintf(stderr,"usage:%s <filename>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		for(i = 1; i < argc;++i ){
			parser_file(argv[i]);
		}
	}
	return 0;
}
