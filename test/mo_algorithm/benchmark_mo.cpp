#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include "Mo.hpp"
#include "MoSnake.hpp"
#include "MoHilbert.hpp"
#include "MoNoshi91.hpp"

using namespace std;
using namespace iramina;

// 状態は軽量なもの（合計和）にしてアルゴリズム本体（区間走査）のオーバーヘッドを測る
struct SumState {
    using answer_type = long long;
    const vector<int>* a_ptr;
    long long current_sum;

    SumState(const vector<int>& a) : a_ptr(&a), current_sum(0) {}

    inline void AddLeft(int index) { current_sum += (*a_ptr)[index]; }
    inline void AddRight(int index) { current_sum += (*a_ptr)[index]; }
    inline void RemoveLeft(int index) { current_sum -= (*a_ptr)[index]; }
    inline void RemoveRight(int index) { current_sum -= (*a_ptr)[index]; }

    inline answer_type GetAnswer() const { return current_sum; }
};

void run_benchmark(int n, int q, const string& condition_name, const vector<pair<int, int>>& queries, const vector<int>& a) {
    cout << "--- " << condition_name << " (N=" << n << ", Q=" << q << ") ---" << endl;

    auto measure = [&](const string& name, auto mo_instance) {
        for (int i = 0; i < q; ++i) {
            mo_instance.AddQuery(queries[i].first, queries[i].second);
        }
        SumState state(a);
        vector<long long> ans(q);

        auto start = chrono::high_resolution_clock::now();
        mo_instance.Run(state, ans);
        auto end = chrono::high_resolution_clock::now();
        
        // 念のため最適化で消えないように出力に絡ませるが、邪魔なのでハッシュだけ計算
        long long hash = 0;
        for(auto x : ans) hash ^= x;

        double ms = chrono::duration<double, milli>(end - start).count();
        cout << left << setw(15) << name << " : " << fixed << setprecision(2) << ms << " ms (Hash: " << hash << ")" << endl;
    };

    measure("Mo (Normal)", Mo<SumState>(n));
    measure("MoSnake", MoSnake<SumState>(n));
    measure("MoHilbert", MoHilbert<SumState>(n));
    measure("MoNoshi91", MoNoshi91<SumState>(n));
    cout << endl;
}

int main() {
    int n = 200000;
    int q = 200000;
    
    mt19937 rng(42);
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = rng() % 1000;
    }

    // 1. Completely Random
    {
        vector<pair<int, int>> queries(q);
        for(int i=0; i<q; ++i) {
            int l = rng() % n;
            int r = rng() % n;
            if(l > r) swap(l, r);
            if(l == r) r++;
            if(r > n) r = n;
            queries[i] = {l, r};
        }
        run_benchmark(n, q, "Random Queries", queries, a);
    }

    // 2. Short Intervals (length <= 100)
    {
        vector<pair<int, int>> queries(q);
        for(int i=0; i<q; ++i) {
            int l = rng() % n;
            int r = l + (rng() % 100) + 1;
            if(r > n) r = n;
            queries[i] = {l, r};
        }
        run_benchmark(n, q, "Short Intervals (L ~ R)", queries, a);
    }

    // 3. Sliding Window like
    {
        vector<pair<int, int>> queries(q);
        int current_l = 0;
        int current_r = 1000;
        for(int i=0; i<q; ++i) {
            current_l += (rng() % 3) - 1; 
            current_r += (rng() % 3) - 1;
            if(current_l < 0) current_l = 0;
            if(current_r > n) current_r = n;
            if(current_l >= current_r) current_r = current_l + 1;
            queries[i] = {current_l, current_r};
        }
        run_benchmark(n, q, "Sliding Window like", queries, a);
    }
    
    // 4. Two clusters (e.g. left half and right half)
    {
        vector<pair<int, int>> queries(q);
        for(int i=0; i<q; ++i) {
            if(rng() % 2 == 0) {
                int l = rng() % (n/2);
                int r = rng() % (n/2);
                if(l > r) swap(l, r);
                if(l == r) r++;
                queries[i] = {l, r};
            } else {
                int l = n/2 + rng() % (n/2);
                int r = n/2 + rng() % (n/2);
                if(l > r) swap(l, r);
                if(l == r) r++;
                if(r>n) r=n;
                queries[i] = {l, r};
            }
        }
        run_benchmark(n, q, "Two Clusters", queries, a);
    }

    return 0;
}
