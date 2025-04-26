#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include <string>
#include <random>

using namespace std;

// Генерация случайных чисел
set<uint64_t> RandomGen(int t, uint64_t limit) {
    set<uint64_t> nums;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, limit-1);

    while (nums.size() < t)
        nums.insert(dist(gen));

    return nums;
}

// Вывод количества бит в записи числа
int bit_length(uint64_t num) {
    if (num == 0) return 1;
    int length = 0;
    while (num) {
        num >>= 1;
        length++;
    }
    return length;
}

// Модулярная арифметика
uint64_t ModularArith(uint64_t a, uint64_t x, uint64_t p) {
    uint64_t res = 1;
    a %= p;
    while (x > 0) {
        if (x % 2 == 1)
            res = (res * a) % p;
        a = (a * a) % p;
        x /= 2;
    }
    return res;
}

// Функция Эйлера
int EulerFunc(int n) {
    int result = n;
    for (int i = 2; i*i <= n; ++i) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
                result -= result / i;
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

// Решето Ератосфена
vector<int> Eratosthenes(int n) {
    vector<bool> v(n+1, true);
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (v[i]) {
            primes.push_back(i);
            for (int j = i*2; j <= n; j += i) {
                v[j] = false;
            }
        }
    }
    return primes;
}

// Простые множители числа (Уникальные)
set<int> Divisors(int n) {
    set<int> result;

    for (int i = 2; i <= sqrt(n); ++i) {
        while (n % i == 0) {
            result.insert(i);
            n /= i;
        }
    }
    if (n != 1)
        result.insert(n);
    return result;
}

// Тест Миллера
string Miller(uint64_t n, const set<int>& divs, int t) {
    // double err_probability = 1 - (double)EulerFunc(n-1)/(n-1);
    if (n <= 1) return "n - составное";
    if (n <= 3) return "n - простое число";

    // 1. Генерация t различных случайных чисел
    auto rand = RandomGen(t, n);

    // 2. Проверка для каждого случайного числа
    for (int a : rand) {
        if (ModularArith(a, n-1, n) != 1) return "n - составное";
    }

    // 3.
    for (int q : divs) {
        bool flag = true;
        for (int a : rand) {
            if (ModularArith(a, (n-1)/q, n) != 1) {
                flag = false;
                break;
            }
        }
        if (flag)
            return "вероятно, n - составное число";
    }

    // 4.
    return "n - простое число";
}



// Построение числа с тестом Миллера
uint64_t MillerNumber(int bit_size, int t, const vector<int>& primes) {
    int target_bit = bit_size - 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, primes.size() - 1);

    while (true) {
        uint64_t m {1};
        while (true) {
            const int q = primes[dist(gen)];
            if (m > ((1ULL << target_bit) - 1) / q) break;
            m *= q;
        }

        uint64_t n = 2*m + 1;
        auto n_minus_divs = Divisors(n-1);

        string result = Miller(n, n_minus_divs, t);
        if (result != "n - составное")
            return n;
    }
}

uint64_t randomR(uint64_t f) {
    random_device rd;
    mt19937_64 gen(rd());

    int f_bits = bit_length(f);
    int r_bits = f_bits - 1;
    uint64_t r_min = 1ULL << (r_bits - 1);
    uint64_t r_max = (1ULL << r_bits) - 1;

    uniform_int_distribution<uint64_t> dist(r_min, r_max);
    uint64_t r = dist(gen);

    if (r % 2 == 1)
        r -= 1;

    return r;
}

// Расчет вероятности ошибки
// double PocklingtonErrProb(set<int>& divs) {
//     double res {1};
//     for (int q : divs)
//         res *= (1 - (double)(1/q));
//     return 1 - res;
// }

// Тест Поклингтона
string Pocklington(uint64_t n, int t, int r, set<int> divs) {
    // 1. Генерация t различных случайных чисел
    auto rand = RandomGen(t, n);

    // 2. Проверка для каждого случайного числа
    for (int a : rand) {
        if (ModularArith(a, n-1, n) != 1) return "n - составное";
    }

    // 3.
    // double err_probability = PocklingtonErrProb(divs);
    for (int a : rand) {
        bool flag = true;
        for (int q : divs) {
            if (ModularArith(a, (n-1)/q, n) == 1) {
                flag = false;
                break;
            }
        }
        if (flag)
            return "n - простое число";
    }

    // 4.
    return "вероятно, n - составное число";
}


// Построение числа с помощью теста Поклингтона
uint64_t PocklingtonNumber(int bit_size, int t, const vector<int>& primes) {
    int target_bit = bit_size / 2 + 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, primes.size() - 1);

    while (true) {
        uint64_t f {1};
        while (true) {
            const int q = primes[dist(gen)];
            if (f > (1ULL << target_bit) / q) break;
            f *= q;
        }
        if (bit_length(f) != target_bit) continue;

        uint64_t r = randomR(f);
        uint64_t n = r * f + 1;
        auto f_divs = Divisors(f);

        auto result = Pocklington(n, t, r, f_divs);
        if (result != "n - составное" && bit_length(n) == bit_size)
            return n;
    }
}







// pair<uint64_t, uint64_t> f(const vector<int>& primes, int bit_size) {
//     int target_bit = bit_size - 1;
//
//     random_device rd;
//     mt19937 gen(rd());
//     uniform_int_distribution<> dist(0, primes.size() - 1);
//
//     while (true) {
//         uint64_t m {1};
//         while (true) {
//             const int q = primes[dist(gen)];
//             if (m > ((1LL << target_bit) - 1) / q) break;
//             m *= q;
//         }
//
//         if (bit_length(m) == target_bit) {
//             uint64_t n = 2*m + 1;
//             return {n, m};
//         }
//     }
//
// }

