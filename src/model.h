#include <cstring>
#include <cfloat>
#include <iostream>
#include <vector>
#include <stdio.h>

#define BIG_M 999999
#define INFINITE 999999

#define MAX -1

#define EQ 0 
#define L_EQ 1 
#define G_EQ -1 

typedef std::vector<std::vector<double>> Table;
typedef void (*solver_func)(Table & tableau);

class Model {

public:
    Model(solver_func func, int var_qnt);
    ~Model();

    class obj_func;
    class cstr;

    void def(std::string model_type);
    void func_add(obj_func func);
    void cstr_add(cstr constraint);
    void tableau_generate();
    Table tableau_get();
    void tableau_print();
    void inverse_matrix_get();  // analise
    void non_basic_coef_get();   // analise
    void solution_primal_get();
    void solution_dual_get();
    int n_var_get();
    int size();
    void analyse();   // analise
    void solve();
    void print();
    double obj_value_get();
    void b_opt_store();   // analise
    void b_range_calc();    // analise
    void c_range_calc();    // analise
    void tableau_resize(cstr cstr_new);   // analise
    void analyse_add(cstr cstr_new);    // analise
    void analyse_reopt();     // analise

private:
    int var_qnt;
    std::string type;
    int type_id;
    double obj_value;


    solver_func solver;

    obj_func * main_func;
    std::vector<cstr> cstr_vec;
    std::vector<cstr> cstr_vec_new;

    Table tableau;
    Table solution_tableau;

    std::vector<double> solution_primal;
    std::vector<double> solution_dual;

    // infos recalculadas a cada analise

    std::vector<double> b_opt;      // analise
    std::vector<std::pair<double, double>> b_range; //restrictions right rand
    std::vector<std::pair<double, double>> c_range; // obj_func coefs

    std::vector<int> I_index;    // analise
    std::vector<std::vector<double>> inverse_matrix;    // analise

    std::vector<double> original_coef;    // analise
    std::vector<double> non_basic_coef;    // analise


    void vec_multiply_scalar(std::vector<double> & vec, double scalar);
    void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor);
    void vec_print_dbl(std::vector<double> vec);
};

class Model::obj_func {

public:
    obj_func(int qnt);
    void var_add(char name[]);
    void coef_add(double coef);
    std::vector<double> coef_get();
    std::string var_name_get(int n);
    int size();
    void print();

private:
    int var_qnt;
    std::vector<std::string> var_name;
    std::vector<double> coef;
};

typedef std::string cstr_t;

class Model::cstr {

public:
   void type_def(cstr_t type);
   void coef_add(double var_coef);
   void value_add(double value_cstr);
   void coef_print();

   cstr_t type; 
   int type_id;
   std::vector<double> exp_coef;
   double value;

};

