#include <vector>
#include <cfloat>
#include <iostream>

typedef std::vector<std::vector<double>> Tableau;

// to solve tableau
int pivot_column_identify(std::vector<double> obj_func);
int pivot_line_identify(Tableau & tableau, int pivot_column);
void tableau_print(const Tableau & tableau);
void pivot_column_clean(Tableau & tableau, int line, int column);
void simplex_solve(Tableau tableau);
