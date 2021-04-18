#include <cstring>
#include <cfloat>
#include <iostream>
#include <vector>

#define BIG_M 999999

#define MAX -1

#define EQ 0 
#define G_EQ -1 

typedef std::vector<std::vector<double>> Table;

class Model {

public:
    Model(int var_qnt);

    class obj_func;
    class cstr;

    void def(std::string model_type);
    void func_add(obj_func func);
    void cstr_add(cstr constraint);
    void tableau_generate();
    Table tableau_get();
    void tableau_print();
    int n_var_get();
    int size();

    Table tableau;

private:
    int var_qnt;
    std::string type;
    int type_id;


    //std::vector<double> obj_func;
    obj_func * main_func;// = new obj_func::obj_func(var_qnt);
    std::vector<cstr> cstr_vec;

    void vec_multiply_scalar(std::vector<double> & vec, double scalar);
    void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor);



};

class Model::obj_func {

public:
    obj_func(int qnt);
    void var_add(char name[]);
    void coef_add(double coef);
    std::vector<double> coef_get();
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

