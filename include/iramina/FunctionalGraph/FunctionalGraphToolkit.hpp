#pragma once

#include <vector>
#include <string>
#include <string_view>
#include "iramina/FunctionalGraph/FunctionalGraph.hpp"
#include "iramina/FunctionalGraph/FunctionalGraphDoubling.hpp"

namespace iramina {

template<class Monoid = FunctionalGraphSumMonoid<long long>>
struct FunctionalGraphToolkit {
    using T = typename Monoid::value_type;

    struct Result {
        int v;
        T prod;
    };

    FunctionalGraph fg;
    FunctionalGraphDoubling<Monoid> dbl;

    FunctionalGraphToolkit() {}

    FunctionalGraphToolkit(
        const std::vector<int>& to,
        const std::vector<T>& one_step_value
    ) {
        init(to, one_step_value);
    }

    void init(
        const std::vector<int>& to,
        const std::vector<T>& one_step_value
    ) {
        fg.init(to);
        // int の限界(約2e9)では足りないため、十分に大きい値を max_k とする
        // k <= 10^18 に対応するため 10^18 以上の適当な値を指定する。
        dbl.init(to, one_step_value, 2000000000000000000LL); // 2e18
    }

    Result jump(int v, long long k) const {
        auto r = dbl.jump(v, k);
        return {r.v, r.prod};
    }

    Result jump_decimal(int v, std::string_view k_str) const {
        int start = 0;
        while (start < (int)k_str.length() && k_str[start] == '0') {
            start++;
        }
        std::string_view s = (start == (int)k_str.length()) ? "0" : k_str.substr(start);

        int d = fg.dist_to_cycle_of(v);

        if (s.length() <= 18) {
            long long k_val = 0;
            for (char c : s) {
                k_val = k_val * 10 + (c - '0');
            }
            
            if (k_val <= (long long)d) {
                return jump(v, k_val);
            }
            
            long long rem_k = k_val - d;
            int L = fg.cycle_length_of(v);
            long long q = rem_k / L;
            int r = rem_k % L;

            Result r1 = jump(v, d);
            T cycle_prod = jump(r1.v, L).prod;
            T q_prod = power(cycle_prod, q);
            Result r2 = jump(r1.v, r);

            return {r2.v, Monoid::op(r1.prod, Monoid::op(q_prod, r2.prod))};
        } else {
            // パス部分より明らかに長いので文字列で計算
            std::string rem_k_str = sub_string(std::string(s), d);
            int L = fg.cycle_length_of(v);
            int r = 0;
            std::string q_str = div_string(rem_k_str, L, r);

            Result r1 = jump(v, d);
            T cycle_prod = jump(r1.v, L).prod;
            T q_prod = power_string(cycle_prod, q_str);
            Result r2 = jump(r1.v, r);

            return {r2.v, Monoid::op(r1.prod, Monoid::op(q_prod, r2.prod))};
        }
    }

    int kth_next(int v, long long k) const {
        return jump(v, k).v;
    }

    int kth_next_decimal(int v, std::string_view k) const {
        return jump_decimal(v, k).v;
    }

    T path_prod(int v, long long k) const {
        return jump(v, k).prod;
    }

    T path_prod_decimal(int v, std::string_view k) const {
        return jump_decimal(v, k).prod;
    }

    Result jump_to_cycle(int v) const {
        int d = fg.dist_to_cycle_of(v);
        return jump(v, d);
    }

    int first_cycle_vertex(int v) const {
        return fg.root_cycle_vertex_of(v);
    }

    int cycle_length_of(int v) const {
        return fg.cycle_length_of(v);
    }

    bool is_on_cycle(int v) const {
        return fg.is_on_cycle(v);
    }

private:
    std::string sub_string(std::string s, int d) const {
        int n = s.length();
        for (int i = n - 1; i >= 0 && d > 0; --i) {
            int sub = d % 10;
            d /= 10;
            int val = (s[i] - '0') - sub;
            if (val < 0) {
                val += 10;
                d += 1;
            }
            s[i] = '0' + val;
        }
        int start = 0;
        while (start < n - 1 && s[start] == '0') {
            start++;
        }
        return s.substr(start);
    }

    std::string div_string(std::string_view s, int L, int& rem) const {
        std::string q;
        long long cur = 0;
        for (char c : s) {
            cur = cur * 10 + (c - '0');
            if (!q.empty() || cur >= L) {
                q.push_back((char)('0' + cur / L));
            }
            cur %= L;
        }
        if (q.empty()) q = "0";
        rem = cur;
        return q;
    }

    T power(T x, long long q) const {
        T res = Monoid::e();
        T base = x;
        while (q > 0) {
            if (q & 1) res = Monoid::op(res, base);
            base = Monoid::op(base, base);
            q >>= 1;
        }
        return res;
    }

    T power_string(T x, std::string_view q) const {
        T res = Monoid::e();
        for (char c : q) {
            T next_res = Monoid::e();
            for (int i = 0; i < 10; ++i) {
                next_res = Monoid::op(next_res, res);
            }
            res = next_res;
            
            int d = c - '0';
            T x_d = Monoid::e();
            for (int i = 0; i < d; ++i) {
                x_d = Monoid::op(x_d, x);
            }
            res = Monoid::op(res, x_d);
        }
        return res;
    }
};

} // namespace iramina
