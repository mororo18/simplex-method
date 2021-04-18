#include "model.h"

Model::Model(solver_func func, int qnt){
    solver = func;

    var_qnt = qnt;

    main_func = nullptr;
}

void Model::def(std::string model_type){
    /*
        max -> 1
        min -> -1
    */

    if(!model_type.compare("max")){
        type = model_type;
        // for 'max problem' the coefs have the opposite sign on the tableau
        type_id = -1; 
    }else if(!model_type.compare("min")){
        type = model_type;
        type_id = 1;
    }else
        std::cout << "Invalid constraint type" << std::endl;
}

void Model::func_add(Model::obj_func func){

    if(func.size() != var_qnt){
        std::cout << "Invalid OF size" << std::endl;
        return;
    }

    if(main_func != nullptr)
        delete main_func;
            
    main_func = new Model::obj_func (var_qnt);

    *main_func = func;
}

void Model::cstr_add(Model::cstr constraint){

    if(constraint.exp_coef.size() != var_qnt){
        std::cout << "Invalid constraint variables" << std::endl;
        return;
    }
        
    cstr_vec.push_back(constraint);
}

void Model::vec_multiply_scalar(std::vector<double> & vec, double scalar){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] <= DBL_EPSILON && vec[i] >= -DBL_EPSILON) //eq to 0
            continue;

        vec[i] *= scalar;
    }
}

void Model::vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor){
    //vec_a - vec_b
    for(int i = 0; i < vec_a.size(); i++){
        vec_a[i] += factor * vec_b[i]; 
    }
}

void Model::tableau_generate(){

    if(!tableau.empty()){
        tableau.clear();
    }

    const int cstr_qnt = cstr_vec.size();

    std::vector<double> vec = main_func->coef_get();
    // insert the first row
    tableau.push_back(vec);

    //for(int i = 0; i < vec.size(); i++)
        //std::cout << vec[i] << " ";
    //exit(0);

    // columns for slack variables of first row 
    for(int i = 0; i < cstr_qnt; i++)
        tableau[0].push_back(0);
    
    // current solution value
    tableau[0].push_back(0);

    if(type_id == MAX)
        vec_multiply_scalar(tableau[0], MAX);

    for(int i = 0; i < cstr_vec.size(); i++){
        std::vector<double> cstr_cpy = cstr_vec[i].exp_coef;

        for(int j = 0; j < cstr_qnt; j++){
            cstr_cpy.push_back(0);
        }

        double cstr_value = cstr_vec[i].value;
        cstr_cpy.push_back(cstr_value);

        if(cstr_vec[i].type_id == EQ){
            // subtract the first row(obj_func) by the current row times BIG_M (eq_cstr)
            vec_add_vec(tableau[0], cstr_cpy, -BIG_M);
        }else if(cstr_vec[i].type_id == G_EQ){
            vec_multiply_scalar(cstr_cpy, G_EQ);
        }

        //add the identity sub-matrix 
        cstr_cpy[var_qnt + i] = 1;
    
        tableau.push_back(cstr_cpy);
    }

}

Table Model::tableau_get(){
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

int Model::n_var_get(){
    return var_qnt;
}

int Model::size(){
    return cstr_vec.size();
}

void Model::print(){

}

void Model::solve(){
    tableau_generate();
    solver(tableau);
}

Model::obj_func::obj_func(int qnt){
    var_qnt = qnt;
}

void Model::obj_func::var_add(char name[]){
    
    if(var_name.size() + 1 > var_qnt){
        std::cout << "Invalid OF size" << std::endl;
        return;
    }

    std::string var_new (name);

    var_name.push_back(var_new);
}

void Model::obj_func::coef_add(double var_coef){

    if(coef.size() + 1 > var_qnt){
        std::cout << "Invalid OF size" << std::endl;
        return;
    }

    coef.push_back(var_coef);
}

std::vector<double> Model::obj_func::coef_get(){
    return coef;
}

int Model::obj_func::size(){
    return var_qnt;
}

void Model::obj_func::print(){
    for(int i = 0; i < coef.size(); i++)
        std::cout << coef[i] << "*" << var_name[i] << " ";
    std::cout << std::endl;

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
    exp_coef.push_back(var_coef);
}

void Model::cstr::value_add(double value_cstr){
    value = value_cstr;
}

void Model::cstr::coef_print(){
    for(int i = 0; i < exp_coef.size(); i++)
        std::cout << exp_coef[i] << " ";
    std::cout << std::endl;
}
