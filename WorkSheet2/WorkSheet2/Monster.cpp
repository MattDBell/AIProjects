#include "Monster.h"
#include "Matrix.h"
#include <cmath>
using namespace std;

Monster::Monster(int c, int r, int player)
{
  width=c; height=r;
  
  //player 1 at bottom left
  if (player == 1) 
    {
      row=r-1; col=0;
      direction = RIGHT;
    }
  //player 2 (monster is top right)
  else 
    {
      row=0; col=c-1;
      direction = LEFT;
    }
  
  p_forward = P_FORWARD;
  p_turn_left = P_TURN_LEFT;
  p_turn_right = P_TURN_RIGHT;
}

Monster::~Monster()
{
}


//moves the monster or player
void Monster::Update()
{
  float roll = ( rand() % 100 ) / 100.0f;
  
  
  //move forward
  if (roll < p_forward)
    {
      //printf("Monster FORWARD!!\n");
      
      switch (direction)
	{
	case UP:
	  row = ( row == 0 ? 0 : (row-1) );
	  break;
	case DOWN:
	  row = ( row == (height-1) ? row : (row+1) );
	  break;
	case LEFT:
	  col = col == 0 ? 0 : (col-1);
	  break;
	case RIGHT:
	  col = ( col == (width-1) ? col : (col+1) );
	  break;
	default:
	  break;
	}
    }
  //turn left
  else if (roll < p_turn_left + p_forward)
    {
      //printf("Monster LEFT!!\n");
      
      switch (direction)
	{
	case UP:
	  direction = LEFT;
	  break;
	case DOWN:
	  direction = RIGHT;
	  break;
	case LEFT:
	  direction = DOWN;
	  break;
	case RIGHT:
	  direction = UP;
	  break;
	default:
	  break;
	}
    }
  //turn right
  else if (roll < p_turn_right + p_turn_left + p_forward)
    {
      //printf("Monster RIGHT!!\n");
      
      switch (direction)
	{
	case UP:
	  direction = RIGHT;
	  break;
	case DOWN:
	  direction = LEFT;
	  break;
	case LEFT:
	  direction = UP;
	  break;
	case RIGHT:
	  direction = DOWN;
	  break;
	default:
	  break;
	}
    }
}

//rotates or moves your player
enum MOVE_CHOICE { MCLEFT, MCRIGHT,MCFORWARD, MCTOTAL};
struct Step{
	int row, col;
	int facing;
	float gTohere; //Distance + Monster chance
	float hFromHere;
	MOVE_CHOICE choice;

	Step * lastStep;
	Step * nextStepInList;

	~Step(){
		if(nextStepInList != NULL)
			delete nextStepInList;
	}
};

float Distance(int row, int col, int toRow, int toCol){
	int distX = row - toRow;
	int distY = col - toCol;
	return sqrt((float)(distX * distX + distY * distY));
}

void Monster::Move(Matrix * mat)
{
	int targetRow = 0;
	int targetCol = 0;
	for(int y = 0; y < 10; ++y){
		for(int x = 0; x < 10; ++x){
			if(mat->Element(x, y).P[TILE] > mat->Element(targetCol, targetRow).P[TILE]){
				targetRow = y;
				targetCol = x;
			}
		}
	}


	Step * curr = new Step();
	Step * closed = NULL;
	curr->row = row;
	curr->facing = direction;
	curr->col = col;
	curr->lastStep = NULL;
	curr->gTohere = 0;
	curr->nextStepInList = NULL;
	curr->choice = MCFORWARD;
	curr->hFromHere = 1000000;
	
	int TURNFACINGS[4][3] = {
		{LEFT, RIGHT, UP},			//UP
		{RIGHT, LEFT, DOWN },			//DOWN
		{DOWN, UP, LEFT },			//LEFT
		{UP, DOWN, RIGHT}			//RIGHT
	};
	while(curr->row != targetRow || curr->col != targetCol){
		Step * toAdd = NULL;
		
		for(int i = 0; i < MCTOTAL ; ++i){
			int endCol= curr->col;
			int endRow= curr->row;
			int endFacing = TURNFACINGS[curr->facing][i];
			if(i == MCFORWARD){
				switch(curr->facing){
					case LEFT:
						endCol = (endCol == 0) ? endCol : endCol -1;
						break;
					case RIGHT:
						endCol = (endCol == width-1) ? endCol : endCol + 1;
						break;
					case UP:
						endRow = (endRow == 0)? endRow : endRow -1;
						break;
					case DOWN:
						endRow = (endRow == height-1)? endRow : endRow + 1;
						break;
				}
			}
			bool skip = false;
			Step* currInClosed = closed;
			while(currInClosed != NULL){
				if(currInClosed->col == endCol &&
					currInClosed->row == endRow &&
					currInClosed->facing == endFacing){
					skip = true;
					break;
				}
				currInClosed = currInClosed->nextStepInList;
			}
			if(skip)
				continue;
			Step* adding = new Step();
			adding->choice = (MOVE_CHOICE)i;
			adding->col = endCol;
			adding->row = endRow;
			adding->facing = endFacing;
			float inverseProbability = 1 - mat->Element(endCol, endRow).P[TILE];
			adding->gTohere = curr->gTohere + 0.02f + inverseProbability * 500;
			adding->hFromHere = Distance(endCol, endRow, targetRow, targetCol);
			adding->lastStep = curr;
			adding->nextStepInList = toAdd;
			toAdd = adding;
		}
		Step * temp = curr;
		curr = curr->nextStepInList;
		temp->nextStepInList = closed;
		closed = temp;
		while(toAdd != NULL){
			Step* next = toAdd->nextStepInList;
			if(curr == NULL || curr->gTohere + curr->hFromHere > toAdd->gTohere + toAdd->hFromHere){
				toAdd->nextStepInList = curr;
				curr = toAdd;
			} else {
				Step * iter = curr;
				while(iter->nextStepInList != NULL &&
					iter->nextStepInList->gTohere + iter->nextStepInList->hFromHere < toAdd->gTohere + toAdd->hFromHere){
					iter = iter->nextStepInList;
				}
				toAdd->nextStepInList = iter->nextStepInList;
				iter->nextStepInList = toAdd;
			}
			toAdd = next;
		}
	}
	char option = 'd';
	Step* iter = curr;
	while(iter->lastStep != NULL && iter->lastStep->lastStep != NULL){
		iter = iter->lastStep;
	}
	switch(iter->choice){
		case MCFORWARD:
			option = 'w';
			break;
		case MCLEFT:
			option = 'a';
			break;
	}
	delete curr;
	delete closed;
	p_forward = 0;
	p_turn_left = 0;
	p_turn_right = 0;


	switch(option)
	{
	case 'f':
	case 'w':
	  p_forward = 1;
	  break;
	case 'l':
	case 'a':
	  p_turn_left = 1;
	  break;
	case 'r':
	case 'd':
	  p_turn_right = 1;
	  break;
	default:
	  break;
	}
}

int Monster::GetRow()
{
  return row;
}

int Monster::GetCol()
{
  return col;
}

int Monster::GetDir()
{
  return direction;
}
