#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

double task1(double x) {
    double y;

    if (x <= -6) {
        y = -14 - 2 * x;
    }
    else if (x > -6 && x <= -2) {
        y = sqrt(4 - pow(x+4, 2)) - 2;
    }
    else if (x > -2 && x <= 2) {
        y = log2(2+x);
    }
    else if (x > 2 && x <= 4) {
        y = sqrt(1 - pow(x-3, 2));
    }
    else return 0;

    return y;
}

int main() {
    double x_begin = -7.0;
    double x_end = 5.0;
    double dx = 1.0;

    cout << "x\ty" << endl;
    for (double x = x_begin; x <= x_end; x += dx) {
        double y = task1(x);
        cout << fixed << setprecision(2) << x << "\t" << y << endl;
    }

    return 0;
}
