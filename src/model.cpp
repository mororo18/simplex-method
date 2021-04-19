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
        std::cout << "Invalid constraint type" << std::endl; } 
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

void Model::vec_print_dbl(std::vector<double> vec){
    for(int i = 0; i < vec.size(); i++){
        std::cout << vec[i] << " " ;//+= factor * vec_b[i]; 
    }

    std::cout << std::endl;
}

void Model::tableau_generate(){

    if(!tableau.empty()){
        tableau.clear();
        I_index.clear();
    }

    const int cstr_qnt = cstr_vec.size();
    int n = var_qnt;

    // store the original coef value
    original_coef = main_func->coef_get();

    //std::vector<double> vec = main_func->coef_get();
    // insert the first row
    tableau.push_back(original_coef);

    //for(int i = 0; i < vec.size(); i++)
        //std::cout << vec[i] << " ";
    //exit(0);

    int slack_cols = 0;

    // columns for slack variables of first row 
    for(int i = 0; i < cstr_qnt; i++){
        if(cstr_vec[i].type_id == G_EQ){
            // the two cols of the artificial variables of the geq constraint
            tableau[0].push_back(0);
            tableau[0].push_back(0);
            slack_cols += 2;
        }else {
            tableau[0].push_back(0);
            slack_cols++;
        }
    }
    
    // current solution value
    tableau[0].push_back(0);

    if(type_id == MAX)
        vec_multiply_scalar(tableau[0], MAX);

    for(int i = 0; i < cstr_vec.size(); i++){
        std::vector<double> tableau_row = cstr_vec[i].exp_coef;

        // slack vars cols
        for(int j = 0; j < slack_cols; j++){
            tableau_row.push_back(0);
        }

        double cstr_value = cstr_vec[i].value;
        tableau_row.push_back(cstr_value);

        if(cstr_vec[i].type_id == EQ){
            // subtract the first row(obj_func) by the current row times BIG_M (eq_cstr)
            vec_add_vec(tableau[0], tableau_row, -BIG_M);

            // store the original coef value of the slack var
            original_coef.push_back(BIG_M);
        }else if(cstr_vec[i].type_id == G_EQ){
            //vec_multiply_scalar(cstr_cpy, G_EQ);
            tableau_row[n+i] = -1;
            n++;
            vec_add_vec(tableau[0], tableau_row, -BIG_M);

            // store the original coef value of the slack var
            original_coef.push_back(0);
            original_coef.push_back(BIG_M);
        }else
            // store the original coef value of the slack var
            original_coef.push_back(0);

        //add the identity sub-matrix 
        tableau_row[n + i] = 1;

        // store the identity sub-matrix col indexes
        I_index.push_back(n + i);
    
        tableau.push_back(tableau_row);
    }

}

void Model::inverse_matrix_get(){
    inverse_matrix = std::vector<std::vector<double>> (I_index.size(), std::vector<double> (I_index.size()));

    for(int i = 0; i < I_index.size(); i++){
        int col = I_index[i];
        for(int j = 1; j < tableau.size(); j++){
            inverse_matrix[j-1][i] = tableau[j][col]; 
        }
    }

    /*
    for(int i = 0; i < inverse_matrix.size(); i++){
        for(int j = 0; j < inverse_matrix[0].size(); j++){
            std::cout << inverse_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    vec_print_dbl(original_coef);
    */
}

void Model::non_basic_coef_get(){
    std::vector<double> coef (size());
    for(int i = 0; i < tableau[0].size(); i++){
        if(tableau[0][i] == 0){
            for(int j = 1; j < tableau.size(); j++){
                if(tableau[j][i] == 1){
                    coef[j-1] = -1 * type_id * original_coef[i];
                }
            }
        }
    }

    non_basic_coef = coef;
    //vec_print_dbl(non_basic_coef);
}

void Model::solution_primal_get(){
    const int s_col = tableau[0].size() - 1;
    for(int i = 0; i < var_qnt; i++){
        for(int j = 1; j < tableau.size(); j++){
            if(tableau[j][i] == 1)
                solution_primal.push_back(tableau[j][s_col]);
        }
    }

    obj_value = std::abs(tableau[0][s_col]);
}

void Model::solution_dual_get(){
    inverse_matrix_get();
    non_basic_coef_get();
    
    for(int i = 0; i < inverse_matrix.size(); i++){
        double sum = 0;
        for(int j = 0; j < inverse_matrix.size(); j++){
            sum += non_basic_coef[j] * inverse_matrix[j][i]; 
        }

        solution_dual.push_back(sum);
    }
}

double Model::obj_value_get(){
    return obj_value;
}

void Model::solve(){
    tableau_generate();
    solver(tableau);

    solution_primal_get();
    solution_dual_get();
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

void Model::analyse(){

}

void Model::print(){
    std::cout << "Funcao objetivo : " << obj_value_get() << std::endl; 
    std::cout << "Solucao primal : "; vec_print_dbl(solution_primal);
    std::cout << "Solucao dual : ";vec_print_dbl(solution_dual);
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
        std::cout << "aqui" << std::endl;
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
