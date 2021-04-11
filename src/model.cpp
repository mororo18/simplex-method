#include "model.h"

Model::Model(int qnt){
    var_qnt = qnt;
}

void Model::type_def(std::string model_type){
    /*
        max -> 1
        min -> -1
    */

    if(!model_type.compare("max")){
        type = model_type;
        type_id = -1; 
    }else if( !model_type.compare("min")){
        type = model_type;
        type_id = 1;
    }else
        std::cout << "Invalid constraint type" << std::endl;
}

void Model::obj_coef_add(double coef){

    if(obj_func.size() + 1 > var_qnt){
        std::cout << "Invalid OF size" << std::endl;
        return;
    }

    obj_func.push_back(coef);
}

void Model::cstr_add(Model::cstr constraint){

    //constraint.coef.push_back(constraint.value);

    if(constraint.coef.size() != var_qnt){
        std::cout << "Invalid constraint variables" << std::endl;
        return;
    }
        
    cstr_vec.push_back(constraint);
}

void Model::tableau_generate(){

    if(!tableau.empty()){
        tableau.clear();
    }

    const int cstr_qnt = cstr_vec.size();

    // insert the first line
    tableau.push_back(obj_func);

    // columns for slack variables of first line
    for(int i = 0; i < cstr_qnt; i++)
        tableau[0].push_back(0);
    
    // current solution value
    tableau[0].push_back(0);

    if(type_id == -1)
        vec_multiply_scalar(tableau[0], -1);

    for(int i = 0; i < cstr_vec.size(); i++){
        std::vector<double> cstr_cpy = cstr_vec[i].coef;

        for(int j = 0; j < cstr_qnt; j++){
            cstr_cpy.push_back(0);
        }

        double cstr_value = cstr_vec[i].value;
        cstr_cpy.push_back(cstr_value);

        if(cstr_vec[i].type_id == 0){
            // subtract the first line(obj_func) by the current line times BIG_M (eq_cstr)
            vec_add_vec(tableau[0], cstr_cpy, -BIG_M);
        }else if(cstr_vec[i].type_id == -1){
            vec_multiply_scalar(cstr_cpy, -1);
        }

        //add the identity sub-matrix 
        cstr_cpy[var_qnt + i] = 1;
    
        tableau.push_back(cstr_cpy);
    }

}

Tableau Model::tableau_get(){
    return tableau;
}

void Model::tableau_print(){
    for(int i = 0; i < tableau.size(); i++){
        for(int j = 0; j < tableau[i].size(); j++){
            std::cout << tableau[i][j] << " ";
        }
        std::cout << std::endl;
    }
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
    
    if(!tp.compare("eq")){
        type = tp;
        type_id = 0;
    }else if(!tp.compare("leq")){
        type = tp;
        type_id = 1;
    }else if(!tp.compare("geq")){
        type = tp;
        type_id = -1;
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
