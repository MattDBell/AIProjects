#ifndef LOGICALSTATEMENT_H
#define LOGICALSTATEMENT_H

enum RESULT {
	FALSE,
	TRUE,
	UNRESOLVED,
	TOTAL;
};

class LogicalStatement{
public:
	virtual RESULT Resolve() = 0;
};

class AndStatement : public LogicalStatement{
	LogicalStatement ** statements;
	uint8_t numStatements;
public:
	AndStatement(uint8_t numStatements)
		:numStatements(numStatements)
	{
		statements = new LogicalStatemnt*[numStatements];
	}
	void AddStatement(uint8_t slot, LogicalStatement* statement){
		if(slot < numStatements)
			statements[slot] = statement;
	}
	virtual RESULT Resolve(){
		RESULT res = TRUE;
		for(uint8_t size = 0; size < numStatements; ++size){
			RESULT curRes = statements[0]->Resolve();
			if(curRes == FALSE)
				res = FALSE;
			if(curRes == UNRESOLVED){
				res = UNRESOLVED;
				break;
			}
		}
		return res;
	}
};


#endif//LOGICALSTATEMENT_H