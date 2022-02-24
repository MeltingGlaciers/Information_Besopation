// Datchik.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
//#define M_PI
using namespace std;



double generetor(double z0, double R0, int n,int iter) {

    double prevR = R0;
    double currR;
    double currZ;
    double prevZ = z0;
    double temp = 0;

    for (unsigned int i = 1;i<iter; i++) {

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        prevZ = currZ;
        prevR = currR;

    }

    return currR;
}

double expectedVal(double z0, double R0, int n, int iter) {

    double prevR = R0;
    double currR;
    double currZ;
    double prevZ = z0;
    double temp = 0;
    double sum = 0;

    for (unsigned int i = 1;i<iter; i++) {

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        prevZ = currZ;
        sum += currR;

        
        prevR = currR;

    }
    return sum / iter;

}

double dispersion(double z0, double R0, int n, int iter) {

    double eV = expectedVal(z0, R0, n, iter);

    double prevR = R0;
    double currR;
    double currZ;
    double prevZ = z0;
    double temp = 0;
    double sum = 0;

    for (unsigned int i = 1; i < iter; i++) {

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        prevZ = currZ;
        sum += pow(currR-eV,2);


        prevR = currR;

    }

    return sum / iter;

}

int* freqDiag(double z0, double R0, int n, int iter, int split) {

    int *freq = new int[split];
    memset(freq, 0, split*4);

    double prevR = R0;
    double currR;
    double currZ;
    double prevZ = z0;
    double temp = 0;

    for (unsigned int i = 1; i < iter; i++) {

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        prevZ = currZ;
        //cout << currR << endl;
        freq[(int)(currR * split)]++;


        prevR = currR;

    }

    return freq;

}

double hyDiag(double z0, double R0, int n, int iter, int split) {

    int *freq = freqDiag(z0, R0, n, iter, split);
    double u = iter / split;

    double hy = 0;

    for (int i = 0; i < split; i++) {

        hy += pow(freq[i] - u, 2) / u;

    }

    return hy;

}

int main() 
{

    setlocale(LC_ALL, "Russian");

    int split=20;
    double z0 = 0.011;
    double R0 = 0;
    int n = 4;
    int iter = 1000;

    cout << "Результаты тестов при \nz0 = " << z0 << "\nR0 = " << R0 << "\nn = " << n << "\nКоличестве итераций равным " << iter << ", при разбиении " << split << endl << endl;

    cout << "Мат. Ожидание: " << expectedVal(z0, R0, n, iter) << endl<<endl;
    cout << "Дисперсия: " << dispersion(z0, R0, n, iter)<<endl << endl;
    cout << "Частотное распределение: " << endl;

    int* freq = freqDiag(z0, R0, n, iter, split);
    for (int i = 0; i < split; i++) {

        cout << (double)i / split << " " << freq[i] << endl;
    }
    cout << endl;
    cout<<"Хи квадрат: "<<hyDiag(z0, R0, n, iter, split);

    
}
