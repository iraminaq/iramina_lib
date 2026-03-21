#pragma once
#include "Mo.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace iramina {

template <class State>
class MoSnake : public Mo<State> {
public:
    // コンストラクタを継承
    using Mo<State>::Mo;

    void Run(State& state, std::vector<typename State::answer_type>& ans) const {
        if (this->queries_.empty()) return;

        int q = this->queries_.size();
        int block_size = std::max<int>(1, this->n_ / std::sqrt(q));

        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);

        // スネーク順（右端のソート方向をブロックごとに反転する）
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int block_a = this->queries_[a].l / block_size;
            int block_b = this->queries_[b].l / block_size;
            if (block_a != block_b) {
                return block_a < block_b;
            }
            if (block_a % 2 == 1) {
                return this->queries_[a].r > this->queries_[b].r;
            } else {
                return this->queries_[a].r < this->queries_[b].r;
            }
        });

        this->RunWithOrder(state, ans, order);
    }
};

} // namespace iramina
