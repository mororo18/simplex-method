#include <cstring>
#include <iostream>
#include <vector>


class Model {

public:
    Model(int var_qnt);

    int var_qnt;
    class cstr;
    std::vector<double> obj_func;
    std::vector<cstr> cstr_vec;
    //Table table;

    void add_obj_coef(double coef);
    void add_cstr(cstr constraint);
    int size();
    void obj_func_print();

};

typedef std::string cstr_t;

class Model::cstr {

public:
   cstr_t type; 
   std::vector<double> coef;
   double value;

   void type_def(cstr_t type);
   void coef_add(double var_coef);
   void value_add(double value_cstr);
   void coef_print();
};

