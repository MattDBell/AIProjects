#include <stdio.h>
#include "Matrix.h"


Matrix::Matrix() : width(0), height(0)
{  
}


//generating vector of rows
Matrix::Matrix(int w, int h) : width(w), height(h)
{
  map.resize(w);
  for (int i=0; i<h; ++i)
  {
    map[i].resize(h);
  }
  player = new Monster(w,h,1);
  monster = new Monster(w,h,2);

  map[w-1][0].P[LEFT] = 1;
  map[w-1][0].P[TILE] = 1;
}

Matrix::~Matrix()
{
}

//moves the monster
bool Matrix::Iterate(char player_move)
{

  player->Move(player_move);

  monster->Update();
  player->Update();

  ProbabilityUpdate();

  UpdateEvents();

  //game over!!
  if (player->GetRow() == monster->GetRow() && 
      player->GetCol() == monster->GetCol())
  {
    printf("YOU ARE TEH DEAD!!\n");
    return false;
  }
  //other game over!!
  else if (player->GetRow() == 0 && player->GetCol() == width-1)
  {
    printf("YOU WINNER!!\n");
    return false;
  }


  return true;
}

void Matrix::UpdateEvents()
{
  event = 0;

  //smell event
  if (abs(player->GetRow() - monster->GetRow()) <= 1 &&
      abs(player->GetCol() - monster->GetCol()) <= 1)
  {
    event = event | SMELL;
  }

  //hear event
  if (monster->GetRow() == monster->GetCol())
  {
    event = event | HEAR;
  }

  //feel event
  if (player->GetRow() == monster->GetRow() ||
      player->GetCol() == monster->GetCol())
  {
    event = event | FEEL;
  }
}


//outputs the map
// *note that the top left coordinate is (0,0)
void Matrix::Print()
{
  PrintEvents();

  for (int row=0; row<height; ++row)
  {
    for (int col=0; col<width; ++col)
    {
      printf("%.3f ", map[col][row].P[TILE]);
    }
    printf("\n");
  }
}


//outputs the map
// *note that the top left coordinate is (0,0)
void Matrix::Reveal()
{
  PrintEvents();

  for (int row=0; row<height; ++row)
  {
    for (int col=0; col<width; ++col)
    {
      //print monster location
      if (monster->GetRow() == row && monster->GetCol() == col)
      {
	switch (monster->GetDir())
	{
	case UP:
	  printf("M ");
	  break;
	case DOWN:
	  printf("M ");
	  break;
	case LEFT:
	  printf("M ");
	  break;
	case RIGHT:
	  printf("M ");
	  break;
	default:
	  break;
	}	
      }
      //print player location and orientation
      else if (player->GetRow() == row && player->GetCol() == col)
      {
	switch (player->GetDir())
	{
	case UP:
	  printf("^ ");
	  break;
	case DOWN:
	  printf("v ");
	  break;
	case LEFT:
	  printf("< ");
	  break;
	case RIGHT:
	  printf("> ");
	  break;
	default:
	  break;
	}
      }
      else
      {
	printf("_ ");
      }
    }
    printf("\n");
  }
}


Tile& Matrix::Element(int col, int row)
{
  //out of range
  if (row < 0 || row >= height || col < 0 || col >= width)
  {
    printf("ERROR: Index out of range\n");
    exit(0);
    return map[0][0];
  }

  return map[row][col];
}


void Matrix::PrintEvents()
{
  int event_count = 0;

  printf("EVENTS: ");

  if (event & SMELL)
  {
    event_count++;
    printf("SMELL ");
  }
  if (event & HEAR)
  {
    event_count++;
    printf("HEAR ");
  }
  if (event & FEEL)
  {
    event_count++;
    printf("FEEL");
  }

  if (event_count == 0)
  {
    printf("none");
  }

  printf("\n");
}


/*
  This is where the brunt of your code will be placed, or at least where you
  will start. 

  For each tile position, you will need to determine the 
  probability the monster is there facing, left, right, up, or down 
  (P[LEFT], P[RIGHT], P[UP], P[DOWN], respectively).

  The overall probability P[TILE] is just the summation of those probabilities
*/
void Matrix::ProbabilityUpdate()
{
	static bool firstRun = true;
	static Tile * cached = NULL;
	if(firstRune){
		cached = new Tile[width * height];
		firstRun = false;
	}
	//cache last results
	for(int x = 0; x < width; ++x){
		for(int y = 0; y < height; ++y){
			for(int facing = 0; facing < 4; ++facing){
				cached[x + y*width].P[facing] = Element(x,y).P[facing];
			}
		}
	}
	//iterate map using cached results
  //obviously get rid of this
  //Element(1,1).P[TILE] = .3333;


}
