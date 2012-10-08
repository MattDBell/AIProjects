#include "Monster.h"

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
  float roll = ( rand() % 100 ) / 100.0;
  
  
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
void Monster::Move(char option)
{
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
