#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include <string>
#include <random>

using namespace std;

// Генерация случайных чисел
set<int> RandomGen(int t, int limit) {
    set<int> nums;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(2, limit-1);

    while (nums.size() < t)
        nums.insert(dist(gen));

    return nums;
}

// Вывод количества бит в записи числа
int bit_length(int num) {
    if (num == 0) return 1;
    int length = 0;
    while (num) {
        num >>= 1;
        length++;
    }
    return length;
}

// Модулярная арифметика
int ModularArith(int a, int x, int p) {
    int res = 1;
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
string Miller(int n, const set<int>& divs, int t) {
    // double err_probability = 1 - (double)EulerFunc(n-1)/(n-1);
    if (n <= 1) return "составное";
    if (n <= 3) return "простое";

    // 1. Генерация t различных случайных чисел
    auto rand = RandomGen(t, n);

    // 2. Проверка для каждого случайного числа
    for (int a : rand) {
        if (ModularArith(a, n-1, n) != 1) return "составное";
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
            return "вероятно, составное";
    }

    // 4.
    return "простое";
}



// Построение числа с тестом Миллера
pair<int, string> MillerNumber(int bit_size, int t, const vector<int>& primes) {
    int target_bit = bit_size - 1;
    int target_max = (1 << target_bit) - 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, primes.size() - 1);

    while (true) {
        int m {1};
        while (true) {
            const int q = primes[dist(gen)];
            if (m > (target_max) / q) break;
            m *= q;
        }

        int n = 2*m + 1;
        if (bit_length(n) != bit_size) continue;
        auto n_minus_divs = Divisors(n-1);

        string result = Miller(n, n_minus_divs, t);
        return {n, result};
    }
}

int randomR(int f) {
    random_device rd;
    mt19937_64 gen(rd());

    int f_bits = bit_length(f);
    int r_bits = f_bits - 1;
    int r_min = 1 << (r_bits - 1);
    int r_max = (1 << r_bits) - 1;

    uniform_int_distribution<int> dist(r_min, r_max);
    int r = dist(gen);

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
string Pocklington(int n, int t, int r, set<int> divs) {
    // 1. Генерация t различных случайных чисел
    auto rand = RandomGen(t, n);

    // 2. Проверка для каждого случайного числа
    for (int a : rand) {
        if (ModularArith(a, n-1, n) != 1) return "составное";
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
            return "простое";
    }

    // 4.
    return "вероятно, составное";
}


// Построение числа с помощью теста Поклингтона
int PocklingtonNumber(int bit_size, int t, const vector<int>& primes) {
    int target_bit = bit_size / 2 + 1;
    int target_max = 1 << target_bit;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, primes.size() - 1);

    while (true) {
        int f {1};
        while (true) {
            const int q = primes[dist(gen)];
            if (f > (target_max) / q) break;
            f *= q;
        }
        if (bit_length(f) != target_bit) continue;

        int r = randomR(f);
        int n = r * f + 1;
        auto f_divs = Divisors(f);

        auto result = Pocklington(n, t, r, f_divs);
        if (result != "n - составное" && bit_length(n) == bit_size)
            return n;
    }
}


// Генерация вещественное числа в (0, 1)
double GenerateXi() {
    random_device rd;
    minstd_rand rnd(rd());
    uniform_real_distribution<double> xi(0., 1.);
    return xi(rnd);
}

// ГОСТ: Переход от меньшего простого к большему
int StateStandardAlgo(int q, int t, double xi = GenerateXi()) {
    int u {};

    while (true) {
        // 1. Вычисление n
        double n_double = pow(2, t-1) / q + (pow(2, t-1) * xi) / q;
        int n = static_cast<int>(round(n_double));
        if (n % 2 == 1) n++;

        while (true) {
            // 2. Создание кандидата в простые числа
            int p = (n + u)*q + 1;

            // 3. Проверка на размерность
            if (p > (1 << t)) break;

            // 4. Финальная проверка по условиям
            if ( ModularArith(2, p-1, p) == 1 && ModularArith(2, n+u, p) != 1) {
                return p;
            }
            // 5. В случае неудачи
            u = u + 2;
        }
    }
}
