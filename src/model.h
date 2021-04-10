#include <cstring>
#include <iostream>
#include <vector>
#include "simplex.h"

#define BIG_M 10


class Model {

public:
    Model(int var_qnt);

    int var_qnt;
    std::string type;
    int type_id;
    class cstr;
    std::vector<double> obj_func;
    std::vector<cstr> cstr_vec;
    Tableau tableau;

    void type_def(std::string model_type);
    void add_obj_coef(double coef);
    void add_cstr(cstr constraint);
    Tableau tableau_generate();
    int size();
    void obj_func_print();

};

typedef std::string cstr_t;

class Model::cstr {

public:
   cstr_t type; 
   int type_id;
   std::vector<double> coef;
   double value;

   void type_def(cstr_t type);
   void coef_add(double var_coef);
   void value_add(double value_cstr);
   void coef_print();
};

