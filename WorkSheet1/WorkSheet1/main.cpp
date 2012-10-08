#include "Graph.h"
//#include "FileParser.h"

int main(){
	char fileName[256];
	memset(fileName, 0, 256 * sizeof(char));
	printf("Please enter the fileName of the map, limit 256 characters:");
	scanf("%s", fileName);
	Graph graph(fileName);
	if(graph.parser.success)
		printf("File Successfully Open and Parsed!");
	else
		printf("File Opening and Parsing failed!");
	int choices = graph.numNodes;
	graph.PrintNodes();

	int first = 0, second = 0;
	printf("\nPlease enter the path you wish to travel in the format of (node1,node2), excluding braces\n");
	scanf("%d,%d", &first, &second);
	graph.AStar(first, second);
	return 0;
}