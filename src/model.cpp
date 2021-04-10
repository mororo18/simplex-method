#include "model.h"

Model::Model(int qnt){
    var_qnt = qnt;
}

void Model::add_obj_coef(double coef){

    if(obj_func.size() + 1 > var_qnt){
        std::cout << "Invalid OF size" << std::endl;
        return;
    }

    obj_func.push_back(coef);
}

void Model::add_cstr(Model::cstr constraint){

    //constraint.coef.push_back(constraint.value);

    if(constraint.coef.size() > var_qnt){
        std::cout << "Invalid constraint variables" << std::endl;
        return;
    }
        
    cstr_vec.push_back(constraint);
}

void Model::obj_func_print(){
    for(int i = 0; i < obj_func.size(); i++)
        std::cout << obj_func[i] << " ";
    std::cout << std::endl;

}

int Model::size(){
    return cstr_vec.size();
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

void Model::cstr::value_add(double value_cstr){
    value = value_cstr;
}

void Model::cstr::coef_print(){
    for(int i = 0; i < coef.size(); i++)
        std::cout << coef[i] << " ";
    std::cout << std::endl;
}
