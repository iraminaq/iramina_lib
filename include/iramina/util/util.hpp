#pragma once

#include <cassert>
#include <utility>

inline __int128 powI64(long long x, long long n){
    __int128 ret = 1;
    __int128 base = x;
    while(n > 0){
        if(n & 1) ret *= base;
        base *= base;
        n >>= 1;
    }
    return ret;
}
inline long long modNormalize(long long x, long long m){
    return (x % m + m)%m;
}
inline __int128 powMod(long long x, long long n, long long mod){
    __int128 ret = 1;
    __int128 base = x;
    while(n > 0){
        if(n & 1) ret = ret * base % mod;
        base = base * base % mod;
        n >>= 1;
    }
    return ret;
}

inline int floorLog2(int x) {
    assert(x > 0);
    return 31 - __builtin_clz(x);
}
inline int floorLog2(long long x) {
    assert(x > 0);
    return 63 - __builtin_clzll(x);
}

inline int ceilLog2(int x){
    assert(x > 0);
    if (x == 1) return 0;
    return 32 - __builtin_clz(x-1);
}
inline int ceilLog2(long long x) {
    assert(x > 0);
    if (x == 1) return 0;
    return 64 - __builtin_clzll(x - 1);
}

int digits(long long x) {
    if (x == 0) return 1;
    int d = 0;
    while (x) {
        ++d;
        x /= 10;
    }
    return d;
}

long long floorDiv(long long a, long long b) {
    assert(b != 0);
    long long q = a / b, r = a % b;
    if (r != 0 && ((r > 0) != (b > 0))) --q;
    return q;
}

long long ceilDiv(long long a, long long b) {
    assert(b != 0);
    long long q = a / b, r = a % b;
    if (r != 0 && ((r > 0) == (b > 0))) ++q;
    return q;
}

template<typename T> bool chMin(T& a, const T& b){if(a > b){a = b; return true;} return false;}
template<typename T> bool chMax(T& a, const T& b){if(a < b){a = b; return true;} return false;}

inline long long C2(long long n){
    if(n<2) return 0;
    return (long long)((__int128)n * (n - 1) / 2);
}

//add 260316
template<typename T> bool chAbs(T& a){if(a < 0){a = -a; return true;} return false;}

//[l, r)
template<typename T> bool inRange(const T& a, const T& l, const T& r){
    return l <= a && a < r;
}


template<class T, class S>
std::pair<T, S> operator+(const std::pair<T,S>& a, const std::pair<T,S>& b) {
    return {a.first + b.first, a.second + b.second};
}

template<class T, class S>
std::pair<T, S> operator-(const std::pair<T,S>& a, const std::pair<T,S>& b) {
    return {a.first - b.first, a.second - b.second};
}

template<class T, class S>
std::pair<T, S> operator*(const std::pair<T,S>& a, const std::pair<T,S>& b) {
    return {a.first * b.first, a.second * b.second};
}

template<class T, class S>
std::pair<T, S> operator/(const std::pair<T,S>& a, const std::pair<T,S>& b) {
    return {a.first / b.first, a.second / b.second};
}


//260321
template <class T>
constexpr bool bit_test(T x, int k) {
    return (x >> k) & T(1);
}

template <class T>
constexpr void bit_set(T& x, int k) {
    x |= (T(1) << k);
}

template <class T>
constexpr void bit_reset(T& x, int k) {
    x &= ~(T(1) << k);
}

template <class T>
constexpr void bit_flip(T& x, int k) {
    x ^= (T(1) << k);
}

template <class T>
constexpr void bit_assign(T& x, int k, bool v) {
    if (v) bit_set(x, k);
    else bit_reset(x, k);
}

template <class T>
constexpr bool bit_any(T x, int mask) {
    return (x & mask) != 0;
}

template <class T>
constexpr int bit_popcount(T x) {
    if constexpr (sizeof(T) <= sizeof(unsigned int)) {
        return __builtin_popcount((unsigned int)x);
    } else if constexpr (sizeof(T) <= sizeof(unsigned long)) {
        return __builtin_popcountl((unsigned long)x);
    } else {
        return __builtin_popcountll((unsigned long long)x);
    }
}

template <class T, class S>
void keySort(vector<T>&A, vector<S>&key){
    int N = A.size();
    vector<pair<S, T>> C;
    for (int i = 0; i < N; i++) {
        C.push_back({key[i], A[i]});
    }

    sort(C.begin(), C.end());

    for (int i = 0; i < N; i++) {
        key[i] = C[i].first;
        A[i] = C[i].second;
    }
}

template<class T, class S>
void secondSort(vector<pair<T, S>>&A){
    struct Cmp {
        bool operator()(const pair<T, S>& a, const pair<T, S>& b) const {
            return a.second < b.second;
        }
    };
    sort(A.begin(), A.end(), Cmp());
}

template<class T, class S>
void ascDescSort(vector<pair<T, S>>&A){
    struct Cmp {
        bool operator()(const pair<T, S>& a, const pair<T, S>& b) const {
            if (a.first != b.first) return a.first < b.first;
            return a.second > b.second;
        }
    };
    sort(A.begin(), A.end(), Cmp());
}