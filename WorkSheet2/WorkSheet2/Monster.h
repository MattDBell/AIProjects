/*
  This class is used to hold the movement behaviors of the monster and
  actually move the monsters as desired
*/

#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

class Matrix;
enum {UP, DOWN, LEFT, RIGHT, TILE};

#define P_FORWARD    .50f
#define P_TURN_LEFT  .25f
#define P_TURN_RIGHT .25f


class Monster
{
 public:
  Monster(int c, int r, int player);
  ~Monster();
  void Update();
  void Move(Matrix * mat);
  int GetRow();             //accessor, get row of monster
  int GetCol();             //accessor, get column of monster
  int GetDir();             //accessor, get direction of monster

 private:
  //location and direction of monster
  int row;
  int col;
  int direction;

  //dimensions of map
  int width;
  int height;

  //transition model
  float p_forward;
  float p_turn_left;
  float p_turn_right;

};


#endif
