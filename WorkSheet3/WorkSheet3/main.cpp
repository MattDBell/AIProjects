/*
  Here is where you will be injecting your code. You may have your own class to
  process your knowledge base, but the only thing you will really be 
  interfacing in the startup code is in the main function
*/

#include <iostream>
#include "minefield.h"

//Bomb value is set to 127... because you cannot have that many adjcent bombs
//#define BOMB 127

using namespace std;

int main()
{
  minefield *minesweeper;
  
  int w,h,m;
  int x,y;
  int tile_value = 0;
  
  cout << "Enter dimensions of field: ";
  cin >> w >> h;
  
  cout << "Enter number of mines: ";
  cin >> m;
  
  minesweeper = new minefield(w,h,m);
  
  minesweeper->Print();
  
  while (tile_value != BOMB && tile_value != WIN)
    {
      cout << "Choose coordinate (x,y) to reveal: ";
      
      // ** replace these x,y input values with whatever your 
      //  logic resolver would decide
      cin >> x >> y;
      
      tile_value = minesweeper->RevealTile(x,y);
      // ** tile returns [0,8] for number of adjacent bombs, returns 127
      //  if current tile is a bomb

      // ** use the (x,y) coordinate and the information you get from the 
      //  reveal to build up your knowledge base and come up with save 
      //  coordinates to expand on
      
      minesweeper->Print();
    }
  
  if (tile_value == WIN)
    cout << "YOU WINNER!!\n";
  else if (tile_value == BOMB)
    cout << "YOUR HEAD ASPLODE!!\n";
  
  return 0;
}

