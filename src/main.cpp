#include <iostream>
#include <vector>
#include <cfloat>
#include "simplex.h"
#include "model.h"

int main(){
    Model model(2);
    Model::cstr constraint;

    model.add_obj_coef(0.4);
    model.add_obj_coef(0.5);
    model.obj_func_print();
    
    constraint.coef_add(0.3);
    constraint.coef_add(0.1);
    constraint.value_add(2.7);
    //constraint.coef_print();
    model.add_cstr(constraint);
    //model.table[0].coef_print();
    //std::cout << model.table[0].value << std::endl;

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
