#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>
#include <time.h>
#include <stdlib.h>
#include "Monster.h"

using namespace std;

enum {SMELL=1, HEAR=2, FEEL=4};

//each tile on the map holds probabilities... up to you to fill them
struct Tile
{
  Tile()
  {
    P[UP]=0;
    P[DOWN]=0;
    P[LEFT]=0;
    P[RIGHT]=0;
    P[TILE]=0;
  }

  float P[5];
};


/*

*/
class Matrix
{
 public:
  Matrix();
  Matrix(int,int);     //width,height
  ~Matrix();

  bool Iterate(char);  //move the monster and yourself
  void UpdateEvents();
  void Print();        //print the map of probabilities
  void Reveal();       //print the map showing the monster!
  void PrintEvents();
  void ProbabilityUpdate(); // the function you will be working on

  Tile& Element(int, int); //reference map by (col, row)

 private:
  int event;

  int width;
  int height;

  vector<vector<Tile> > map;

  Monster *monster;
  Monster *player;
};

#endif
