#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <tuple>

class expr {
	int variableNum; // number of total variables
	int clauseNum; // number of total clauses

	// variable x_i unassigned: -1
	// variable x_i set to true: 1
	// variable x_i set to false: 0
	std::vector<int> literals;
	std::vector<std::list<int>> variables; 
	std::vector<std::tuple<int, int, int, std::list<int>>> clauses;
	
	// Returns index of largest assigned variable, and -1 in the case
	// were no variables have been assigned yet
	int largestAssignment(); 
	bool isSolution(); // Determines if current assignment is solution
	bool isDeadEnd(); // Determines if current assignment is dead end
	
	// Sets assignment of the largest unassigned variable to valAssigned
	void setAssignment(bool valAssigned);
	
	// Undoes the previous assignment just made. This is called once
	// we have reached a dead end, and need to backtrack
	void undoAssignment();	
       
       public:	
	// Buildings satFormula based on DIMACS input
	void buildFormula();	
	
	// Determines if satFormula taken as input in DIMACS format is satisfiable.
	// If it is, it prints places the satisfiable assignment in the vector solutions
	void solveSat(std::vector<int> &solutions, bool foundSolution);
};
