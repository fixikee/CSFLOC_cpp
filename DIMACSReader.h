#ifndef DIMACSREADER_H
#define DIMACSREADER_H

#include "Clauses.h"
#include <vector>
#include <string>

class DIMACSReader {public:
	int numberOfVariables = 0;
	int numberOfClauses = 0;
	std::vector< std::vector<int>* > cs;
	std::vector<int> copyBuffer;

	DIMACSReader(const std::string fileName);

private:
	void readPLine(const std::string cnfClause);
	void initDataStructure();
	void addCNFClause(const std::string cnfClause);
};


class HighLevelReader {public:
	int numberOfVariables = 0;
	int indexOfLastVariableOfBestBlackClause = 0;
	std::vector<ClauseList*> clauseListOrderdByLastVarIndexNeg;
	std::vector<ClauseList*> clauseListOrderdByLastVarIndexPos;
	Clause *bestBlackClause;
	std::vector< std::vector<int>* > cs;

	HighLevelReader(DIMACSReader *dr);

	virtual ClauseSet *getClauseSet();

private:
	void orderLiterals();
	void initDataStructure();
	void fillDataStructure();
};

class CheckSolution {
public:
	virtual bool SimpleCheckSolution(std::vector< std::vector<int> *> cs, std::vector<bool> *solution);
};

#endif // DIMACSREADER_H
