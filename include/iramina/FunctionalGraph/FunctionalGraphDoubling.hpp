#pragma once

#include <vector>

namespace iramina {

template<class T>
struct FunctionalGraphSumMonoid {
    using value_type = T;
    static value_type op(value_type a, value_type b) {
        return a + b;
    }
    static value_type e() {
        return T(0);
    }
};

template<class Monoid = FunctionalGraphSumMonoid<long long>>
struct FunctionalGraphDoubling {
    using T = typename Monoid::value_type;

    struct Result {
        int v;
        T prod;
    };

    int n;
    int log;
    std::vector<std::vector<int>> nxt;
    std::vector<std::vector<T>> acc;

    FunctionalGraphDoubling() : n(0), log(0) {}

    FunctionalGraphDoubling(
        const std::vector<int>& to,
        const std::vector<T>& one_step_value,
        long long max_k
    ) {
        init(to, one_step_value, max_k);
    }

    void init(
        const std::vector<int>& to,
        const std::vector<T>& one_step_value,
        long long max_k
    ) {
        n = to.size();
        
        // max_k = 0 の場合でも最低1段は持つ
        log = 1;
        while (log < 62 && (1LL << log) <= max_k) {
            ++log;
        }

        // 行の確保および初期化
        nxt.assign(log, std::vector<int>(n));
        acc.assign(log, std::vector<T>(n));

        for (int i = 0; i < n; ++i) {
            nxt[0][i] = to[i];
            acc[0][i] = one_step_value[i];
        }

        for (int j = 0; j + 1 < log; ++j) {
            for (int i = 0; i < n; ++i) {
                int next_v = nxt[j][i];
                nxt[j + 1][i] = nxt[j][next_v];
                // 演算は「前半の区間を適用したあとに後半の区間を適用する」順番で行う
                acc[j + 1][i] = Monoid::op(acc[j][i], acc[j][next_v]);
            }
        }
    }

    Result jump(int v, long long k) const {
        T res = Monoid::e();
        int cur = v;
        for (int i = 0; i < log; ++i) {
            if ((k >> i) & 1LL) {
                // 先にここまでの集約値に、今回の追加分を結合する
                res = Monoid::op(res, acc[i][cur]);
                cur = nxt[i][cur];
            }
        }
        return {cur, res};
    }

    int kth_next(int v, long long k) const {
        return jump(v, k).v;
    }

    T path_prod(int v, long long k) const {
        return jump(v, k).prod;
    }
};

} // namespace iramina
