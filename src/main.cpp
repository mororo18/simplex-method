#include "simplex.h"
#include "model.h"

/*
    The class Model generates a tableau from the received data
    and the simplex algorithm solves the tableau
*/  

int main(){
    // qnt of variables
    Model model(2);

    // def the problem type
    model.type_def("min");

    //add the coeficients of obj_func sequentially
    model.obj_coef_add(0.4);
    model.obj_coef_add(0.5);
    //model.obj_func_print();
    
    Model::cstr cstr_01;

    // def the type of cstr
    cstr_01.type_def("leq");

    //add the coeficients of the constraint sequentially
    cstr_01.coef_add(0.3);
    cstr_01.coef_add(0.1);
    // add the cstr value
    cstr_01.value_add(2.7);

    Model::cstr cstr_02;

    cstr_02.type_def("eq");

    cstr_02.coef_add(0.5);
    cstr_02.coef_add(0.5);
    cstr_02.value_add(6);


    Model::cstr cstr_03;

    cstr_03.type_def("geq");

    cstr_03.coef_add(0.6);
    cstr_03.coef_add(0.4);
    cstr_03.value_add(6);

    // add the cstrs to the model
    model.cstr_add(cstr_01);
    model.cstr_add(cstr_02);
    model.cstr_add(cstr_03);

    model.tableau_generate();
    model.tableau_print();

    // generate the tableau
    Tableau tableau = model.tableau_get();

    simplex_solve(tableau);
    tableau_print(tableau);
    //int M = 10;

/*
    tableau = 
    {{-0.5*M + 0.4, -0.5*M + 0.5, 0, 0, 0, -6*M},
    {0.3, 0.1, 1, 0, 0, 2.7},
    {0.5, 0.5, 0, 1, 0, 6},
    {-0.6, -0.4, 0, 0, 1, -6}};
    */
    

    return 0;
}
