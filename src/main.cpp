#include <fstream>
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

        model.print_model();

        //model.tableau_print();
        model.solve();
        model.print_solution();

        model.analyse();

        model.print_analysis();

        //model.print();
        //model.tableau_print();

        const char * f_name = "results/example1.txt";
        std::ofstream file {f_name};

        std::ofstream o_file;
        o_file.open(f_name, std::ios::in );
        if(file.is_open()){
            o_file << model.output_get();
            o_file.close();
        }
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

        // PROBLEMA DE ALOCACAO AQUI
        model.analyse_reopt();

        model.print();
        //model.print_mod();
        //model.tableau_print();

        const char * f_name = "results/example2.txt";
        const char * f_name_mod = "results/example2_mod.txt";
        
        std::ofstream file {f_name};
        std::ofstream file_mod {f_name_mod};

        std::ofstream o_file;
        o_file.open(f_name, std::ios::in );
        if(file.is_open()){
            o_file << model.output_get();
            o_file.close();
        }

        o_file.open(f_name_mod, std::ios::in );
        if(file.is_open()){
            o_file << model.output_mod_get();
            o_file.close();
        }
    }

    /* =========== MODEL n ============*/

    {
        const int n_var = 6;

        // data
        std::vector<int> OF_coef = { 4, 8, 6, 4, 3, 9}; 
        std::vector<std::vector<int>> EQ_m = {
            {1, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 0, 0},
            {0, 0, 0, 0, 1, 1}
        };

        std::vector<int> EQ_v = {500, 1200, 1800};

        std::vector<std::vector<int>> LEQ_m = {
            {1, 0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0, 1},
            {1, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0, 1},
        };

        std::vector<int> LEQ_v = {2000, 2000, 300, 300, 720, 720, 1080, 1080};

        std::vector<std::vector<int>> GEQ_m = {
            {1, 0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0, 1}
        };

        std::vector<int> GEQ_v = {1400, 1400};
        // init with the solver and qnt of variables
        Model model(simplex(), n_var);

        // def the problem type
        model.def("max");

        /* =========== OBJECTIVE FUNCTION ============*/

        // declares the O.F. 
        Model::obj_func func(n_var);

        char buf[100];
        for(int i = 0; i < OF_coef.size(); i++){

            sprintf(buf, "X_%d", i+1);

            // add the coeficients
            func.coef_add(OF_coef[i]);
            // add the variables names
            func.var_add(buf);
        }

        // add the F.O. to the model
        model.func_add(func);

        /* =========== CONSTRAINTS ============*/

        for(int i = 0; i < EQ_m.size(); i++){
            Model::cstr cstr;
            cstr.type_def("eq");
            cstr.value_add(EQ_v[i]);
            for(int j = 0; j < EQ_m[0].size(); j++){
                cstr.coef_add(EQ_m[i][j]);
            }
            // add the cstrs to the model
            model.cstr_add(cstr);

        }

        for(int i = 0; i < LEQ_m.size(); i++){
            Model::cstr cstr;
            cstr.type_def("leq");
            cstr.value_add(LEQ_v[i]);
            for(int j = 0; j < LEQ_m[0].size(); j++){
                cstr.coef_add(LEQ_m[i][j]);
            }
            model.cstr_add(cstr);
        }

        for(int i = 0; i < GEQ_m.size(); i++){
            Model::cstr cstr;
            cstr.type_def("geq");
            cstr.value_add(GEQ_v[i]);
            for(int j = 0; j < GEQ_m[0].size(); j++){
                cstr.coef_add(GEQ_m[i][j]);
            }
            model.cstr_add(cstr);
        }

        model.print_model();

        model.solve();
        model.print_solution();

        model.analyse();
        model.print_analysis();

        //model.print();

        const char * f_name = "results/example3.txt";
        std::ofstream file {f_name};

        std::ofstream o_file;
        o_file.open(f_name, std::ios::in );
        if(file.is_open()){
            o_file << model.output_get();
            o_file.close();
        }
    }

    return 0;
}
