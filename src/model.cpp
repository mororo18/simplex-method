#include "model.h"

Model::Model(int qnt){
    var_qnt = qnt;
}

void Model::add_cstr(Model::cstr constraint){

    //constraint.coef.push_back(constraint.value);

    if(constraint.coef.size() > var_qnt){
        std::cout << "Invalid constraint variables" << std::endl;
        return;
    }
        
    table.push_back(constraint);
}

/*
    0 -> eq
    1 -> leq
   -1 -> geq
*/

void Model::cstr::type_def(cstr_t tp){
    
    if(!type.compare("eq") || !type.compare("leq") || !type.compare("geq")){
        type = tp;
    }else
        std::cout << "Invalid constraint type" << std::endl;
}

void Model::cstr::coef_add(double var_coef){
    coef.push_back(var_coef);
}

void Model::cstr::coef_print(){
    for(int i = 0; i < coef.size(); i++)
        std::cout << coef[i] << " ";
    std::cout << std::endl;
}
