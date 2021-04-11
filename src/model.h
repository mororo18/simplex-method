#include <cstring>
#include <cfloat>
#include <iostream>
#include <vector>

#define BIG_M 999999

typedef std::vector<std::vector<double>> Table;

class Model {

public:
    Model(int var_qnt);
    class cstr;

    void type_def(std::string model_type);
    void obj_coef_add(double coef);
    void cstr_add(cstr constraint);
    void tableau_generate();
    Table tableau_get();
    void tableau_print();
    int size();
    void obj_func_print();

private:
    int var_qnt;
    std::string type;
    int type_id;
    std::vector<double> obj_func;
    std::vector<cstr> cstr_vec;
    Table tableau;
    void vec_multiply_scalar(std::vector<double> & vec, double scalar);
    void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor);



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
   std::vector<double> coef;
   double value;

};

