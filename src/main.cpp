#include <iostream>
#include <vector>
#include <cfloat>
#include "simplex.h"
#include "model.h"

int main(){
    Model model(2);
    Model::cstr constraint;
    constraint.coef_add(0.3);
    constraint.coef_add(0.1);
    constraint.coef_print();
    model.add_cstr(constraint);

    Tableau tableau;
    int M = 10;

    tableau = 
    {{-0.5*M + 0.4, -0.5*M + 0.5, 0, 0, 0, -6*M},
    {0.3, 0.1, 1, 0, 0, 2.7},
    {0.5, 0.5, 0, 1, 0, 6},
    {-0.6, -0.4, 0, 0, 1, -6}};
    
    simplex_solve(tableau);

    return 0;
}
