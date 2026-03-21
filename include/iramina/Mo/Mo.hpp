#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace iramina {

template <class State>
class Mo {
public:
    using answer_type = typename State::answer_type;

protected:
    struct Query {
        int l;
        int r;
        int id;
    };

    int n_;
    std::vector<Query> queries_;

    // クエリの処理順序を受け取り、その順で状態を更新しながら答えを記録する
    void RunWithOrder(State& state, std::vector<answer_type>& ans, const std::vector<int>& order) const {
        int nl = 0, nr = 0;
        for (int idx : order) {
            const auto& qry = queries_[idx];
            
            while (nl > qry.l) {
                nl--;
                state.AddLeft(nl);
            }
            while (nr < qry.r) {
                state.AddRight(nr);
                nr++;
            }
            while (nl < qry.l) {
                state.RemoveLeft(nl);
                nl++;
            }
            while (nr > qry.r) {
                state.RemoveRight(nr - 1);
                nr--;
            }

            ans[qry.id] = state.GetAnswer();
        }
    }

public:
    explicit Mo(int n) : n_(n) {}

    void AddQuery(int l, int r) {
        queries_.push_back({l, r, static_cast<int>(queries_.size())});
    }

    void Run(State& state, std::vector<answer_type>& ans) const {
        if (queries_.empty()) return;

        int q = queries_.size();
        int block_size = std::max<int>(1, n_ / std::sqrt(q));

        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);

        // スネーク順（右端のソート方向をブロックごとに反転する高速化版）
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int block_a = queries_[a].l / block_size;
            int block_b = queries_[b].l / block_size;
            if (block_a != block_b) {
                return block_a < block_b;
            }
            if (block_a % 2 == 1) {
                return queries_[a].r > queries_[b].r;
            } else {
                return queries_[a].r < queries_[b].r;
            }
        });

        RunWithOrder(state, ans, order);
    }
};

} // namespace iramina
