#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define TRUE  1
#define MAXLINE 80

int setup(char * inputBuffer,char * args[],int * background)
{
	int start = 0;
	int end = 0;
	int i = 0;
	while(*(inputBuffer + end) != 0){
		if(isspace(*(inputBuffer + end))){
			end++;
			continue;
		}
		start = end;
		while(!isspace(*(inputBuffer + end))){
			end++;
		}
		args[i] = (char*)malloc(end - start + 1);
		memcpy(args[i],inputBuffer + start,end - start);
		args[i][end-start] = '\0';
		i++;
	}
	if(strcmp(args[i-1],"&") == 0){
		i -=1;
		*background = 1;
	}
	else{
		*background = 0;	
	}

	args[i] = NULL;
	return i;
}

int main(int argc, char *argv[])
{
	char inputBuffer[MAXLINE];
	int background;
	char * args[MAXLINE/2];

	while(TRUE){
		printf("COMMAND->");
		fgets(inputBuffer,MAXLINE,stdin);
		inputBuffer[strlen(inputBuffer)] = '\0';
		int args_len = setup(inputBuffer,args,&background);
		
		if(fork() == 0){
			execvp(args[0],args);
		}
		else{
			if(background == 0){
				wait(NULL);				
			}
		}
	}

}