#include "DIMACSReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

DIMACSReader::DIMACSReader(const std::string &fileName)
{
	try
	{
		std::ifstream infile(fileName);
		//std::string clause = infile->readLine();
		std::string clause;
		std::getline(infile, clause);
        /*std::string content( (std::istreambuf_iterator<char>(infile) ),
                            (std::istreambuf_iterator<char>()    ) );

        std::cout<< clause;*/
		while (clause[0] != 'p')
		{
		    //printf("dimacstry 2");
			std::getline(infile, clause);
			//std::cout << clause<<std::endl;
		}
		readPLine(clause);
		initDataStructure();
		std::getline(infile, clause);
		while (clause != "")		{			addCNFClause(clause);
			std::getline(infile, clause);
			//std::cout << clause<<std::endl;
		}
		//printf(clause.c_str());
		//delete infile
	}
	catch (const std::exception &exc)
	{
		std::cerr << exc.what();
	}
}

void DIMACSReader::readPLine(const std::string &cnfClause)
{
	int i1 = 6;
	int i2 = (int)cnfClause.find(" ", i1);
	//numberOfVariables = Integer.parseInt(cnfClause.substring(i1, i2));
	numberOfVariables = std::stoi(cnfClause.substr(i1, i2 - i1));

	while (cnfClause[i2] == ' ')
		i2++;

	int i3 = (int)cnfClause.find(" ", i2);
	if (i3 == -1)
		i3 = cnfClause.length();

	numberOfClauses = std::stoi(cnfClause.substr(i2, i3 - i2));
}

void DIMACSReader::initDataStructure()
{
	copyBuffer = std::vector<int>(numberOfVariables);
}

void DIMACSReader::addCNFClause(const std::string &cnfClause)
{
    //std::cout << cnfClause;
	if (cnfClause.length() == 0 || cnfClause[0] == '0' || cnfClause[0] == 'c' || cnfClause[0] == '%')
		return;

	int i = 0;
	int db = 0;
	unsigned int i1 = 0;
	while (i1 < cnfClause.length()) {
		int i2 = cnfClause.find(" ", i1);
		if (i1 == i2)
		{
			i1++;
			continue;
		}
		if (i2 == -1)
			break;

		std::string lit = cnfClause.substr(i1, i2 - i1);
		i1 = i2;
		int literal = std::stoi(lit);
		if (literal == 0)
			break;

		copyBuffer[i] = literal;
		i++;
		db++;
		//printf("i:%d, ",i);
	}
	//printf("db: %d ",db);
	//int[] literals = new int[i];
	std::vector<int> *literals = new std::vector<int>();

    for(int index=0; index<i; index++)
        literals->push_back(copyBuffer[index]);

    //for(int i = 0; i<literals.size(); i++)
        //std::cout<<literals[i]<<" ";

    cs.push_back(literals);

    //for(int i = 0; i< cs.size(); i++){
        //for(int j = 0; j<cs.at(i)->size(); j++);
            //printf("%d, ", cs.at(i)->at(j));
    //}
}

HighLevelReader::HighLevelReader(DIMACSReader *dr)
{
	numberOfVariables = dr->numberOfVariables;
	/*for(int i = 0; i < dr->cs.size(); i++) {
        for(int j = 0; j < dr->cs.at(i)->size(); j++) {
            std::cout << dr->cs.at(i)->at(j) << " ";
        }
	}*/
	cs = dr->cs;
	orderLiterals(cs);
	initDataStructure();
	fillDataStructure();
    //printf("highlevelreader");
}

ClauseSet *HighLevelReader::getClauseSet()
{
	//gyanús
	return new ClauseSet(numberOfVariables,
		indexOfLastVariableOfBestBlackClause,
		bestBlackClause,
		clauseListOrderdByLastVarIndexNeg,
		clauseListOrderdByLastVarIndexPos,		cs);
}

void HighLevelReader::fillDataStructure()
{
	for (int i = 0; i < cs.size(); i++) {
		Clause *newClause = new Clause(*cs.at(i));
		if (newClause->lastLiteral > 0)			clauseListOrderdByLastVarIndexPos[newClause->lastVarIndex]->insert(newClause);
		else clauseListOrderdByLastVarIndexNeg[newClause->lastVarIndex]->insert(newClause);
		if (newClause->isBlack() && newClause->lastVarIndex < indexOfLastVariableOfBestBlackClause)
		{
			indexOfLastVariableOfBestBlackClause = newClause->lastVarIndex;
			bestBlackClause = newClause;
		}
	}
}

void HighLevelReader::orderLiterals(std::vector< std::vector<int> *> &cs)
{

	for (int i = 0; i < cs.size(); i++)	{
		std::vector<int> *clause = cs.at(i);
		/*for(int s = 0; s < 1; s++)
        {
            std::cout << cs.at(i)->at(s) << " ";
		}*/
		int j = 1;
		int flag = 1;
		while (flag != 0)		{
			flag = 0;
			for (int k = 0; k < clause->size() - j; k++)
			{
				if (std::abs(clause->at(k)) > std::abs(clause->at(k + 1)))
				{
					int temp = clause->at(k);
					clause->at(k) = clause->at(k+1);
					clause->at(k+1) = temp;
					flag = 1;
				}
			}
			j++;
		}
	}
}

void HighLevelReader::initDataStructure()
{
	clauseListOrderdByLastVarIndexNeg = std::vector<ClauseList*>(numberOfVariables + 1);
	clauseListOrderdByLastVarIndexPos = std::vector<ClauseList*>(numberOfVariables + 1);
	for (int i = 0; i < clauseListOrderdByLastVarIndexPos.size(); i++)
		clauseListOrderdByLastVarIndexPos[i] = new ClauseList();
	for (int i = 0; i < clauseListOrderdByLastVarIndexNeg.size(); i++)
		clauseListOrderdByLastVarIndexNeg[i] = new ClauseList();
	indexOfLastVariableOfBestBlackClause = numberOfVariables + 1;
}

bool CheckSolution::SimpleCheckSolution(std::vector< std::vector<int> *> cs, std::vector<bool> *solution)
{
	for (int i = 0; i < cs.size(); i++)	{
		std::vector<int> *clause = cs.at(i);
		bool satisfied = false;
		for (int j = 0; j < clause->size(); j++)
		{
			int lit = clause->at(j);
			int var = std::abs(lit);
			if (solution->at(var) == (lit > 0))
			{
				satisfied = true;
				break;
			}
		}
		if (!satisfied)
			return false;
	}
	return true;
}
