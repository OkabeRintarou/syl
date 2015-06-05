#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static char * str = 
"#include <sys/types.h>\n\
#include <sys/wait.h>\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <fcntl.h>\n\
#include <unistd.h>\n\
#include <string.h>\n\
\n\
int main(int argc,char * argv[])\n\
{\n\
	return 0;\n\
}\n";

#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc,char * argv[])
{
	if(argc != 2){
		fprintf(stderr,"usage: %s <filename>\n",argv[0]);
		exit(1);
	}

	int fd;
	if((fd = open(argv[1],O_RDWR | O_CREAT,RWRR)) < 0){
		perror("open error\n");
		exit(1);
	}

	static const int len = strlen(str);
	printf("len =  %ld",len);
	char buf[len];
	strcpy(buf,str);
	if((write(fd,buf,len)) != len){
		perror("write error\n");
		exit(1);
	}

	close(fd);

	static const int cmdlen = strlen("subl ") + strlen(argv[1]);
	char cmdbuf[cmdlen + 1];
	cmdbuf[0] = 's';cmdbuf[1] = 'u';cmdbuf[2] = 'b';cmdbuf[3] = 'l';cmdbuf[4] = ' ';
	strcpy(cmdbuf + 5,argv[1]);
	cmdbuf[cmdlen] = '\0';

	system(cmdbuf);
	return 0;
}