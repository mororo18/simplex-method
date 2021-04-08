#include <cstring>
#include <iostream>
#include <vector>


class Model {

public:
    Model(int var_qnt);

    int var_qnt;
    class cstr;
    typedef std::vector<cstr> Table;
    Table table;

    void add_cstr(cstr constraint);

};

typedef std::string cstr_t;

class Model::cstr {

public:
   cstr_t type; 
   std::vector<double> coef;
   double value;
   void type_def(cstr_t type);
   void coef_add(double var_coef);
   void coef_print();
};

