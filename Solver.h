#ifndef SOLVER_H
#define SOLVER_H

#include "Clauses.h"
#include <vector>
#include <stdio.h>

class Solver {
public:
    std::vector<bool> counter;
    virtual std::vector<bool> *CSFLOC_v7(ClauseSet *S);

private:
	int usingBestClause_v4(ClauseSet *S, int index);
	int increaseCounter_v4(ClauseSet *S, int index);
};

#endif // SOLVER_H
