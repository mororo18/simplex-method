#include <vector>
#include <cfloat>
#include <iostream>

typedef std::vector<std::vector<double>> Tableau;
typedef void (*simplex_ptr)(Tableau & tableau);

// vec funcs
void vec_multiply_scalar(std::vector<double> & vec, double scalar);
void vec_add_vec(std::vector<double> & vec_a, const std::vector<double> & vec_b, double factor);

// to solve tableau
int pivot_column_identify(std::vector<double> obj_func);
int pivot_row_identify(Tableau & tableau, int pivot_column);
void tableau_print(const Tableau & tableau);
void pivot_column_clean(Tableau & tableau, int row, int column);
void simplex_solve(Tableau & tableau);
int dual_simplex_pivot_row_identify(Tableau & tableau);
int dual_simplex_pivot_row_identify(Tableau & tableau, int pivot_row);
void dual_simplex_solve(Tableau & tableau);
void algo_def(Tableau & tableau);
simplex_ptr simplex();
