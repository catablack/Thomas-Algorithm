#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

using namespace std;


void readVectors(string file, vector<double> &vectorA, vector<double> &vectorB, vector<double> &vectorC,
                 vector<double> &vectorX) {

    ifstream f;
    double x;

    f.open(file);

    while (f >> x && x != 0) {
        vectorA.push_back(x);
    }

    while (f >> x && x != 0) {
        vectorB.push_back(x);
    }

    while (f >> x && x != 0) {
        vectorC.push_back(x);
    }

    while (f >> x && x != 0) {
        vectorX.push_back(x);
    }

    f.close();
}

void resolveBCVector(vector<double> vectorA, vector<double> &vectorB, vector<double> vectorC, int len) {

    double mc;

    for (int i = 1; i < len; i++) {
        cout<<"Thread BC"<<endl;
        mc = vectorA[i] / vectorB[i - 1];
        vectorB[i] = vectorB[i] - mc * vectorC[i - 1];
    }

    cout<<"Bc Vector"<<endl;

    for (int i = 0; i < vectorB.size(); i++) {
        cout<<vectorB[i]<<endl;
    }
}

void resolveDCVector(vector<double> vectorA, vector<double> vectorB, vector<double> &vectorD, int len) {

    double mc;

    for (int i = 1; i < len; i++) {
        cout<<"Thread DC"<<endl;
        mc = vectorA[i] / vectorB[i - 1];
        vectorD[i] = vectorD[i] - mc * vectorD[i - 1];
    }
}

vector<double> solveTDM(vector<double> &vectorA, vector<double> &vectorB, vector<double> &vectorC, vector<double> &vectorD) {

    vector<double> acVector(vectorA), bcVector(vectorB), ccVector(vectorC), dcVector(vectorD);
    double mc;
    int len = vectorD.size();

    vector<double> result(len, 0);

    thread t1(resolveBCVector,  acVector, ref(bcVector), ccVector, len);
    t1.join();


    cout<<"Normal thread Vector"<<endl;

    for (int i = 0; i < bcVector.size(); i++) {
        cout<<bcVector[i]<<endl;
    }

    thread t2(resolveDCVector,vectorA, vectorB, ref(vectorD), len);
    t2.join();

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

    result = solveTDM(aVector, bVector, cVector, dVector);

    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }

    return 0;
}