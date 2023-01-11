#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <tuple>
#include "satFormula.h"
using namespace std;

// "SAT" followed by the satisfiable assignment of the
// variables. If a variable is printed as -1, it implies any assignment to
// that varibale will work
void printSolution(vector<int> &solutions){
        if (empty(solutions)){
                cout << "UNSAT" << endl;
        } else {
                cout << "SAT" << endl;
                for (auto it = solutions.begin(); it != solutions.end(); ++it){
                        cout << *it;
                        if (it != solutions.end() - 1){
                        cout << " ";
                        }
                }
                cout << endl;
        }
}

int main(){	
	expr satFormula;
	satFormula.buildFormula();
	vector<int> solutions;
	satFormula.solveSat(solutions, false);
	printSolution(solutions);
}
