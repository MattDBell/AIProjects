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

class UnResolved : public LogicalStatement{
public:
	virtual RESULT Resolve(){
		return UNRESOLVED;
	}
}

class False : public LogicalStatement{
public:
	virtual RESULT Resolve(){
		return FALSE;
	}
};


class True : public LogicalStatement{
public:
	virtual RESULT Resolve(){
		return TRUE;
	}
};


class Not : public LogicalStatement{
	LogicalStatement ** statement;
public:
	void SetStatement(LogicalStatement ** statement){
		this->statement = statement;
	}
	virtual RESULT Resolve(){
		if(!statement || !*statement)
			return UNRESOLVED;
		switch(*statement->Resolve()){
			case TRUE:
				return FALSE;
			case FALSE:
				return TRUE;
			default:
				return UNRESOLVED;
		}
	}
}



class And : public LogicalStatement{
	LogicalStatement *** statements;
	uint8_t numStatements;
public:
	And(uint8_t numStatements)
		:numStatements(numStatements)
	{
		statements = new LogicalStatemnt**[numStatements];
	}
	void AddStatement(uint8_t slot, LogicalStatement** statement){
		if(slot < numStatements)
			statements[slot] = statement;
	}
	virtual RESULT Resolve(){
		RESULT res = TRUE;
		for(uint8_t i = 0; i < numStatements; ++i){
			RESULT curRes = *(statements[i])->Resolve();
			if(curRes == UNRESOLVED)
				res = UNRESOLVED;
			if(curRes == FALSE){
				res = FALSE;
				break;
			}
		}
		return res;
	}
};




class Or : public LogicalStatement{
	LogicalStatement ***statements;
	uint8_t numStatements;
public:
	Or(uint8_t numStatements)
		: numStatements(numStatements){
		statements = new LogicalStatement**[numStatements];
	}
	virtual RESULT Resolve(){
		RESULT res = FALSE;
		for(uint8_t i = 0; i < numStatements; ++i){
			if(!statements[i] || !*statements[i]){
				res = UNRESOLVED;
				continue;
			}
			RESULT curRes = *(statements[i])->Resolve();
			if(curRes == UNRESOLVED)
				res = UNRESOLVED;
			if(curRes == TRUE){
				res = FALSE;
				break;
			}
		}
		return res;
	}
}
class NOfM : public LogicalStatement{
	LogicalStatement ***statements;
	uint8_t total;
	uint8_t numTrue;
public:
	NofM(uint8_t total, uint8_t numTrue)
		:total(total), numTrue(numTrue)
	{
		statements = new LogicalStatement**[total];
	}
	void AddStatement(uint8_t slot, LogicalStatement** statement){
		if(slot < total)
			statements[slot] = statement;
	}
	virtual RESULT Resolve(){
		uint8_t count;
		bool noUnresolves;
		for(int i = 0; i < total; ++i){
			if(!statements || !*statements || !**statements){
				switch(*(statements[i])->Resolve){
					case TRUE:
						count++;
						break;
					case FALSE:
						break;
					case UNRESOLVED:
						noUnresolves = false;
						break;
				}
			}
		}
		if(count == numTrue)
			return FALSE;
		if(noUnresolves)
			return TRUE;
		return UNRESOLVED;
	}
};
#endif//LOGICALSTATEMENT_H