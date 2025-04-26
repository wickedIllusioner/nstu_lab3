#pragma once
#include <cstdint>

using namespace std;

set<uint64_t> RandomGen(int t, uint64_t limit);
int bit_length(uint64_t num);
int ModularArith(int a, int x, int p);
int EulerFunc(int n);
vector<int> Eratosthenes(int n);
set<int> Divisors(int n);

string Miller(uint64_t n, const set<int>& divs, int t);
uint64_t MillerNumber(int bit_size, int t, const vector<int>& primes);

uint64_t randomR(uint64_t f);
double PocklingtonErrProb(set<int>& divs);
pair<string, double> Pocklington(uint64_t n, int t, int r, set<int> divs);
uint64_t PocklingtonNumber(int bit_size, int t, const vector<int>& primes);

