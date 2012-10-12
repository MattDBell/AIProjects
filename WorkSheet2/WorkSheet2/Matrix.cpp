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

  player->Move(this);

  monster->Update();
  player->Update();
  UpdateEvents();
  ProbabilityUpdate();

  

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
	static int sWidth = 0, sHeight = 0;
	static Tile * cached = NULL;
	if(width > sWidth || height > sHeight){
		if(cached != NULL)
			delete cached;
		sWidth = sWidth > width ? sWidth : width;
		sHeight = sHeight > height ? sHeight: height;
		cached = new Tile[sWidth * sHeight];
	}
	//cache last results
	

	Prepare(cached);
	UpdateOnMovement(cached);

	
	UpdateOnSmell(cached);

	UpdateOnHear(cached);

	UpdateOnFeel(cached);

	for(int x= 0; x < width; ++x){
		for(int y = 0; y < height; ++y){
			Tile & t = Element(x, y);
			t.P[TILE] = t.P[UP] + t.P[DOWN] + t.P[LEFT] + t.P[RIGHT];
		}
	}
	//iterate map using cached results
	//obviously get rid of this
	//Element(1,1).P[TILE] = .3333;

}
void Matrix::Prepare(Tile * cached){
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			for(int facing = 0; facing < 4; ++facing){
				cached[x + y*width].P[facing] = Element(x,y).P[facing];
				Element(x,y).P[facing] = 0;
			}
		}
	}
}
void Matrix::UpdateOnMovement(Tile * cached){
	for(int y = 0; y < height; ++y){
		for(int x = 0; x < width; ++x){
			for(int facing = 0; facing < TILE; ++facing){
				int forwardChangeX = 0, forwardChangeY = 0;;
				int rightDir = RIGHT;
				int leftDir = LEFT;
				switch(facing){
					case UP:
						forwardChangeY = y==0? 0 : -1;
						rightDir = RIGHT;
						leftDir= LEFT;
						break;
					case DOWN:
						forwardChangeY = y== height-1? 0 : 1;
						rightDir = LEFT;
						leftDir = RIGHT;
						break;
					case LEFT:
						forwardChangeX = x == 0 ? 0 : -1;
						rightDir = UP;
						leftDir = DOWN;
						break;
					case RIGHT:
						forwardChangeX = x == width - 1 ? 0 : 1;
						rightDir = DOWN;
						leftDir = UP;
						break;
				}
				Element(x, y).P[rightDir] += cached[x + y * width].P[facing] * P_TURN_RIGHT;
				Element(x, y).P[leftDir] += cached[x + y * width].P[facing] * P_TURN_LEFT;
				Element(x + forwardChangeX, y  + forwardChangeY).P[facing] += cached[x + y * width].P[facing] * P_FORWARD;

			}
		}
	}
}
void Matrix::UpdateOnSmell(Tile * cached){
	if((event & SMELL) != 0){
		Prepare(cached);
		float sum = 0;
		for(int y = player->GetRow() - 1; y < player->GetRow() + 2; ++y){
			if(y < 0 || y > height - 1)
				continue;
			for(int x = player->GetCol() - 1; x < player->GetCol() + 2; ++x){
				if(x < 0 || x > width - 1)
					continue;
				for(int i = 0; i < TILE; ++i){
					sum += cached[y + x * height].P[i];
				}
			}
		}
		for(int y = player->GetRow() - 1; y < player->GetRow() + 2; ++y){
			if(y < 0 || y > height - 1)
				continue;
			for(int x = player->GetCol() -1; x < player->GetCol() + 2; ++x){
				if(x < 0 || x > width - 1)
					continue;
				for(int i = 0; i < TILE; ++i){
					Element(y, x).P[i] = cached[y + x * height].P[i] / sum;
				}
			}
		}
	}
}
void Matrix::UpdateOnHear(Tile * cached){

}
void Matrix::UpdateOnFeel(Tile * cached){

}