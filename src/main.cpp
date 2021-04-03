#include <iostream>
#include <vector>
#include <cfloat>
#include "simplex.h"

int main(){
    std::vector< std::vector<double> > tableau;

    tableau = 
    {{-3, -5, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 4},
    {0, 2, 0, 1, 0, 12},
    {3, 2, 0, 0, 1, 18}};
    
    simplex_solve(tableau);

    return 0;
}
