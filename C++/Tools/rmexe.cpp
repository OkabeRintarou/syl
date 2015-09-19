#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
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

std::string new_file_name(const std::string & dirname,const std::string & filename)
{
	assert(dirname.length() > 0);
	std::string ret = dirname;
	if(dirname.back() != '/'){
		ret += '/';
	}
	ret += filename;
	return ret;
}

void scan_dir_find_exec(std::vector<std::string> & vec,const std::string & dirname)
{
	DIR * pdir = NULL;
	if((pdir = opendir(dirname.c_str())) == NULL){
		char * err = strerror(errno);
		printf("open dir %s failed: %s.\n ",dirname.c_str(),err);
		return;
	}
	
	struct dirent * pdirent = NULL;
	while((pdirent = readdir(pdir)) != NULL){

		std::string filename = pdirent->d_name;

		if(filename != "." && filename != ".." ){

			struct stat filestat;
			if(stat(new_file_name(dirname,filename).c_str(),&filestat) == 0){
				mode_t mode = filestat.st_mode;
				if(S_ISREG(mode)){
					if((S_IXUSR & mode) || (S_IXGRP & mode) || (S_IXOTH & mode)){
						vec.push_back(new_file_name(dirname,filename));
					}
				}
				else if(S_ISDIR(mode)){
					std::string new_dir = dirname;
					if(dirname.back() != '/'){
						new_dir += "/";
					}
					scan_dir_find_exec(vec,new_dir + filename);
				}
			}			
		}
	}
}

int main(int argc,char * argv[])
{
	if(argc <  2){
		fprintf(stderr,"usage: %s <pathname-list>\n",argv[0]);
		exit(1);
	}
	
	std::vector<std::string> files_to_remove;
	for(int i = 1; i < argc;++i){
		scan_dir_find_exec(files_to_remove,argv[i]);
	}
	for(auto & filename:files_to_remove){
		printf("rm %s ",filename.c_str());
		printf(remove(filename.c_str()) == 0 ? "SUCCEEDED\n":"FAILED\n");
	}
	return 0;
}
