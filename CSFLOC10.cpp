#include "Solver.h"
#include "DIMACSReader.h"
#include <time.h>

int main(int argc, char *argv[])
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
	std::string fileName = "";
	if (arguments.size() != 1)
	{
		fileName = "uf75-02.cnf";
    }
	else fileName = arguments[0];

	printf(fileName.c_str());

	clock_t startTime = clock();
	//long startTime = System.currentTimeMillis();

	//DIMACSReader dr(fileName);
	HighLevelReader reader(new DIMACSReader(fileName));

	//printf("valami");
	ClauseSet *S = reader.getClauseSet();

	//printf("!!!main numberofVariables: %d\n", S->numberOfVariables);

	std::vector< std::vector<int> *> cs = reader.cs;

	Solver *solver = new Solver();

	//printf("solver létrehozva");
	std::vector<bool> *solution = solver->CSFLOC_v7(S);

	printf("\nCPU time taken: %.2fs\n", (double)(clock() - startTime) / CLOCKS_PER_SEC);

	if (solution != nullptr)
	{
		printf("The solution is: ");
		for (int i = 1; i<S->numberOfVariables + 1; i++)
		{
			int var = i;
			if (solution->at(var))
                printf("%d ", i);
            else
                printf("%d ", -i);
		}
		printf("\n");
		printf("Because the following full length clause is not subsumed:\n");

		for (int i = 1; i<S->numberOfVariables + 1; i++)
		{
			int var = i;
			if (solution->at(var))
                printf("%d ", -i);
            else
                printf("%d ", i);
		}
		printf("\n\n");

		CheckSolution check;

		if (!check.SimpleCheckSolution(cs, solution))
			printf("The solution is checked to be sure, but it is NOT A SOLUTION! There must be a bug:(\n");
	}
	else printf("There is no solution\n");
}

