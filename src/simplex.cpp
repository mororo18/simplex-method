#include "simplex.h"

void tableau_print(const Tableau & tableau){
    for(int i = 0; i < tableau.size(); i++){
        for(int j = 0; j < tableau[i].size(); j++){
            std::cout << tableau[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int pivot_column_identify(std::vector<double> obj_func){
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

int pivot_line_identify(Tableau & tableau, int pivot_column){
    //std::vector<double
    double lim;
    double lim_min  = 9999999;
    int line_i;
    bool first = true;
    int k = tableau[0].size() - 1;      //restrictions column
    for(int i = 1; i < tableau.size(); i++){
        double co_ij = tableau[i][pivot_column];
        if(co_ij <= DBL_EPSILON) //less or eq to 0
            continue;
        
        lim = tableau[i][k] / co_ij;
        if(lim < lim_min ){
            lim_min = lim;
            line_i = i;
        }

    }

    return line_i;
}

static void vec_multiply_scalar(std::vector<double> & vec, double scalar){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] <= DBL_EPSILON && vec[i] >= -DBL_EPSILON) //eq to 0
            continue;

        vec[i] *= scalar;
    }
}

static void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor){
    //vec_a - vec_b
    for(int i = 0; i < vec_a.size(); i++){
        vec_a[i] += factor * vec_b[i]; 
    }
}

void pivot_column_clean(Tableau & tableau, int line, int column){
    double d = 1.0f / tableau[line][column];
    
    // divide line by the coeficient of x_i
    vec_multiply_scalar(tableau[line], d);

    for(int i = 0; i < tableau.size(); i++){
        double x_coef = tableau[i][column];
        //std::cout << "line  " << i << " coef  "<< x_coef << std::endl;
        if(x_coef == 0 || i == line) //eq to 0
            continue;

        vec_add_vec(tableau[i], tableau[line], -x_coef); 
    }

}

void simplex_solve(Tableau tableau){
    int count = 0;
    while(true){

        tableau_print(tableau);
        std::cout << std::endl;
        //if(count++ == 3) break;
        int column = pivot_column_identify(tableau[0]);
        //std::cout << "pivot column " << column << std::endl;
        if(column == -1)
            break;
        int line = pivot_line_identify(tableau, column);
        //std::cout << "pivot line " << line << std::endl;

        pivot_column_clean(tableau, line, column);
        //tableau_print(tableau);
    }
}
