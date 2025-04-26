#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "header.h"

using namespace std;

template <typename T>
ostream& operator<<(ostream& os, vector<T> v) {
    for (T elem : v) {
        os << elem << " ";
    }
    os << endl;
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, set<T> v) {
    for (T elem : v) {
        os << elem << " ";
    }
    os << endl;
    return os;
}



int main() {
    vector<int> primes = Eratosthenes(500);

    return 0;
}
