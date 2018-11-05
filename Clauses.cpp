#include "Clauses.h"

Clause::Clause(std::vector<int> &literals)
{
    this->literals = literals;
    if (literals.size() > 0)
        lastLiteral = literals[literals.size() - 1];
    lastVarIndex = std::abs(lastLiteral);
    if (literals.size() > 1)
        lastButOneVarIndex = std::abs(literals[literals.size() - 2]);
}

bool Clause::isBlack()
{
    for (unsigned int i = 0; i < literals.size(); i++)
    {
        if (literals[i] > 0)
            return false;
    }
    return true;
}

bool Clause::subsumedBy(std::vector<bool> &counter)
{
    for (unsigned int i = 0; i < literals.size(); i++)
    {
        if (counter.at(std::abs(literals[i])) != (literals[i] > 0))
            return false;
    }
    return true;
}

Clause *Clause::union_v2(Clause *a, Clause *b, int uptoIndex)
{
    int aIndex = 0;
    int bIndex = 0;

    std::vector<int> aLiterals = a->literals;
    std::vector<int> bLiterals = b->literals;
    std::vector<int> copyBuffer(aLiterals.size() + bLiterals.size());
    int index = 0;
    int aLit = aLiterals[aIndex];
    int bLit = bLiterals[bIndex];

    while (aLit != -bLit)
    {
        if (aLit == bLit)
        {
            copyBuffer[index] = aLit;
            aIndex++;
            bIndex++;
            if (aIndex == aLiterals.size() || bIndex == bLiterals.size())
                break;
            aLit = aLiterals[aIndex];
            bLit = bLiterals[bIndex];
        }
        else if (std::abs(aLit) < std::abs(bLit))
        {
            copyBuffer[index] = aLit;
            aIndex++;
            if (aIndex == aLiterals.size())
                break;
            aLit = aLiterals[aIndex];
        }
        else
        {
            copyBuffer[index] = bLit;
            bIndex++;
            if (bIndex == bLiterals.size())
                break;
            bLit = bLiterals[bIndex];
        }
        index++;
    }
    if (aIndex >= aLiterals.size())
    {
        while (std::abs(copyBuffer[index]) <= uptoIndex)
        {
            index++;
            copyBuffer[index] = bLiterals[bIndex];
            bIndex++;
        }
    }
    else if (bIndex >= bLiterals.size())
    {
        while (std::abs(copyBuffer[index]) <= uptoIndex)
        {
            index++;
            copyBuffer[index] = aLiterals[aIndex];
            aIndex++;
        }
    }

    std::vector<int> literals(index);
    for (int i = 0; i < index; i++)
        literals[i] = copyBuffer[i];

    return new Clause(literals);

}

void ClauseList::insert(Clause *c)
{
    int i = 0;
    while (i < clauseList.size() && std::abs(clauseList[i]->literals[0]) < std::abs(c->literals[0]))
        i++;

    clauseList.push_back(c);
}

ClauseSet::ClauseSet(int numberOfVariables, int indexOfLastVariableOfBestBlackClause, Clause *bestBlackClause,
                     std::vector<ClauseList*> clauseListOrderdByLastVarIndexNeg, std::vector<ClauseList*> clauseListOrderdByLastVarIndexPos,
                     std::vector< std::vector<int> *> cs)
{
    this->numberOfVariables = numberOfVariables;
    this->indexOfLastVariableOfBestBlackClause = indexOfLastVariableOfBestBlackClause;
    this->bestBlackClause = bestBlackClause;
    this->clauseListOrderdByLastVarIndexNeg = std::vector<std::vector<Clause*>>(this->numberOfVariables + 1);

    for (int i = 0; i <= numberOfVariables; i++)
    {
        int length = clauseListOrderdByLastVarIndexNeg[i]->clauseList.size();
        this->clauseListOrderdByLastVarIndexNeg[i] = std::vector<Clause*>(length);

        for (int j = 0; j<length; j++)
            this->clauseListOrderdByLastVarIndexNeg[i][j] = clauseListOrderdByLastVarIndexNeg[i]->clauseList.at(j);
    }

    this->clauseListOrderdByLastVarIndexPos = std::vector<std::vector<Clause*>>(this->numberOfVariables + 1);
    for (int i = 0; i <= numberOfVariables; i++)
    {
        int length = clauseListOrderdByLastVarIndexPos[i]->clauseList.size();
        this->clauseListOrderdByLastVarIndexPos[i] = std::vector<Clause*>(length);
        for (int j = 0; j<length; j++)
            this->clauseListOrderdByLastVarIndexPos[i][j] = clauseListOrderdByLastVarIndexPos[i]->clauseList.at(j);
    }
    this->cs = cs;

    setEffectedClauses();
    setLastUnion();
}

int ClauseSet::addEffectedClause(Clause *c)
{
    int index = c->lastVarIndex;

    if (c->lastLiteral == -index)
        effectedClauses[index][0] = c;
    else
        effectedClauses[index][1] = c;
    return index;
}

void ClauseSet::addLastUnion_v3(Clause *c)
{
    if (maxNumberOfLastUnion <= 0)
        return;

    int index = c->lastVarIndex;
    if (c->literals.size() == index)
        return;
    if (c->lastLiteral > 0)
    {
        if (lastUnionPos[index].clauseList.size() == maxNumberOfLastUnion)
            lastUnionPos[index].clauseList.erase(lastUnionPos[index].clauseList.begin()+(maxNumberOfLastUnion - 1));
        lastUnionPos[index].insert(c);
    }
    else
    {
        if (lastUnionNeg[index].clauseList.size() == maxNumberOfLastUnion)
            lastUnionNeg[index].clauseList.erase(lastUnionNeg[index].clauseList.begin()+(maxNumberOfLastUnion - 1));
        lastUnionNeg[index].insert(c);
    }
}

void ClauseSet::setEffectedClauses()
{
    effectedClauses = std::vector<std::vector<Clause*>>(this->numberOfVariables + 1);
    for (int i = 0; i < effectedClauses.size(); i++)
        effectedClauses[i] = std::vector<Clause*>(2);
}

void ClauseSet::setLastUnion()
{
    const int numOfVariables = this->numberOfVariables + 1;
    lastUnionNeg = ClauseList[numOfVariables];
    lastUnionPos = new ClauseList[numOfVariables];

    for (int i = 0; i <= numberOfVariables; i++)
    {
        lastUnionNeg[i] = ClauseList();
        lastUnionPos[i] = ClauseList();
    }
}
