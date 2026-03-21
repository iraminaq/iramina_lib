#include <iostream>
#include <vector>
#include <cassert>
#include <random>
#include <chrono>
#include "Mo.hpp"
#include "MoSnake.hpp"
#include "MoHilbert.hpp"
#include "MoNoshi91.hpp"

using namespace std;
using namespace iramina;

// 状態クラスの例：区間内の異なる要素の種類数を数える
struct DistinctElementsState {
    using answer_type = int;
    vector<int> a;
    vector<int> cnt;
    int current_distinct;

    DistinctElementsState(const vector<int>& a) : a(a), cnt(100005, 0), current_distinct(0) {}

    void AddLeft(int index) {
        if (cnt[a[index]] == 0) current_distinct++;
        cnt[a[index]]++;
    }
    void AddRight(int index) {
        if (cnt[a[index]] == 0) current_distinct++;
        cnt[a[index]]++;
    }
    void RemoveLeft(int index) {
        cnt[a[index]]--;
        if (cnt[a[index]] == 0) current_distinct--;
    }
    void RemoveRight(int index) {
        cnt[a[index]]--;
        if (cnt[a[index]] == 0) current_distinct--;
    }

    answer_type GetAnswer() const {
        return current_distinct;
    }
};

void run_test() {
    int n = 1000;
    int q = 1000;
    vector<int> a(n);
    mt19937 rng(12345);
    for (int i = 0; i < n; ++i) {
        a[i] = rng() % 100 + 1;
    }

    vector<pair<int, int>> queries;
    for (int i = 0; i < q; ++i) {
        int l = rng() % (n + 1);
        int r = rng() % (n + 1);
        if (l > r) swap(l, r);
        if (l == r) {
            if (r < n) r++;
            else l--;
        }
        queries.push_back({l, r});
    }

    // 愚直な計算 (Naive)
    vector<int> expected_ans(q);
    for (int i = 0; i < q; ++i) {
        int l = queries[i].first;
        int r = queries[i].second;
        vector<int> cnt(1005, 0);
        int distinct = 0;
        for (int j = l; j < r; ++j) {
            if (cnt[a[j]] == 0) distinct++;
            cnt[a[j]]++;
        }
        expected_ans[i] = distinct;
    }

    // 各Moの実装でテスト
    auto test_mo_variant = [&](const string& name, auto mo_instance) {
        for (int i = 0; i < q; ++i) {
            mo_instance.AddQuery(queries[i].first, queries[i].second);
        }
        DistinctElementsState state(a);
        vector<int> ans(q);
        mo_instance.Run(state, ans);

        for (int i = 0; i < q; ++i) {
            if (ans[i] != expected_ans[i]) {
                cerr << "Error in " << name << " at query " << i 
                     << " (" << queries[i].first << ", " << queries[i].second << ")"
                     << ". Expected " << expected_ans[i] << ", got " << ans[i] << endl;
                exit(1);
            }
        }
        cout << name << " passed." << endl;
    };

    test_mo_variant("Mo (Normal)", Mo<DistinctElementsState>(n));
    test_mo_variant("MoSnake", MoSnake<DistinctElementsState>(n));
    test_mo_variant("MoHilbert", MoHilbert<DistinctElementsState>(n));
    test_mo_variant("MoNoshi91", MoNoshi91<DistinctElementsState>(n));
}

int main() {
    run_test();
    cout << "All tests passed!" << endl;
    return 0;
}
