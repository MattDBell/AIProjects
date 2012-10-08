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
	bool success;
	int numPoints;
	Point * points;

	FileParser()
		:numPoints(0), points(0), success(false)
	{}
	FileParser(char * fileName)
		:numPoints(0), points(0), success(false){
		Parse(fileName);
	}
	void Parse(char * fileName){
		FileRAII file(fileName, "r");
		if(file.file == NULL || ferror(file.file) != 0)
			return;

		//Get to first line
		
		fscanf(file.file, " %d ", &numPoints);

		//Clear useless lines
		//Fill Point coordinates
		points = new Point[numPoints];
		for(int i = 0; i < numPoints; ++i){
			fscanf(file.file, " %d %d ", &points[i].x, &points[i].y);
			if(feof(file.file))
				return;
		}
		
		char in[100];
		memset(in, 0, 100 * sizeof(char));
		int* connections = new int[numPoints];
		for(int i = 0; i < numPoints; ++i){
			fgets(in, 100, file.file);
			if(feof(file.file))
				return;
			if(in[0] == '\n' || in[0] == '\0'){
				--i;
				continue;
			}
			int digits = 0;
			int currNum = 0;
			int pos = 0;
			int arrayPos = 0;
			memset(connections, 0, sizeof(int) * numPoints);
			while(in[pos] != '\0'){
				if(in[pos] == ' '){
					if(digits != 0){
						connections[arrayPos++] = currNum;
						digits = 0;
						currNum = 0;
						
					}
				}
				if(in[pos] >= '0' && in[pos] <= '9'){
					currNum *= 10;
					currNum += (int)(in[pos] - '0');
					++digits;
				}
				++pos;
			}
			if(digits != 0){
				connections[arrayPos++] = currNum;
				digits = 0;
				currNum = 0;
			}
			points[i].numConnections = arrayPos;
			if(arrayPos == 0)
				continue;
			points[i].connections = new int[arrayPos];
			for(int y = 0; y < arrayPos; ++y){
				points[i].connections[y] = connections[y];
			}
		}
		success = true;
		delete[] connections;
	}
	

};

#endif