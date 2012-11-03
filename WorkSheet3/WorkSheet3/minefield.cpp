#include "minefield.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

//default constructor... doesnt do much
minefield::minefield()
{
  srand(time(NULL));
  
  width = -1;
  height = -1;
  num_mines = -1;
  num_revealed = 0;
}

//useful constructor, generates minefield and places mines
minefield::minefield(int w, int h, int m)
{
  srand(time(NULL));
  
  width = w;
  height = h;
  num_mines = m;
  num_revealed = 0;
  
  field.resize(w);
  for (int i=0; i<w; ++i)
    {
      field[i].resize(h);
    }
  
  GenerateField();
}

//destructor
minefield::~minefield()
{
  for (int i=0; i<field.size(); ++i)
    {
      field[i].clear();
    }	
}


void minefield::GenerateField()
{
  PlaceMines();
}

//places mines randomly and calculates the adjacent tile values
void minefield::PlaceMines()
{
  int num_tiles = width*height;
  if (num_mines > num_tiles)
    {
      cout << "ERROR: More mines than can possibly be placed\n";
      return;
    }
  
  for (int i=0; i<num_mines; ++i)
    {
      bool placed = false;
      do
	{
	  int pos = rand()%num_tiles;
	  
	  // pos = row*|col| + col
	  // row = pos % col
	  int row = pos / width;
	  int col = pos - (row*width);
	  
	  //setting us up the bomb!
	  if (field[col][row] != BOMB)
	    {
	      field[col][row] = BOMB;
	      //left
	      if (col-1 >= 0)
		{ 
		  //left
		  if (field[col-1][row] != BOMB)
		    ++field[col-1][row];
		  if (row+1 < height)
		    {
		      //down left
		      if (field[col-1][row+1] != BOMB)
			++field[col-1][row+1];
		    }	
		  if (row-1 >= 0)
		    {
		      // up left					
		      if (field[col-1][row-1] != BOMB)
			++field[col-1][row-1];
		    }					
		}
	      //right
	      if (col+1 < width)
		{
		  // right
		  if (field[col+1][row] != BOMB)
		    ++field[col+1][row];
		  //down right
		  if (row+1 < height)
		    if (field[col+1][row+1] != BOMB)
		      ++field[col+1][row+1];
		  // up right					
		  if (row-1 >= 0)
		    if (field[col+1][row-1] != BOMB)
		      ++field[col+1][row-1];
		}
	      //down
	      if (row+1 < height)
		if (field[col][row+1] != BOMB)
		  ++field[col][row+1];
	      // up
	      if (row-1 >= 0)
		if (field[col][row-1] != BOMB)
		  ++field[col][row-1];
	      placed = true;
	    }
	} while (!placed);
    }
}

//select a tile, will you die?
int minefield::RevealTile(int col, int row)
{
  //you already looked here, dummy!!
  if (field[col][row] & KNOWN)
    return field[col][row] & BOMB;
  
  if (field[col][row] == BOMB) {
    cout << "GAME OVER!!\n";
  }
  
  int value = field [col][row];
  field[col][row] |= KNOWN;
  
  ++num_revealed;
  
  if ((height*width - num_mines) == num_revealed && 
      value != BOMB)
    {
      value = WIN;
    }
  
  return value;
}

//outputs the field
// X for unrevealed, numbers for known, B for bombs
void minefield::Print()
{
  cout << "Num revealed: " << num_revealed 
       << " Num Mines: " << num_mines << endl;
  
  //for (int row=0; row<height; ++row)
  for (int row=height-1; row>=0; --row)
    {
      for (int col=0; col<width; ++col)
      //for (int col=width-1; col>=0; --col)
	{
	  //row numbers
	  if (col == 0)
	  {
	    cout << setw(3) << row << " |";
	  }

	  //output coordinates
	  if (field[col][row] & KNOWN)
	  {
	    int value = int(field[col][row] & BOMB);
	    if (value == BOMB)
	      cout << setw(3) << "*";
	    else		
	      cout << setw(3) << value;
	  }
	  else
	  {
	    cout << setw(3) << ".";
	  }
	}
      cout << endl;


      //column numbers
      if (row == 0)
      {
	cout << setw(5) << " ";
	for (int c=0; c<width; ++c)
	{
	  cout << setw(3) << "___";
	}

	cout << endl << setw(5) << " ";
	for (int c=0; c<width; ++c)
	{
	  cout << setw(3) << c ;
	}
	cout << endl << "   " << endl;
      }


    }
  
  cout << endl;
}
