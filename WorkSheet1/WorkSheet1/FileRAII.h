#ifndef FILERAII_H
#define FILERAII_H
#include <cstdio>

struct FileRAII{
	FILE * file;
	FileRAII(char * fileName, char * mode){
		file = fopen(fileName, mode);
	}
	~FileRAII(){
		fclose(file);
	}
};

#endif