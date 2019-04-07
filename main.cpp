#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <omp.h>
#include <zconf.h>

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

vector<double>
solveTDM(vector<double> &vectorA, vector<double> &vectorB, vector<double> &vectorC, vector<double> &vectorD) {

    vector<double> acVector(vectorA), bcVector(vectorB), ccVector(vectorC), dcVector(vectorD);
    double mc;
    int len = vectorD.size();

    vector<double> result(len, 0);

    for (int i = 1; i < len; i++) {
        mc = acVector[i] / bcVector[i - 1];
        bcVector[i] = bcVector[i] - mc * ccVector[i - 1];
        dcVector[i] = dcVector[i] - mc * dcVector[i - 1];
    }

    result[len - 1] = dcVector[len - 1] / bcVector[len - 1];

    for (int i = len - 2; i > -1; i--) {
        result[i] = (dcVector[i] - (ccVector[i] * result[i + 1])) / bcVector[i];
    }

    return result;
}

int main() {

    vector<double> aVector, bVector, cVector, dVector, result;

    readVectors("../date.in", aVector, bVector, cVector, dVector);

    aVector.insert(aVector.begin(), 0);
    cVector.insert(cVector.end(), 0);

    cout << "Start " << endl;
    auto start = std::chrono::high_resolution_clock::now();

    result = solveTDM(aVector, bVector, cVector, dVector);

    auto finish = std::chrono::high_resolution_clock::now();
    cout << "Finish " << endl;

    std::chrono::duration<double> elapsed = finish - start;

    cout << "Tmie :" << elapsed.count() << endl;

    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }

    return 0;
}