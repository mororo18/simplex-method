#include <iostream>
#include <vector>
#include <cfloat>
#include "simplex.h"

int main(){
    std::vector< std::vector<double> > tableau;
    int M = 10;

    tableau = 
    {{-1.1*M + 0.4, -0.9*M + 0.5, 0, 0, M,  0, -12*M},
    {0.3, 0.1, 1, 0, 0, 0, 2.7},
    {0.5, 0.5, 0, 1, 0, 0, 6},
    {0.6, 0.4, 0, 0, -1, 1, 6}};
    
    simplex_solve(tableau);

    return 0;
}
