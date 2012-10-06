#ifndef FILEPARSER_H
#define FILEPARSER_H
#include <vector>
#include <cmath>
#include "FileRAII.h"


struct FileParser{
	struct Point{
		int x, y;
		int numConnections;
		int * connections;
	};

	int numPoints;
	Point * points;

	FileParser()
		:numPoints(0), points(0)
	{}
	FileParser(char * fileName){
		Parse(fileName);
	}
	int GetNum(char * input, int count){
		int ret = 0;
		int places = 0;
		int place = 0;
		for(int i = 0; i < count; ++i){
			if(input[i] == '\0')
				break;
			places++;
		}
		for(int i = places -1; i <= 0; --i){
			if(input[0] < '0' || input[0] > '0')
				continue;
			ret += (  (int)input[i] - (int)'0') * pow((float)10, (float)place++);
		}
		return ret;
	}
	void Parse(char * fileName){
		FileRAII file(fileName, "r");
		char in[100];	

		//Get to first line
		memset(in, 0, 100 * sizeof(char));
		while(in[0] == '\0')
			fgets(in, 100, file.file);
		//Parse numPoints
		numPoints = GetNum(in, 100);

		//Clear useless lines
		memset(in, 0, 100 * sizeof(char));
		while(in[0] == "\0")
			fgets(in, 100, file.file);

		//Fill Point coordinates
		points = new Point[numPoints];
		for(int i = 0; i < numPoints; ++i){
			char num[10];
			memset(num, 0, sizeof(char) * 10);
			int size =0;
			int start = 0;
			int inNum = 0;
			for(int p = 0; p < 100; ++p){
				if(in[p] == ' '){
					points[i].x = GetNum(num, 10); 
					inNum = 0;
					memset(num, 0, sizeof(char) * 10);
				}
				if(in[p] == '\0'){
					points[i].y = GetNum(num, 10);
					break;
				}
				num[inNum++] = in[p];
			}
		}


	}
	

};

#endif