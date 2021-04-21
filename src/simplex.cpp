#include "simplex.h"

void tableau_print(const Tableau & tableau){
    for(int i = 0; i < tableau.size(); i++){
        for(int j = 0; j < tableau[i].size(); j++){
            std::cout << tableau[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void vec_multiply_scalar(std::vector<double> & vec, double scalar){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] <= DBL_EPSILON && vec[i] >= -DBL_EPSILON) //eq to 0
            continue;

        vec[i] *= scalar;
    }
}

void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor){
    //vec_a - vec_b
    for(int i = 0; i < vec_a.size(); i++){
        vec_a[i] += factor * vec_b[i]; 
    }
}

void pivot_column_clean(Tableau & tableau, int row, int column){
    double d = 1.0f / tableau[row][column];
    
    // divide row by the coeficient of x_i
    vec_multiply_scalar(tableau[row], d);

    for(int i = 0; i < tableau.size(); i++){
        double x_coef = tableau[i][column];
        //std::cout << "line  " << i << " coef  "<< x_coef << std::endl;
        if(x_coef == 0 || i == row) //eq to 0
            continue;

        vec_add_vec(tableau[i], tableau[row], -x_coef); 
    }

}

int simplex_pivot_column_identify(std::vector<double> obj_func){
    double min = 0;
    int min_i = -1;
    for(int i = 0; i < obj_func.size() - 1; i++){
        if(obj_func[i] < min){
            min = obj_func[i];
            min_i = i;
        }
    }

    return min_i;
}

int simplex_pivot_row_identify(Tableau & tableau, int pivot_column){
    //std::vector<double
    double lim;
    double lim_min  = 9999999;
    int row_i;
    bool first = true;
    int k = tableau[0].size() - 1;      //restrictions column
    for(int i = 1; i < tableau.size(); i++){
        double co_ij = tableau[i][pivot_column];
        if(co_ij <= DBL_EPSILON) //less or eq to 0
            continue;
        
        lim = tableau[i][k] / co_ij;
        if(lim < lim_min ){
            lim_min = lim;
            row_i = i;
        }

    }

    return row_i;
}

void simplex_solve(Tableau & tableau){
    int count = 0;
    while(true){

        //tableau_print(tableau);
        //std::cout << std::endl;
        int column = simplex_pivot_column_identify(tableau[0]);

        if(column == -1)
            break;
        int row = simplex_pivot_row_identify(tableau, column);

        pivot_column_clean(tableau, row, column);
    }
}

int dual_simplex_pivot_row_identify(Tableau & tableau){
    double min = 0;
    int row_i = -1;
    int k = tableau[0].size() - 1;      //restrictions column
    for(int row = 1; row < tableau.size(); row++){
        double co_ij = tableau[row][k];

        if(co_ij >= 0) // g_eq then 0
            continue;

        if(co_ij < min){
            min = co_ij;
            row_i = row;
        }
            
    }

    return row_i;
}

int dual_simplex_pivot_row_identify(Tableau & tableau, int pivot_row){
    double lim;
    double lim_min  = 9999999;
    int col_i;
    int f_row = 0;     
    for(int col = 0; col < tableau[0].size() - 1; col++){
        double co_ij = tableau[pivot_row][col];
        if(co_ij >= 0) //geq then 0
            continue;
        
        lim = tableau[f_row][col] / co_ij;
        lim = std::abs(lim);

        if(lim < lim_min ){
            lim_min = lim;
            col_i = col;
        }

    }

    return col_i;
}

void dual_simplex_solve(Tableau & tableau){
    while(true){
        //tableau_print(tableau);
        int row = dual_simplex_pivot_row_identify(tableau);

        //std::cout << "linha " << row << std::endl;
        if(row == -1)
            break;

        int column = dual_simplex_pivot_row_identify(tableau, row);
        //std::cout << "coluna " << column << std::endl;

        pivot_column_clean(tableau, row, column);
    }
}

void algo_def(Tableau & tableau){
    bool simplex = false;
    for(int col = 0; col < tableau[0].size() - 1; col++){
        if(tableau[0][col] < 0)
            simplex = true;
    }

    if(simplex)
        simplex_solve(tableau);
    else
        dual_simplex_solve(tableau);
}

simplex_ptr simplex(){
    return algo_def;
}
