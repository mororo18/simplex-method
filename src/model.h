#include <cstring>
#include <cfloat>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <iomanip>

#define BIG_M 999999
#define INFINITE 999999

#define MAX -1

#define EQ 0 
#define L_EQ 1 
#define G_EQ -1 

#define OUT_PRECISION 3

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
    Table tableau_get();
    void tableau_print();
    int n_var_get();
    int size();
    void analyse();   
    void solve();
    double obj_value_get();
    void analyse_add(cstr cstr_new);    // analise
    void analyse_reopt();     // analise

    std::string output_get();
    std::string output_mod_get();
    void print();
    void print_mod();
    void print_model();
    void print_model_mod();
    void print_solution();
    void print_solution_mod();
    void print_analysis();
    void print_analysis_mod();

private:
    int var_qnt;                                            // qnt of variables 
    std::string type;                                       // type of problem ( max or min)
    int type_id;                                            // numerical id
    double obj_value;                                       // final OF value
    double obj_value_mod;                                   // final mod OF value

    bool solved;                                            // flags
    bool analysed;                                          // "
    bool analysed_mod;                                      // "

    solver_func solver;                                     // pointer to solver's main func

    obj_func * main_func;                                   // OF
    std::vector<cstr> cstr_vec;
    std::vector<cstr> cstr_vec_new;                         // new constraints

    Table tableau;
    Table solution_tableau;                                 // final tableau of the original model

    std::vector<double> solution_primal;
    std::vector<double> solution_primal_mod;
    std::vector<double> solution_dual;
    std::vector<double> solution_dual_mod;

    // infos recalculadas a cada analise

    std::vector<double> b_opt;                              // right hand of the final tableau
    std::vector<std::pair<double, double>> b_range;         //restrictions right hand
    std::vector<std::pair<double, double>> b_range_mod;     //restrictions right hand
    std::vector<std::pair<double, double>> c_range;         // obj_func coefs
    std::vector<std::pair<double, double>> c_range_mod;     // obj_func coefs

    std::vector<int> I_index;                               // inicial identity matrix indexes
    std::vector<std::vector<double>> inverse_matrix;        // solution matrix of final tableau

    std::vector<double> original_coef;                      // original OF coefs
    std::vector<double> basic_coef;                         // original coef of the final basic variables

    std::string output_model;
    std::string output_model_mod;
    std::string output_solution;
    std::string output_solution_mod;
    std::string output_analysis;
    std::string output_analysis_mod;


    void vec_multiply_scalar(std::vector<double> & vec, double scalar);
    void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor);
    void vec_print_dbl(std::vector<double> vec);

    void tableau_generate();
    void inverse_matrix_store();  
    void basic_coef_get();   
    void solution_primal_get(std::vector<double> & solution_primal);
    void solution_dual_get(std::vector<double> & solution_dual);
    void output_generate();
    void output_mod_generate();

    void b_opt_store();   
    void b_range_calc(std::vector<std::pair<double, double>> & b_range);    // analise
    void c_range_calc(std::vector<std::pair<double, double>> & c_range);    // analise
    void tableau_resize(cstr cstr_new);   
};

class Model::obj_func {

public:
    obj_func(int qnt);
    void var_add(char name[]);
    void coef_add(double coef);
    std::vector<double> coef_get();
    std::string var_name_get(int n);
    int size();
    std::string output_generate();
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

