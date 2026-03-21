#pragma once
#include "Mo.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

namespace iramina {

template <class State>
class MoHilbert : public Mo<State> {
private:
    static inline int64_t HilbertOrder(int x, int y, int pow, int rotate) {
        if (pow == 0) return 0;
        int hpow = 1 << (pow - 1);
        int seg = (x < hpow) ? ((y < hpow) ? 0 : 3) : ((y < hpow) ? 1 : 2);
        seg = (seg + rotate) & 3;
        const int rotate_delta[4] = {3, 0, 0, 1};
        int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
        int nrot = (rotate + rotate_delta[seg]) & 3;
        int64_t sub_square_size = int64_t(1) << (2 * pow - 2);
        int64_t ans = seg * sub_square_size;
        int64_t add = HilbertOrder(nx, ny, pow - 1, nrot);
        ans += (seg == 1 || seg == 2) ? add : (sub_square_size - add - 1);
        return ans;
    }

    static inline int64_t FastHilbertOrder(int x, int y) {
        int64_t d = 0;
        for (int s = 1 << 20; s > 0; s >>= 1) {
            int rx = (x & s) > 0;
            int ry = (y & s) > 0;
            d += (int64_t)s * s * ((3 * rx) ^ ry);
            if (ry == 0) {
                if (rx == 1) {
                    x = (1 << 21) - 1 - x;
                    y = (1 << 21) - 1 - y;
                }
                std::swap(x, y);
            }
        }
        return d;
    }

public:
    // コンストラクタを継承
    using Mo<State>::Mo;

    void Run(State& state, std::vector<typename State::answer_type>& ans) const {
        if (this->queries_.empty()) return;

        int q = this->queries_.size();
        std::vector<int> order(q);
        std::iota(order.begin(), order.end(), 0);

        std::vector<int64_t> h_order(q);
        for (int i = 0; i < q; ++i) {
            // N > 2^21 になる場合は別途 pow を計算するロジックを利用するが、
            // 競プロの制約 (N <= 10^6) であれば固定 21 bit で十分
            h_order[i] = FastHilbertOrder(this->queries_[i].l, this->queries_[i].r);
        }

        std::sort(order.begin(), order.end(), [&](int a, int b) {
            return h_order[a] < h_order[b];
        });

        this->RunWithOrder(state, ans, order);
    }
};

} // namespace iramina
