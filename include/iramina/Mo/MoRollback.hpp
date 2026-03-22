#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace iramina {

template <class State>
class MoRollback {
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

public:
    explicit MoRollback(int n) : n_(n) {}

    void AddQuery(int l, int r) {
        queries_.push_back({l, r, static_cast<int>(queries_.size())});
    }

    void Run(State& state, std::vector<answer_type>& ans) const {
        if (queries_.empty()) return;

        int q = queries_.size();
        int block_size = std::max<int>(1, n_ / std::sqrt(q));

        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);

        // Rollback Mo では R は単調増加である必要があるため、スネークソートは行わない
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int block_a = queries_[a].l / block_size;
            int block_b = queries_[b].l / block_size;
            if (block_a != block_b) return block_a < block_b;
            return queries_[a].r < queries_[b].r;
        });

        // まずブロック幅以下の全クエリを処理する（完全に初期状態のStateを使うため）
        state.Reset();
        for (int idx = 0; idx < q; ++idx) {
            const auto& qry = queries_[idx];
            if (qry.r - qry.l <= block_size) {
                state.Snapshot();
                for (int i = qry.l; i < qry.r; ++i) {
                    state.AddRight(i);
                }
                ans[qry.id] = state.GetAnswer();
                state.Rollback();
            }
        }

        int current_block = -1;
        int nr = 0;

        for (int idx : order) {
            const auto& qry = queries_[idx];
            
            // すでに処理済み
            if (qry.r - qry.l <= block_size) continue;

            int block = qry.l / block_size;

            // 新しいブロックに入った時の初期化
            if (current_block != block) {
                current_block = block;
                nr = (block + 1) * block_size;
                state.Reset(); // 状態を全てクリアして、基点を nr に設定
            }

            // 右側を qry.r まで伸ばす（この追加は Snapshot を超えて持ち越される）
            while (nr < qry.r) {
                state.AddRight(nr);
                nr++;
            }

            // 現在の状態（右側だけが追加された状態）を保存
            state.Snapshot();

            // 左側を qry.l まで伸ばす
            int nl = (block + 1) * block_size;
            // nl はブロック境界から左へ qry.l まで下がる
            while (nl > qry.l) {
                nl--;
                state.AddLeft(nl);
            }

            // 答えの記録
            ans[qry.id] = state.GetAnswer();

            // 左側の追加をロールバック
            state.Rollback();
        }
    }
};

} // namespace iramina
