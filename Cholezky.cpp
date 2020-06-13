#include "Cholezky.h"
std::vector<std::vector<double>> Cholesky_Decomposition(std::vector<std::vector<double>> matrix) //, const int n)
{
    int n = matrix.size();
    std::vector<double> inner(n);
    std::vector<std::vector<double>> lower(n, inner);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            int sum = 0;

            if (j == i) // summation for diagnols 
            {
                for (int k = 0; k < j; k++)
                    sum += pow(lower[j][k], 2);
                lower[j][j] = sqrt(matrix[j][j] -
                    sum);
            }
            else {

                // Evaluating L(i, j) using L(j, j) 
                for (int k = 0; k < j; k++)
                    sum += (lower[i][k] * lower[j][k]);
                lower[i][j] = (matrix[i][j] - sum) /
                    lower[j][j];
            }
        }
    }
    return lower;
}