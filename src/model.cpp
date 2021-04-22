#include "model.h"

Model::Model(solver_func func, int qnt){
    solver = func;

    var_qnt = qnt;

    main_func = nullptr;
}

Model::~Model(){
    solution_primal.clear();
    solution_dual.clear();

    delete main_func;

    cstr_vec.clear();
    cstr_vec_new.clear();

    tableau.clear();
    solution_tableau.clear();

    b_opt.clear();
    b_range.clear();
    c_range.clear();
    I_index.clear();
    inverse_matrix.clear();

    original_coef.clear();
    non_basic_coef.clear();
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

void Model::analyse_add(cstr cstr_new){

    if(cstr_new.exp_coef.size() != var_qnt){
        std::cout << "Invalid constraint added to analyse" << std::endl;
        exit(0);
    }

    //exit(0);

    cstr_vec_new.push_back(cstr_new); 
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

    // insert the first row
    tableau.push_back(original_coef);

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
    if(!solution_primal.empty())
        solution_primal.clear();

    const int s_col = tableau[0].size() - 1;
    
    for(int i = 0; i < var_qnt; i++){
        if(tableau[0][i] == 0){
            for(int j = 1; j < tableau.size(); j++){
                if(tableau[j][i] == 1)
                    solution_primal.push_back(tableau[j][s_col]);
            }
        }else
            solution_primal.push_back(0);
    }

    obj_value = std::abs(tableau[0][s_col]);
}

void Model::solution_dual_get(){
    if(!solution_dual.empty())
        solution_dual.clear();
        
    inverse_matrix_get();
    non_basic_coef_get();
    
    for(int i = 0; i < inverse_matrix.size(); i++){
        double sum = 0;
        for(int j = 0; j < inverse_matrix.size(); j++){
            sum += non_basic_coef[j] * inverse_matrix[j][i]; 
        }

        if(sum < DBL_EPSILON && sum > -DBL_EPSILON)
            sum = 0;

        solution_dual.push_back(sum);
    }
}

void Model::b_opt_store(){
    if(!b_opt.empty())
        b_opt.clear();
            
    const int s_col = tableau[0].size() - 1;
    for(int j = 1; j < tableau.size(); j++)
        b_opt.push_back(tableau[j][s_col]);
    
}

void Model::b_range_calc(){
    b_opt_store();    
    
    if(!b_range.empty())
        b_range.clear();

    for(int col = 0; col < inverse_matrix[0].size(); col++){
        double upper_bound = INFINITE; 
        double lower_bound = -INFINITE;

        for(int row = 0; row < inverse_matrix.size(); row++){
            double coef = inverse_matrix[row][col];
            double coef_inv = 1.0f / coef;
            double value = -1 * b_opt[row];

            value = value * coef_inv;

            if(coef_inv < 0 && value < upper_bound){
                upper_bound = value; 

            }else if(value > lower_bound){
                lower_bound = value;
            }

        }

        std::pair<double, double> range;
        range.first = lower_bound;
        range.second = upper_bound;

        b_range.push_back(range);

        std::cout << "cstr n" << col << "  de " << lower_bound << " a " << upper_bound << std::endl;
    }
}

void Model::c_range_calc(){
    
    if(!c_range.empty())
        c_range.clear();

    for(int col = 0; col < var_qnt; col++){
        if(tableau[0][col] > 0){
            std::pair<double, double> range = std::make_pair(-INFINITE, tableau[0][col]);
            std::cout << "var X_" << col << "  de " << -INFINITE << " a " << tableau[0][col] << std::endl;
            c_range.push_back(range);
        }else if(tableau[0][col] == 0){
            int row_i;

            // identify the var row, where c_i_row == 1
            for(int row = 1; row < tableau.size(); row++){
                if(tableau[row][col] == 1){
                    row_i = row;
                    break;
                }
            }

            double upper_bound = INFINITE ; 
            double lower_bound = -INFINITE ;

            for(int col_i = 0; col_i < tableau[0].size() - 1; col_i++){
                if(tableau[0][col_i] == 0 || tableau[0][col_i] >= INFINITE -2)
                    continue;

                // REVIRSAR POSTERIORMENTE
                double coef = -1.0f * type_id * tableau[row_i][col_i];
                double coef_inv = 1.0f / coef;
                double value = -1.0f * tableau[0][col_i];

                value *= coef_inv;

                if(coef < 0 && value < upper_bound){
                    upper_bound = value;
                }else if(value > lower_bound){
                    lower_bound = value;
                }
                
            }

            std::pair<double, double> range = std::make_pair(lower_bound, upper_bound);
            std::cout << "var X_" << col << "  de " << lower_bound << " a " << upper_bound << std::endl;

            c_range.push_back(range);
            
        }
    }
}

void Model::analyse(){
    tableau = solution_tableau;

    // right hand
    b_range_calc();
    c_range_calc();
}

void Model::tableau_resize(cstr cstr_new){

    int row_size = tableau[0].size();
    // add new row
    std::vector<double> row_new = cstr_new.exp_coef;

    if(cstr_new.type_id == G_EQ){
        for(int row = var_qnt; row < tableau[0].size(); row++)
            row_new.push_back(0); 
        row_new.push_back(-1); 
        row_new.push_back(0); 

        row_new.push_back(cstr_new.value);

        int col_i = row_size - 1;

        for(int row = 0; row < tableau.size(); row++){
            tableau[row].insert(tableau[row].begin() + col_i, 0);
            tableau[row].insert(tableau[row].begin() + col_i, 0);
        }

    }else { 
        for(int row = var_qnt; row < tableau[0].size(); row++)
            row_new.push_back(0); 
        //row_new.push_back(0); 

        row_new.push_back(cstr_new.value);

        int col_i = row_size - 1;

        for(int row = 0; row < tableau.size(); row++)
            tableau[row].insert(tableau[row].begin() + col_i, 0);
    }

    // update I_index vec
    I_index.push_back(tableau[0].size() - 2);

    if(cstr_new.type_id == G_EQ || cstr_new.type_id == EQ)
        vec_add_vec(tableau[0], row_new, -BIG_M);

    int col_i = row_new.size() - 2;
    row_new[col_i] = 1;

    //clean columns of basic vars
    for(int col = 0; col < tableau[0].size() - 1; col++){

        if(tableau[0][col] == 0 && row_new[col] != 0){
            int row_i;
            for(int row = 0; row < tableau.size(); row++){
                if(tableau[row][col] == 1){
                    row_i = row;
                    break;
                }
            }

            double coef = row_new[col];

            vec_add_vec(row_new, tableau[row_i], -coef);

            break;
        }
    }

    tableau.push_back(row_new);


}

void Model::analyse_reopt(){
    if(cstr_vec_new.empty()){
        std::cout << "No constraints added for re-optimal analysis" << std::endl;
        exit(0);
    }else
        tableau = solution_tableau;

    for(int i = 0; i < cstr_vec_new.size(); i++){
        tableau_resize(cstr_vec_new[i]);
    }

    tableau_print();
    solver(tableau);
    
    solution_primal_get();
    solution_dual_get();

    std::cout << "\n\n";
    std::cout << "Funcao objetivo : " << obj_value_get() << std::endl; 
    std::cout << "Solucao primal : "; vec_print_dbl(solution_primal);
    std::cout << "Solucao dual : ";vec_print_dbl(solution_dual);

    // right hand
    b_range_calc();
    c_range_calc();
}

/*
void output_generate(){
    
    char header[200];

    sprintf(header, "========= MODEL =========\n" 
            "\nN_o vars = %d\n" 
            "\n%s ", var_qnt, type.c_str());
            main_func->print();

    printf("\nConstraints :\n");
    for(int i = 0; i < cstr_vec.size(); i++){
        std::vector<double> coef_vec = cstr_vec[i].exp_coef;
        printf("  (%d) ", i);
        for(int j = 0; j < coef_vec.size(); j++){
            if(coef_vec[j] < 0)
                std::cout << " " << coef_vec[j] << " " << main_func->var_name_get(j);
            else if(j != 0)
                std::cout << " + " << coef_vec[j] << " " << main_func->var_name_get(j);
            else
                std::cout << " " << coef_vec[j] << " " << main_func->var_name_get(j);

        }

        if(cstr_vec[i].type_id == EQ)
            std::cout << " = ";
        else if(cstr_vec[i].type_id == L_EQ)
            std::cout << " <= ";
        else if(cstr_vec[i].type_id == G_EQ)
            std::cout << " >= ";

        double value = cstr_vec[i].value;
        std::cout  << value << std::endl;
        
    }

}

*/
void Model::solve(){
    tableau_generate();
    solver(tableau);
    solution_tableau = tableau;

    solution_primal_get();
    solution_dual_get();
    std::cout << "\n\n";
    std::cout << "Funcao objetivo : " << obj_value_get() << std::endl; 
    std::cout << "Solucao primal : "; vec_print_dbl(solution_primal);
    std::cout << "Solucao dual : ";vec_print_dbl(solution_dual);
}

double Model::obj_value_get(){
    return obj_value;
}

Table Model::tableau_get(){
    return tableau;
}

void Model::tableau_print(){
    std::cout << "aqui\n";
    for(int i = 0; i < tableau.size(); i++){
        for(int j = 0; j < tableau[i].size(); j++){
            std::cout << tableau[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "aqui\n";
}

int Model::n_var_get(){
    return var_qnt;
}

int Model::size(){
    return cstr_vec.size();
}

void Model::print(){

    printf("========= MODEL =========\n" 
            "\nN_o vars = %d\n" 
            "\n%s ", var_qnt, type.c_str());
            main_func->print();

    printf("\nConstraints :\n");
    for(int i = 0; i < cstr_vec.size(); i++){
        std::vector<double> coef_vec = cstr_vec[i].exp_coef;
        printf("  (%d) ", i);
        for(int j = 0; j < coef_vec.size(); j++){
            if(coef_vec[j] < 0)
                std::cout << " " << coef_vec[j] << " " << main_func->var_name_get(j);
            else if(j != 0)
                std::cout << " + " << coef_vec[j] << " " << main_func->var_name_get(j);
            else
                std::cout << " " << coef_vec[j] << " " << main_func->var_name_get(j);

        }

        if(cstr_vec[i].type_id == EQ)
            std::cout << " = ";
        else if(cstr_vec[i].type_id == L_EQ)
            std::cout << " <= ";
        else if(cstr_vec[i].type_id == G_EQ)
            std::cout << " >= ";

        double value = cstr_vec[i].value;
        std::cout  << value << std::endl;
        
    }

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

std::string Model::obj_func::var_name_get(int n){
    if(n > var_name.size() - 1){
        std::cout << "Missing some var_name" << std::endl;
        exit(0);
    }
    return var_name[n];
}

int Model::obj_func::size(){
    return var_qnt;
}

std::string Model::obj_func::output_generate(){
    std::string output;

    std::string func = "Z = ";
    std::string opt1;
    std::string opt2;
    std::string opt3;
    
    //std::cout << func;
    //output.append(func);
    output = func;

    for(int i = 0; i < coef.size(); i++){
        std::stringstream coef_stream;
        coef_stream << std::setprecision(3) << coef[i];

        std::string coef_str = coef_stream.str();

        opt1 = " " + coef_str + " " + var_name[i];
        opt2 = " + " + coef_str + " " + var_name[i];
        opt3 = coef_str + " " + var_name[i];

        if(coef[i] < 0){
            //std::cout << opt1;
            output.append(opt1);
        }else if(i != 0){
            //std::cout << " + " << coef[i] << " " << var_name[i];
            //std::cout << opt2;
            output.append(opt2);
        }else{
            //std::cout << coef[i] << " " << var_name[i];
            //std::cout << opt3;
            output.append(opt3);
        }
    }
    //std::cout << std::endl;
    output.append("\n");

    //std::cout << output;

    return output;

}

void Model::obj_func::print(){
    std::cout << output_generate();
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
