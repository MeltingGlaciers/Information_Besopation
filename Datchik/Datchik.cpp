// Datchik.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <map>
#include <vector>
#include <string>
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

vector<int> find_l(double z0, double R0, int n) {

    vector<int> resL;

    double prevR = R0;
    double currR;
    double currZ;
    double prevZ = z0;
    double temp = 0;
    map<float, int> map;
    int i=1;
    while (true) {

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        
        if (map.count(currR) > 0) {
            resL.push_back(i);
            resL.push_back(resL[0] - map[currR]);
            return resL;
        }
            
        map.insert(pair<float, int>(currR,i));

        
        prevZ = currZ;
        prevR = currR;
        i++;
        

    }

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

vector<vector<int>> star_sky(double z0, double R0, int n, int iter, int split) {

    vector<vector<int>> freq;
    for (int k = 0; k < split; k++){

        freq.push_back(vector<int>(split, 0));
    }

    double prevR = R0;
    double currR;
    double currZ;
    double prevZ = z0;
    double temp = 0;

    for (unsigned int i = 1; i < iter/2; i++) {

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        prevZ = currZ;
        prevR = currR;

        currZ = prevZ + pow(10, -n);
        currR = modf((prevR / currZ) + M_PI, &temp);
        prevZ = currZ;

        freq.at((int)(prevR * split)).at((int)(currR * split))++;

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
    double R0 = 0.03;
    int n = 10;
    int iter = 2000;

    cout << "Результаты тестов при \nz0 = " << z0 << "\nR0 = " << R0 << "\nn = " << n << "\nКоличестве итераций равным " << iter << ", при разбиении " << split << endl << endl;
    vector<int> res = find_l(z0, R0, n);
    cout << "L: " << res.at(0) << endl;
    cout << "l: " << res.at(1) << endl;
    cout << "Мат. Ожидание: " << expectedVal(z0, R0, n, iter) << endl<<endl;
    cout << "Дисперсия: " << dispersion(z0, R0, n, iter)<<endl << endl;
    cout << "Частотное распределение: " << endl;
    string hist;
    int* freq = freqDiag(z0, R0, n, iter, split);
    for (int i = 0; i < split; i++) {

        cout << (double)i / split << " " << freq[i] << endl;
        hist.append(to_string(freq[i]));
        hist.append("\n");
    }
    ofstream file("output.txt");
    file << hist;
    file.close();
    cout << endl;
    cout<<"Хи квадрат: "<<hyDiag(z0, R0, n, iter, split);

    split = split / 4;
    vector<vector<int>> star = star_sky(z0, R0, n, iter, split);

    string starstr;
    for (int i = 0; i < split; i++) {
        for (int j = 0; j < split; j++) {

            starstr.append(to_string(i));
            starstr.append(",");
            starstr.append(to_string(j));
            starstr.append(",");
            starstr.append(to_string(star.at(i).at(j)));
            starstr.append("\n");

        }
    }
    ofstream file2("star.txt");
    file2 << starstr;
    file2.close();
    
}
