#ifndef FILERAII_H
#define FILERAII_H
#include <cstdio>

struct FileRAII{
	FILE * file;
	FileRAII(char * fileName, char * mode)
		:file(NULL)
	{
		file = fopen(fileName, mode);
	}
	~FileRAII(){
		if(file != NULL)
			fclose(file);
	}
};

#endif