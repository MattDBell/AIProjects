enum RESULT{
	FALSE,
	TRUE,
	CANNOT_RESOLVE,
	TOTAL;
}

class LogicalStatement{
	virtual void Resolve() = 0;
};

class AndStatement: public LogicalStatement{
	LogicalStatement * statements[2];
	virtual void Resolve(){
		Result results[2];
		results[0] = statements[0]->Resolve();
		results[1] = statements[1]->Resolve();

		if(results[0] == CANNOT_RESOLVE ||results[1] == CANNOT_RESOLVE)
			return CANNOT_RESOLVE;
		
	}
};