#include "Solver.h"

int Solver::usingBestClause_v4(ClauseSet *S, int index)
{
    //printf("usingBestClause belépés\n");
	Clause *a = S->effectedClauses[index][1];

	if (a != nullptr && a->subsumedBy(Solver::counter))
    {
        //printf("\n!!!!! a nem nullptr");
        return index;
    }
    //printf("usingBestClause Clause *a\n");
    //printf("usingBestClause elsõ if után\n");
	std::vector<Clause*> clauseList = S->clauseListOrderdByLastVarIndexPos[index];
	for (Clause *c : clauseList)	{		if (c->subsumedBy(Solver::counter))			return S->addEffectedClause(c);	}     //printf("usingBestClause elsõ ciklus után\n");
	for (int i = 0; i < S->lastUnionPos[index]->clauseList.size(); i++)	{
		Clause *c = S->lastUnionPos[index]->clauseList.at(i);
		if (c->subsumedBy(Solver::counter))			return S->addEffectedClause(c);
	}     //printf("usingBestClause második ciklus\n");
	for (index++; index < S->numberOfVariables + 1; index++)
	{
        //printf("nagy ciklus i:%d\n", index);
		Clause *b = S->effectedClauses[index][0];
		if (b != nullptr && b->subsumedBy(Solver::counter))			return index;
        //printf("elsõ if utan\n");
		clauseList = S->clauseListOrderdByLastVarIndexNeg[index];
		//printf("belsõ ciklus1 elott\n");
		for (Clause *c : clauseList)		{
		    //printf("belsp ciklusban\n");			if (c->subsumedBy(Solver::counter))				return S->addEffectedClause(c);		}        //printf("belsõ ciklus1\n");

		for (int i = 0; i < S->lastUnionNeg[index]->clauseList.size(); i++) {
			Clause *c = S->lastUnionNeg[index]->clauseList.at(i);
			if (c->subsumedBy(Solver::counter))				return S->addEffectedClause(c);
		}
	}
	return 0;
}

int Solver::increaseCounter_v4(ClauseSet *S, int index){
	Clause *unio = nullptr;

	//printf("!!!!!!!iCv4 S.numberOfVariables: %d\n", S->numberOfVariables);
	//printf("%d ", index);
	if (Solver::counter.at(index))	{
	    //printf("ide jutottam icV4 new elott\n");
	    //printf("\n S.addeffclause size: %d\n", S->effectedClauses.at(index).size());
	    //if(S->effectedClauses.at(index).at(0) == nullptr)
            //printf("!!! nullptr lett");
		Clause *a = S->effectedClauses[index][0];
		/*for(int i = 0; i< a->literals.size(); i++){
            printf("clause a: %d\n", a->literals.at(i));
		}
		printf("icV4 clause a\n");*/
		Clause *b = S->effectedClauses[index][1];
		//printf("icV4 clause b\n");
		counter[index] = false;
		index--;        //printf("icV4 unio elott\n");		unio = Clause::union_v2(a, b, index);
		//printf("icV4 unio\n");
		S->addLastUnion_v3(unio);
        //printf("ide jutottam  icV4 elso if vege\n");
	}
	while (counter.at(index))	{
	    //printf("ide jutottam icV4 while elso\n");
		Clause *a = S->effectedClauses[index][0];
		counter[index] = false;
		index--;
		unio = Clause::union_v2(a, unio, index);
	}
	//printf("while utan\n");
	if (unio != nullptr)	{
		while (index>unio->lastVarIndex)
		{
			counter[index] = false;			index--;
		}
		index = S->addEffectedClause(unio);

		return increaseCounter_v4(S, index);
	}
	counter[index] = true;

	return index;
}

std::vector<bool> *Solver::CSFLOC_v7(ClauseSet *S)
{
    //printf("\nnumber of variables: %d\n", S->numberOfVariables);
	Solver::counter = std::vector<bool>(S->numberOfVariables + 1);
	//printf("counter meret: %d\n", counter.size());
	//printf("csflocv7 counter deklaralva");
	int index = S->indexOfLastVariableOfBestBlackClause;
	if (index == S->numberOfVariables + 1)
	{
		counter.flip();
		//counter.flip(0, counter.size() - 1);
		return &counter;
	}
	index = S->addEffectedClause(S->bestBlackClause);
	counter[index] = true;
	while (index > 0)	{
	    //printf("\ncsfloc v7 beléptem a whileba \n");
		index = usingBestClause_v4(S, index);
        //printf("usingBestClausev4 hívása\n");
		if (index == 0)
		{
			counter.flip();
			return &counter;
		}
		index = increaseCounter_v4(S, index);
		//printf("incraseCounterv4 hívása\n");
	}
	return /*std::vector<bool>()*/ nullptr;
}
