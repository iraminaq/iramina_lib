#pragma once
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace iramina {

template <class State>
class DynamicMo {
public:
    using answer_type = typename State::answer_type;

protected:
    struct Query {
        int l;
        int r;
        int t;
        int id;
    };

    int n_;
    std::vector<Query> queries_;
    int update_count_ = 0;

public:
    explicit DynamicMo(int n) : n_(n) {}

    // 現在の更新回数 t を記録してクエリを追加
    void AddQuery(int l, int r) {
        queries_.push_back({l, r, update_count_, static_cast<int>(queries_.size())});
    }

    // 更新イベントが発行されたことを記録（実際のデータ更新は利用側でState等に保持させる）
    void AddUpdate() {
        update_count_++;
    }

    void Run(State& state, std::vector<answer_type>& ans) const {
        if (queries_.empty()) return;

        int q = queries_.size();
        // ブロックサイズの最適化基準 B = (N^2 * U / Q)^(1/3)
        // update_count_ が 0 の場合にも対応できるように max を取る
        double B_req = std::pow(1.0 * n_ * n_ * std::max(1, update_count_) / std::max(1, q), 1.0 / 3.0);
        int block_size = std::max<int>(1, std::round(B_req));

        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);

        // 3D に拡張した Moのアルゴリズム (Z-order/スネークソートの3D版)
        std::sort(order.begin(), order.end(), [&](int a, int b) {
            int block_al = queries_[a].l / block_size;
            int block_bl = queries_[b].l / block_size;
            if (block_al != block_bl) return block_al < block_bl;

            int block_ar = queries_[a].r / block_size;
            int block_br = queries_[b].r / block_size;
            if (block_ar != block_br) {
                if (block_al % 2 == 1) return block_ar > block_br;
                return block_ar < block_br;
            }

            if ((block_al + block_ar) % 2 == 1) {
                return queries_[a].t > queries_[b].t;
            }
            return queries_[a].t < queries_[b].t;
        });

        int nl = 0, nr = 0, nt = 0;
        for (int idx : order) {
            const auto& qry = queries_[idx];

            while (nt < qry.t) {
                state.ApplyUpdate(nt, nl, nr);
                nt++;
            }
            while (nt > qry.t) {
                nt--;
                state.UndoUpdate(nt, nl, nr);
            }

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
};

} // namespace iramina
