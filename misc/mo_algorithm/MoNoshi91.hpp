#pragma once
#include "Mo.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace iramina {

template <class State>
class MoNoshi91 : public Mo<State> {
public:
    // コンストラクタを継承
    using Mo<State>::Mo;

    void Run(State& state, std::vector<typename State::answer_type>& ans) const {
        if (this->queries_.empty()) return;

        int q = this->queries_.size();
        
        // 最適なブロック幅 B。単純な N / sqrt(Q) よりも定数倍最適化として
        // \sqrt{2/3} などを掛けることが多いが、ここでは N / sqrt(Q) を基準にする
        // (環境や実装方針に合わせて微調整が可能)
        int block_size = std::max<int>(1, std::round(this->n_ / std::max<double>(1.0, std::sqrt(q * 2.0 / 3.0))));

        std::vector<int> order1(q);
        std::iota(order1.begin(), order1.end(), 0);
        std::vector<int> order2(q);
        std::iota(order2.begin(), order2.end(), 0);

        // 1つ目のソート: 通常のスネーク順
        std::sort(order1.begin(), order1.end(), [&](int a, int b) {
            int block_a = this->queries_[a].l / block_size;
            int block_b = this->queries_[b].l / block_size;
            if (block_a != block_b) return block_a < block_b;
            if (block_a % 2 == 1) return this->queries_[a].r > this->queries_[b].r;
            return this->queries_[a].r < this->queries_[b].r;
        });

        // 2つ目のソート: 全ての帯を B/2 だけずらしたスネーク順
        // noshi91氏のブログ記事に基づく最適化
        int offset = block_size / 2;
        std::sort(order2.begin(), order2.end(), [&](int a, int b) {
            int block_a = (this->queries_[a].l + offset) / block_size;
            int block_b = (this->queries_[b].l + offset) / block_size;
            if (block_a != block_b) return block_a < block_b;
            if (block_a % 2 == 1) return this->queries_[a].r > this->queries_[b].r;
            return this->queries_[a].r < this->queries_[b].r;
        });

        // どちらの順序がマンハッタン距離として短いか計算する
        auto calc_cost = [&](const std::vector<int>& order) {
            long long cost = 0;
            int cl = 0, cr = 0;
            for (int idx : order) {
                cost += std::abs(cl - this->queries_[idx].l) + std::abs(cr - this->queries_[idx].r);
                cl = this->queries_[idx].l;
                cr = this->queries_[idx].r;
            }
            return cost;
        };

        long long cost1 = calc_cost(order1);
        long long cost2 = calc_cost(order2);

        // より短い経路長の順序を採用する
        if (cost1 <= cost2) {
            this->RunWithOrder(state, ans, order1);
        } else {
            this->RunWithOrder(state, ans, order2);
        }
    }
};

} // namespace iramina
