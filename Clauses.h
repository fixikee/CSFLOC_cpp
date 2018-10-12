#ifndef CLAUSES_H
#define CLAUSES_H
#include <vector>
#include <cstdlib>
#include <stdio.h>

class Clause {
public:
	std::vector<int> literals;
	int lastLiteral;
	int lastVarIndex;
	int lastButOneVarIndex;

	Clause(std::vector<int> &literals);

	virtual bool isBlack();
	virtual bool subsumedBy(std::vector<bool> &counter);
	static Clause *union_v2(Clause *a, Clause *b, int uptoIndex);
};

class ClauseList {
public:
	std::vector<Clause*> clauseList;
	virtual void insert(Clause *c);
};

class ClauseSet {
public:
	int numberOfVariables = 0;
	int indexOfLastVariableOfBestBlackClause = 1;
	int maxNumberOfLastUnion = 3;
	Clause *bestBlackClause;
	std::vector< std::vector<Clause*> > clauseListOrderdByLastVarIndexNeg;
	std::vector< std::vector<Clause*> > clauseListOrderdByLastVarIndexPos;
	std::vector< std::vector<Clause*> > effectedClauses;
	std::vector<ClauseList*> lastUnionNeg;
	std::vector<ClauseList*> lastUnionPos;
	std::vector< std::vector<int> *> cs;

	ClauseSet(int numberOfVariables, int indexOfLastVariableOfBestBlackClause, Clause *bestBlackClause,
		std::vector<ClauseList*> clauseListOrderdByLastVarIndexNeg, std::vector<ClauseList*> clauseListOrderdByLastVarIndexPos,
		std::vector< std::vector<int> *> cs);

	virtual int addEffectedClause(Clause *c);
	virtual void addLastUnion_v3(Clause *c);
	virtual void setEffectedClauses();
	virtual void setLastUnion();
};

#endif // CLAUSES_H
