#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <tuple>
#include "satFormula.h"
using namespace std;

int expr::largestAssignment(){
	for (int i = 0; i < variableNum; ++i){
		if (literals[i] == -1){
			return i - 1;
		}
	}
	return variableNum - 1; 
}

bool expr::isSolution(){
	for (auto it = clauses.begin(); it != clauses.end(); ++it){
		if (get<1>(*it) == 0){
			return false;
		}
	}
	return true;
}

bool expr::isDeadEnd(){
	for (auto it = clauses.begin(); it != clauses.end(); ++it){
		if (get<0>(*it) == get<2>(*it)){
			return true;
		}
	}
	return false;
}	

void expr::setAssignment(bool valAssigned){
	int val = largestAssignment() + 1;
	if (val != variableNum){	
		if (valAssigned){
			literals[val] = 1; // Assigning true to variable val + 1
		} else {
			literals[val] = 0; // Assigning false to variable val - 1
		} 
		// Iterate through clauses that contain variable x_(val + 1)
		list<int> clauseList = variables[val];
        	for (auto it = clauseList.begin(); it != clauseList.end(); ++it){
			int clauseVal = *it;
                	list<int> varInClause = get<3>(clauses[clauseVal - 1]);
                	bool satisfiedClause = 0;
			bool containedInClause = 0;
                	for (auto it1 = varInClause.begin(); it1 != varInClause.end(); ++it1){
				if ((*it1 == val + 1 && valAssigned)|| (*it1 == 0 - (val + 1) && !valAssigned )){
					satisfiedClause = 1; // Does the assignment satisfy the clause?
				} else if (*it1 == val + 1 || *it1 == 0 - (val + 1)){
					containedInClause = 1; // Does the assignment not satisfy the clause?
				}
			}	
			// Update the satisfiable/unsatisfiable 
			// parameters of the current clause
                	if (satisfiedClause){
				get<1>(clauses[clauseVal - 1]) += 1;
			} 
			if (containedInClause) {
				get<2>(clauses[clauseVal - 1]) += 1;
			}
		}
	}	
}

void expr::undoAssignment(){
	int val = largestAssignment();
	int valAssigned = literals[val];
        list<int> clauseList = variables[val];
	// iterate through clauses that contain variable x_(val + 1) 
        for (auto it = clauseList.begin(); it != clauseList.end(); ++it){
		int clauseVal = *it;
                list<int> varInClause = get<3>(clauses[clauseVal - 1]);
                bool satisfiedClause = 0;
		bool containedInClause = 0;
                for (auto it1 = varInClause.begin(); it1 != varInClause.end(); ++it1){
			if ((*it1 == val + 1 && valAssigned) || (*it1 == 0 - (val + 1) && !valAssigned)){
			       	satisfiedClause = 1; // does the assignment we are going to undo satisfy the clause?
			} else if (*it1 == val + 1 || *it1 == 0 - (val + 1)){
				containedInClause = 1; // does the assignment we are going to undo not satisfy the clause?
			}
		}
		// undo satisfiable/unsatisfiable 
		// parameters of the current clause
		if (satisfiedClause){
			get<1>(clauses[clauseVal - 1]) -= 1;
		} 
		if (containedInClause){
			get<2>(clauses[clauseVal - 1]) -= 1;
		} 
	}
	literals[val] = -1;
}

void expr::solveSat(vector<int> &solutions, bool foundSolution){
	if (isSolution()){
		solutions = literals;
		foundSolution = 1;
	} else if (isDeadEnd()){
		undoAssignment(); // We have reached a dead end, so undo previous assignment
	} else {
		// Set new assignment of next unassigned variable with true, and then
		// recurisvely solve sat. Then try false assignment, and recursively
		// solve sat.
		setAssignment(true); 
		solveSat(solutions, foundSolution);
		if (foundSolution){ // unwind stack if solution is found
			return;
		}
		setAssignment(false); 
		solveSat(solutions, foundSolution);
		if (foundSolution){
			return;
		}
		if(literals[0] != -1){
			undoAssignment();
		}
	}	
}

void expr::buildFormula(){
	string val;             // For the sake of this satSolver, the
        cin >> val >> val;      // first two strings in input are not important
        int varVal;
        int clauseVal;
        cin >> varVal;
        cin >> clauseVal;
        variableNum = varVal;
        clauseNum = clauseVal;
        for (int i = 0; i < varVal; ++i){
                literals.push_back(-1);
		list<int> varList;
		variables.push_back(varList);
        }
	int var;
        int currClause = 1;
        int varCount = 0;
        list<int> clauseList;
        while (cin >> var){
                if (var == 0){
                        auto tupleVal = make_tuple(varCount, 0, 0, clauseList);
                        clauses.push_back(tupleVal);
                        ++currClause;
			varCount = 0;
			clauseList.clear();
              		continue;
                } else {
                        ++varCount;
                } 
                clauseList.push_front(var);
		if (var < 0){
			var *= -1;
		}
		variables[var - 1].push_front(currClause);
	}
}
