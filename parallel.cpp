#include <cmath>
#include <iostream>
#include <omp.h>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

void readVectors(string file, vector<double> &vectorA, vector<double> &vectorB, vector<double> &vectorC,
                 vector<double> &vectorX) {

    ifstream f;
    double x;
    int n;

    f.open(file);

    f >> n;

    for (int i = 0; i < n - 1; i++) {
        f >> x;
        vectorA.push_back(x);
    }

    for (int i = 0; i < n; i++) {
        f >> x;
        vectorB.push_back(x);
    }

    for (int i = 0; i < n - 1; i++) {
        f >> x;
        vectorC.push_back(x);
    }

    for (int i = 0; i < n; i++) {
        f >> x;
        vectorX.push_back(x);
    }

    f.close();
}


int main(int argc, char *argv[]) {
    int i, j, k;
    int index1, index2, offset;
    double alpha, gamma;
    int size;

    vector<double> aVector, bVector, cVector, dVector, result;

    readVectors("./date.in", aVector, bVector, cVector, dVector);


    size = bVector.size();

    /* Part 1 */
    double *x = new double[size];
    for (i = 0; i < size; i++)
        x[i] = 0.0;

    //initialize matrix with 0
    double *F = new double[size];
    double **A = new double *[size];
    for (i = 0; i < size; i++) {
        A[i] = new double[size];
        for (j = 0; j < size; j++)
            A[i][j] = 0.;
        F[i] = dVector[i];
    }

    //construct the matrix
    A[0][0] = bVector[0];
    A[0][1] = cVector[0];
    A[size - 1][size - 2] = aVector[size - 2];
    A[size - 1][size - 1] = bVector[size - 1];
    for (i = 1; i < size - 1; i++) {
        A[i][i] = bVector[i];
        A[i][i - 1] = aVector[i - 1];
        A[i][i + 1] = cVector[i];
    }

    auto start = chrono::high_resolution_clock::now();
    /* Part 2 */
    for (i = 0; i < log2(size + 1) - 1; i++) {
        for (j = pow(2, i + 1) - 1; j < size; j = j + pow(2, i + 1)) {
            offset = pow(2, i);
            index1 = j - offset;
            index2 = j + offset;
            alpha = A[j][index1] / A[index1][index1];
            gamma = A[j][index2] / A[index2][index2];

#pragma omp parallel for
            for (k = 0; k < size; k++) {
                A[j][k] -= (alpha * A[index1][k] + gamma * A[index2][k]);
            }

            F[j] -= (alpha * F[index1] + gamma * F[index2]);
        }
    }


/* Part 3 */
    int index = (size - 1) / 2;

    x[index] = F[index] / A[index][index];

    for (i = log2(size + 1) - 2; i >= 0; i--) {
        for (j = pow(2, i + 1) - 1; j < size; j = j + pow(2, i + 1)) {

            offset = pow(2, i);
            index1 = j - offset;
            index2 = j + offset;
            x[index1] = F[index1];
            x[index2] = F[index2];

            for (k = 0; k < size; k++) {
                if (k != index1)
                    x[index1] -= A[index1][k] * x[k];
                if (k != index2)
                    x[index2] -= A[index2][k] * x[k];
            }
            x[index1] = x[index1] / A[index1][index1];
            x[index2] = x[index2] / A[index2][index2];
        }
    }

    auto finish = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = finish - start;

    cout << "Tmie :" << elapsed.count() << " ms" << endl;

    if (argc > 1 && argv[1] == "print") {
        for (i = 0; i < size; i++) {
            cout << x[i] << endl;
        }
    }

    //cleanup
    delete[] x;
    delete[] F;

    for (i = 0; i < size; i++)
        delete[] A[i];

    delete[]A;

    return 0;

}
