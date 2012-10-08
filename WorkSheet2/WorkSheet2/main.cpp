/*

*/

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Matrix.h"

using namespace std;

int main()
{
  srand( time(NULL) );

  Matrix m(10,10);
  char player_move;

  cout << "Controls:\n"
       << "w: move forward\n"
       << "a: turn left\n"
       << "d: turn right\n";

  cout << "\tGet to the goal without getting eaten!!\n\n";

  do
  {
    //uncomment to see the actual monster's movement, and if you want to
    // play the game
    m.Reveal();

    //uncomment to see the probability the monster is at any given location
    //m.Print();

    cout << endl;
    cin >> player_move;

  } while( m.Iterate(player_move) );

  cout << "Game Over YEAAAAAAAAAAHHHHH!!!" << endl;

  return 0;
}
