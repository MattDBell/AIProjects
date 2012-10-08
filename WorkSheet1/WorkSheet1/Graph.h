#ifndef GRAPH_H
#define GRAPH_H
#include <cmath>
#include "FileParser.h"

struct Graph{
	struct Node;
	struct Connection{
		Node * node;
		float weight;
	};
	struct Node{
		int x;
		int y;
		int numConnections;
		Connection* connections;
	};
	FileParser parser;
	Node * nodes;
	int numNodes;
	Graph(char * fileName)
		:parser(fileName)
	{
		if(parser.success)
			ConvertData();
	}
	float Distance(Node* node1, Node* node2){
		float distX = node1->x - node2->x;
		float distY = node1->y - node2->y;
		return sqrt(distX * distX + distY * distY);
	}
	void ConvertData(){
		numNodes = parser.numPoints;
		nodes = new Node[numNodes];
		memset(nodes, 0, sizeof(Node) * numNodes);
		//Build all the nodes!
		for(int i = 0; i < numNodes; ++i){
			nodes[i].x = parser.points[i].x;
			nodes[i].y = parser.points[i].y;
			nodes[i].numConnections = parser.points[i].numConnections;
			nodes[i].connections = new Connection[nodes[i].numConnections];
		}
		for(int i = 0; i < numNodes; ++i){
			for(int y = 0; y < nodes[i].numConnections; ++y){
				nodes[i].connections[y].node = &nodes[parser.points[i].connections[y]];
				nodes[i].connections[y].weight = Distance(&nodes[i], &nodes[parser.points[i].connections[y]]);
			}
		}
	}
	void PrintNodes(){
		for(int i = 0; i < numNodes; ++i){
			printf("\nNode %d : %d , %d ", i, nodes[i].x, nodes[i].y);
		}
	}
	
	struct AStarStep{
		AStarStep* nextInList;

		Node* currNode;
		float gToThisPoint;
		float hFromThisPoint;
		AStarStep* LastStep;
		~AStarStep(){
			if(nextInList != NULL)
				delete nextInList;
		}
	};
	void AStar(int node1, int node2){
		printf("\nAStar From: (%d, %d) To: (%d, %d)\n", nodes[node1].x, nodes[node1].y, nodes[node2].x, nodes[node2].y);

		AStarStep* curr = new AStarStep();
		curr->nextInList = NULL;
		curr->currNode = &nodes[node1];
		curr->gToThisPoint = 0;
		curr->hFromThisPoint = Distance(curr->currNode, &nodes[node2]);
		curr->LastStep = NULL;

		int closedNum = 0;
		AStarStep** closed = new AStarStep*[numNodes];
		memset(closed, 0,sizeof(int) * numNodes);
		while(curr->currNode != &nodes[node2]){
			AStarStep* toAdd = NULL;
			for(int addOpen = 0; addOpen < curr->currNode->numConnections; ++addOpen){
				bool skip = false;
				for(int check = 0; check < closedNum; ++check){
					if(closed[check]->currNode == curr->currNode->connections[addOpen].node){
						skip = true;
						break;
					}
				}
				if(skip) continue;
				AStarStep* adding = new AStarStep();
				adding->currNode = curr->currNode->connections[addOpen].node;
				adding->gToThisPoint = curr->gToThisPoint + curr->currNode->connections[addOpen].weight;
				adding->hFromThisPoint = Distance(adding->currNode, &nodes[node2]);
				adding->LastStep = curr;
				adding->nextInList = toAdd;
				toAdd = adding;
			}
			closed[closedNum++] = curr;
			curr = curr->nextInList;
			closed[closedNum -1]->nextInList = NULL;
			while(toAdd != NULL){				
				AStarStep* next = toAdd->nextInList;
				if(curr == NULL){
					curr = toAdd;
					toAdd->nextInList = NULL;
				}else if(curr->gToThisPoint + curr->hFromThisPoint > toAdd->gToThisPoint + toAdd->hFromThisPoint) {
					toAdd->nextInList = curr;
					curr = toAdd;
				} else{
					AStarStep* iter = curr;
					while(iter->nextInList != NULL &&
						iter->nextInList->gToThisPoint + iter->nextInList->hFromThisPoint < toAdd->gToThisPoint + toAdd->hFromThisPoint){
						iter = iter->nextInList;
					}
					toAdd->nextInList = iter->nextInList;
					iter->nextInList = toAdd;
				}
				toAdd = next;
			}
			
		}
		AStarStep* iter = curr;
		while(iter != NULL){
			printf("\nStep Ends At (%d, %d), distance traveled is %3.2f, distance left is %3.2f", iter->currNode->x, iter->currNode->y, iter->gToThisPoint, iter->hFromThisPoint);
			iter = iter->LastStep;
		}
		for(int i = 0; i < closedNum; ++i){
			delete closed[i];
		}
		delete[] closed;
		delete curr;

	}
};

#endif//GRAPH_H