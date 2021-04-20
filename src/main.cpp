#include "simplex.h"
#include "model.h"

/*
    The Model class generates a tableau from the received data
    and the simplex algorithm solves the tableau
*/  

int main(){
    // def solver and qnt of variables
    Model model(simplex(), 2);

    model.def("min");

    Model::obj_func func(2);

    func.coef_add(0.4);
    func.coef_add(0.5);
    //func.coef_add(8);

    func.var_add("x_1");
    func.var_add("x_2");
    //func.var_add("x_3");

    func.print();

    model.func_add(func);

    // def the problem type
    //model.type_def("min");
    /*

    //add the coeficients of obj_func sequentially
    model.obj_coef_add(0.4);
    model.obj_coef_add(0.5);
    //model.obj_func_print();
    */
    
    Model::cstr cstr_01;

    // def the type of cstr
    cstr_01.type_def("leq");

    //add the coeficients of the constraint sequentially
    cstr_01.coef_add(0.3);
    cstr_01.coef_add(0.1);
    //cstr_01.coef_add(2.7);
    // add the cstr value
    cstr_01.value_add(2.7);

    Model::cstr cstr_02;

    cstr_02.type_def("eq");

    cstr_02.coef_add(0.5);
    cstr_02.coef_add(0.5);
    //cstr_02.coef_add(0);
    cstr_02.value_add(6);


    Model::cstr cstr_03;

    cstr_03.type_def("geq");

    cstr_03.coef_add(0.6);
    cstr_03.coef_add(0.4);
    //cstr_03.coef_add(6);
    cstr_03.value_add(6);

    Model::cstr cstr_04;

    cstr_04.type_def("geq");

    cstr_04.coef_add(1);
    cstr_04.coef_add(0);
    cstr_04.coef_add(1);
    cstr_04.value_add(21);

    // add the cstrs to the model
    model.cstr_add(cstr_01);
    model.cstr_add(cstr_02);
    model.cstr_add(cstr_03);
    //model.cstr_add(cstr_04);

    //model.tableau_print();
    //exit(0);
    model.solve();
    std::cout << std::endl;

    model.analyse();
    model.print();
    model.tableau_print();


    std::cout << std::endl;
    std::cout << std::endl;

    Tableau tableau = 

    {

    {4, 12, 18, 0, 0, 0},
    {-1, 0, -3, 1, 0, -3},
    {0, -2, -2, 0, 1, -5}

    };

    //dual_simplex_solve(tableau);
    //algo_def(tableau);
    //tableau_print(tableau);

    // generate the tableau
    //Tableau tableau = model.tableau_get();

    //simplex_solve(model.tableau);
    //tableau_print(model.tableau);
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
