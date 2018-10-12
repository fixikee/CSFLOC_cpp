#include "Solver.h"

int Solver::usingBestClause_v4(ClauseSet *S, int index)
{
    Clause *a = S->effectedClauses[index][1];

    if (a != nullptr && a->subsumedBy(Solver::counter))
        return index;


    std::vector<Clause*> clauseList = S->clauseListOrderdByLastVarIndexPos[index];
    for (Clause *c : clauseList)
    {
        if (c->subsumedBy(Solver::counter))
            return S->addEffectedClause(c);
    }

    for (int i = 0; i < S->lastUnionPos[index]->clauseList.size(); i++)
    {
        Clause *c = S->lastUnionPos[index]->clauseList.at(i);
        if (c->subsumedBy(Solver::counter))
            return S->addEffectedClause(c);
    }

    for (index++; index < S->numberOfVariables + 1; index++)
    {
        Clause *b = S->effectedClauses[index][0];
        if (b != nullptr && b->subsumedBy(Solver::counter))
            return index;

        clauseList = S->clauseListOrderdByLastVarIndexNeg[index];
        for (Clause *c : clauseList)
        {
            if (c->subsumedBy(Solver::counter))
                return S->addEffectedClause(c);
        }

        for (int i = 0; i < S->lastUnionNeg[index]->clauseList.size(); i++)
        {
            Clause *c = S->lastUnionNeg[index]->clauseList.at(i);

            if (c->subsumedBy(Solver::counter))
                return S->addEffectedClause(c);
        }
    }
    return 0;
}

int Solver::increaseCounter_v4(ClauseSet *S, int index)
{
    Clause *unio = nullptr;

    if (Solver::counter.at(index))
    {
        Clause *a = S->effectedClauses[index][0];
        Clause *b = S->effectedClauses[index][1];
        counter[index] = false;
        index--;
        unio = Clause::union_v2(a, b, index);
        S->addLastUnion_v3(unio);
    }

    while (counter.at(index))
    {
        Clause *a = S->effectedClauses[index][0];
        counter[index] = false;
        index--;

        unio = Clause::union_v2(a, unio, index);
    }

    if (unio != nullptr)
    {
        while (index>unio->lastVarIndex)
        {
            counter[index] = false;
            index--;
        }
        index = S->addEffectedClause(unio);

        return increaseCounter_v4(S, index);
    }
    counter[index] = true;

    return index;
}

std::vector<bool> *Solver::CSFLOC_v7(ClauseSet *S)
{
    Solver::counter = std::vector<bool>(S->numberOfVariables + 1);
    int index = S->indexOfLastVariableOfBestBlackClause;

    if (index == S->numberOfVariables + 1)
    {
        counter.flip();
        return &counter;
    }

    index = S->addEffectedClause(S->bestBlackClause);
    counter[index] = true;

    while (index > 0)
    {
        index = usingBestClause_v4(S, index);
        if (index == 0)
        {
            counter.flip();
            return &counter;
        }
        index = increaseCounter_v4(S, index);
    }
    return nullptr;
}
