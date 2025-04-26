#pragma once

using namespace std;

set<int> RandomGen(int t, int limit);
int bit_length(int num);
int ModularArith(int a, int x, int p);
int EulerFunc(int n);
vector<int> Eratosthenes(int n);
set<int> Divisors(int n);

string Miller(int n, const set<int>& divs, int t);
pair<int, string> MillerNumber(int bit_size, int t, const vector<int>& primes);

int randomR(int f);
double PocklingtonErrProb(set<int>& divs);
pair<string, double> Pocklington(int n, int t, int r, set<int> divs);
int PocklingtonNumber(int bit_size, int t, const vector<int>& primes);

double GenerateXi();
int StateStandardAlgo(int q, int t, double xi = GenerateXi());
