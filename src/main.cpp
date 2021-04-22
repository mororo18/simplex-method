#include "simplex.h"
#include "model.h"

/*
   The Model class generates a tableau from the received data
   and the simplex algorithm solves the tableau
 */  

int main(){

    /* =========== MODEL 01 ============*/

    {
        const int n_var = 2;
        
        // init with the solver and qnt of variables
        Model model(simplex(), n_var);

        // def the problem type
        model.def("min");

        /* =========== OBJECTIVE FUNCTION ============*/

        // declares the O.F. 
        Model::obj_func func(n_var);

        // add the coeficients
        func.coef_add(0.4);
        func.coef_add(0.5);

        // add the variables names
        func.var_add("X_1");
        func.var_add("X_2");

        //func.print();

        // add the F.O. to the model
        model.func_add(func);

        /* =========== CONSTRAINTS ============*/

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

        model.print();

        //model.tableau_print();
        model.solve();
        std::cout << std::endl;

        model.analyse();
        //model.tableau_print();


        std::cout << std::endl;
        std::cout << std::endl;

    }

    /* =========== MODEL 02 ============*/

    {
        const int n_var = 2;
        
        // init with the solver and qnt of variables
        Model model(simplex(), n_var);

        // def the problem type
        model.def("max");

        /* =========== OBJECTIVE FUNCTION ============*/

        // declares the O.F. 
        Model::obj_func func(n_var);

        // add the coeficients
        func.coef_add(3);
        func.coef_add(5);

        // add the variables names
        func.var_add("X_1");
        func.var_add("X_2");

        //func.print();

        // add the F.O. to the model
        model.func_add(func);

        /* =========== CONSTRAINTS ============*/

        Model::cstr cstr_01;

        // def the type of cstr
        cstr_01.type_def("leq");

        //add the coeficients of the constraint sequentially
        cstr_01.coef_add(1);
        cstr_01.coef_add(0);

        // add the cstr value
        cstr_01.value_add(4);


        Model::cstr cstr_02;

        cstr_02.type_def("leq");

        cstr_02.coef_add(0);
        cstr_02.coef_add(2);

        cstr_02.value_add(24);



        Model::cstr cstr_03;

        cstr_03.type_def("leq");

        cstr_03.coef_add(3);
        cstr_03.coef_add(2);

        cstr_03.value_add(18);


        Model::cstr cstr_04;

        cstr_04.type_def("leq");

        cstr_04.coef_add(2);
        cstr_04.coef_add(3);

        cstr_04.value_add(24);

        // add the cstrs to the model
        model.cstr_add(cstr_01);
        model.cstr_add(cstr_02);
        model.cstr_add(cstr_03);

        model.analyse_add(cstr_04);


        //model.tableau_print();
        model.solve();
        std::cout << std::endl;

        model.analyse();
        //model.tableau_print();

        //model.analyse_reopt();
        model.print();
        //model.tableau_print();

        //exit(0);

        //std::cout << std::endl;
        //std::cout << std::endl;

    }
/*
    Tableau tableau = 

    {

    {4, 12, 18, 0, 0, 0},
    {-1, 0, -3, 1, 0, -3},
    {0, -2, -2, 0, 1, -5}

    };
*/

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
