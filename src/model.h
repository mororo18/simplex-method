#include <cstring>
#include <iostream>
#include <vector>
#include "simplex.h"

#define BIG_M 999999


class Model {

public:
    Model(int var_qnt);
    class cstr;

    void type_def(std::string model_type);
    void obj_coef_add(double coef);
    void cstr_add(cstr constraint);
    void tableau_generate();
    Tableau tableau_get();
    void tableau_print();
    int size();
    void obj_func_print();

private:
    int var_qnt;
    std::string type;
    int type_id;
    std::vector<double> obj_func;
    std::vector<cstr> cstr_vec;
    Tableau tableau;



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

