#pragma once

#include <cassert>
namespace ira{
    inline __int128 pow(long long x, long long n){
        __int128 ret = 1;
        __int128 base = x;
        while(n > 0){
            if(n & 1) ret *= base;
            base *= base;
            n >>= 1;
        }
        return ret;
    }
    inline long long mod(long long x, long long m){
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

    long long floor_div(long long a, long long b) {
        assert(b != 0);
        long long q = a / b, r = a % b;
        if (r != 0 && ((r > 0) != (b > 0))) --q;
        return q;
    }

    long long ceil_div(long long a, long long b) {
        assert(b != 0);
        long long q = a / b, r = a % b;
        if (r != 0 && ((r > 0) == (b > 0))) ++q;
        return q;
    }

    template <class C>
    long long ssize(const C& c) {
        return (long long)c.size();
    }

    template<typename T> bool chmin(T& a, T b){if(a > b){a = b; return true;} return false;}
    template<typename T> bool chmax(T& a, T b){if(a < b){a = b; return true;} return false;}

    inline long long C2(long long n){
        if(n<2) return 0;
        return n*(n-1)/2;
    }
}