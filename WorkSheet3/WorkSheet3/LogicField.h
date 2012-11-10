#ifndef LOGICFIELD_H
#define LOGICFIELD_H

#include "minefield.h"
#include "LogicalStatement.h"
class LogicField
{
	minefield* m_field;
	LogicalStatement** l_field;
	bool* checked;
	int width, height;
public:
	LogicField(minefield* field)
		:m_field(field){
		width = field->width;
		height = field->height;
		l_field = new LogicalStatement*	[width * height];
		checked = new bool				[width * height];
		memset(l_field, 0, sizeof(LogicalStatement*) * width * height));
		memset(checked, 0, sizeof(bool) * width *height);
	}
	void SpotToCoords(int spot, int * x, int * y){
		x = spot % width;
		y = spot / width;
	}

	void ChangeBoard(int spot, int gotten){
		int x, y;
		SpotToCoords(spot, &x, &y);
		if(
		for(int i = 0; i < 8; ++i){
			
			CoordsToSpot(x, y);
		}
		
	}

	void step(){
		int bombs = 0;
		for(int spot = 0; spot < height * width; ++spot){
			if(l_field && l_field[spot] && 
				l_field[spot]->Resolve() == FALSE && !checked[spot]){
				checked[spot] = true;
				int gotten = m_field->RevealTile(spot % width, spot / width);
				ChangeBoard(spot, gotten);
			}else	if(l_field && l_field[spot] && 
				l_field[spot]->Resolve() == TRUE){
				bombs++;
			}
		}
		printf("%d bombs found...?", bombs);
	}
	
	~LogicField(){
		delete[] l_field;
		delete[] checked;
	}

};

#endif