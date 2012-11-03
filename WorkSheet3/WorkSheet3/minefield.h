#ifndef _MINEFIELD_H_
#define _MINEFIELD_H_

#include <vector>

using namespace std;

// unsigned  [0, 255],
// signed [-128, 127]
enum {WIN=-1, BOMB= 255 & ~(1<<7),KNOWN=1<<7};

class minefield
{
public:
  minefield();
  minefield(int w, int h, int m);
  ~minefield();
  
  int RevealTile(int c, int r);
  void Print();
  
 private:
  void GenerateField();
  void PlaceMines();
  
  vector<vector<char> > field;
  int width;
  int height;
  int num_mines;
  int num_revealed;
};



#endif
