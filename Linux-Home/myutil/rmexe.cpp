#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>

void initrootdir(std::vector<std::string> & pathname)
{
	DIR * pdir = NULL;
	struct dirent * pdirent = 0;

	pathname.push_back("/");

	if((pdir = opendir("/")) == NULL){
		perror("opendir error: ");
		return;
	}
	else{
		while((pdirent = readdir(pdir)) != NULL){
			struct stat statinfo;
			
			if(stat((std::string("/") + pdirent->d_name).c_str(),&statinfo) == 0){
				if(S_ISDIR(statinfo.st_mode)){
					if(strcmp(pdirent->d_name,".") != 0 &&  strcmp(pdirent->d_name,"..") != 0){
						pathname.push_back(std::string(pdirent->d_name));		
					}
				}
			}
			else{
				perror("stat error");
			}
		}

		closedir(pdir);
	}
}

#define WHOLE(CONTAINER) CONTAINER.begin(),CONTAINER.end()
#define RWHOLE(CONTAINER) CONTAINER.begin(),CONTAINER.end()

void rmexe_in_dir(char * dirname)
{
	DIR * pdir = NULL;
	if((pdir = opendir(dirname)) == NULL){
		perror("opendir error: ");
		exit(1);
	}

	static int count  = 0;

	struct stat statinfo;
	struct dirent * pdirent;
	while((pdirent = readdir(pdir)) != 0){
		if(stat(pdirent->d_name,&statinfo) == 0 && strcmp(pdirent->d_name,".") != 0 &&
			strcmp(pdirent->d_name,"..") != 0){

			mode_t mode = statinfo.st_mode;
			if(S_ISDIR(mode)){
				printf("name = %s\n",pdirent->d_name);
				rmexe_in_dir(pdirent->d_name);
			}
			else if(S_ISREG(mode)){
				if((S_IXUSR & mode) || (S_IXGRP & mode) || (S_IXOTH & mode)){
					remove(pdirent->d_name);
					++count;
				}
			}
		}
	}

	closedir(pdir);
	printf("%d exe files removed.\n",count);
}

int main(int argc,char * argv[])
{
	if(argc != 2){
		fprintf(stderr,"usage: %s <pathname>\n",argv[0]);
		exit(1);
	}

	std::vector<std::string> path_no_safe_vec;
	initrootdir(path_no_safe_vec);
	if(std::find(WHOLE(path_no_safe_vec),argv[1]) != path_no_safe_vec.end()){
		printf("remove exe in %s may be not safe\n",argv[1]);
		exit(1);
	}

	rmexe_in_dir(argv[1]);

	return 0;
}
