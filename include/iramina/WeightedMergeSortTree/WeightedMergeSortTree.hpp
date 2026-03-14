#pragma once
#include <algorithm>
#include <vector>

template <class Key, class Weight>
class WeightedMergeSortTree {
public:
    struct Elem {
        Key key;
        Weight weight;
    };

private:
    struct NodeData {
        std::vector<Key> keys;
        std::vector<Weight> prefix_weight_sum;  // size = keys.size() + 1
        std::vector<int> to_left;               // size = keys.size() + 1
    };

    int n_ = 0;
    int size_ = 1;
    std::vector<NodeData> seg_;

    static int ceil_pow2(int n) {
        int x = 1;
        while (x < n) x <<= 1;
        return x;
    }

    static NodeData make_leaf(const Elem& e) {
        NodeData node;
        node.keys.push_back(e.key);

        node.prefix_weight_sum.resize(2);
        node.prefix_weight_sum[0] = Weight{};
        node.prefix_weight_sum[1] = e.weight;

        node.to_left.resize(2);
        node.to_left[0] = 0;
        node.to_left[1] = 0;  // leaf では未使用

        return node;
    }

    static NodeData merge_node(const NodeData& a, const NodeData& b) {
        NodeData res;
        const int na = (int)a.keys.size();
        const int nb = (int)b.keys.size();
        const int n = na + nb;

        res.keys.resize(n);
        res.prefix_weight_sum.resize(n + 1);
        res.to_left.resize(n + 1);

        int i = 0, j = 0, k = 0;
        res.prefix_weight_sum[0] = Weight{};
        res.to_left[0] = 0;

        while (i < na && j < nb) {
            if (a.keys[i] <= b.keys[j]) {
                res.keys[k] = a.keys[i];
                res.prefix_weight_sum[k + 1] =
                    res.prefix_weight_sum[k] +
                    (a.prefix_weight_sum[i + 1] - a.prefix_weight_sum[i]);
                ++i;
            } else {
                res.keys[k] = b.keys[j];
                res.prefix_weight_sum[k + 1] =
                    res.prefix_weight_sum[k] +
                    (b.prefix_weight_sum[j + 1] - b.prefix_weight_sum[j]);
                ++j;
            }
            ++k;
            res.to_left[k] = i;
        }

        while (i < na) {
            res.keys[k] = a.keys[i];
            res.prefix_weight_sum[k + 1] =
                res.prefix_weight_sum[k] +
                (a.prefix_weight_sum[i + 1] - a.prefix_weight_sum[i]);
            ++i;
            ++k;
            res.to_left[k] = i;
        }

        while (j < nb) {
            res.keys[k] = b.keys[j];
            res.prefix_weight_sum[k + 1] =
                res.prefix_weight_sum[k] +
                (b.prefix_weight_sum[j + 1] - b.prefix_weight_sum[j]);
            ++j;
            ++k;
            res.to_left[k] = i;
        }

        return res;
    }

    int count_impl(int node, int nl, int nr, int ql, int qr, int il, int ir) const {
        if (qr <= nl || nr <= ql) return 0;
        if (ql <= nl && nr <= qr) return ir - il;

        int mid = (nl + nr) >> 1;
        const NodeData& cur = seg_[node];

        int il_l = cur.to_left[il];
        int ir_l = cur.to_left[ir];
        int il_r = il - il_l;
        int ir_r = ir - ir_l;

        int res = 0;
        if (ql < mid) {
            res += count_impl(node << 1, nl, mid, ql, qr, il_l, ir_l);
        }
        if (mid < qr) {
            res += count_impl(node << 1 | 1, mid, nr, ql, qr, il_r, ir_r);
        }
        return res;
    }

    Weight sum_impl(int node, int nl, int nr, int ql, int qr, int il, int ir) const {
        if (qr <= nl || nr <= ql) return Weight{};
        if (ql <= nl && nr <= qr) {
            return seg_[node].prefix_weight_sum[ir] - seg_[node].prefix_weight_sum[il];
        }

        int mid = (nl + nr) >> 1;
        const NodeData& cur = seg_[node];

        int il_l = cur.to_left[il];
        int ir_l = cur.to_left[ir];
        int il_r = il - il_l;
        int ir_r = ir - ir_l;

        Weight res = Weight{};
        if (ql < mid) {
            res += sum_impl(node << 1, nl, mid, ql, qr, il_l, ir_l);
        }
        if (mid < qr) {
            res += sum_impl(node << 1 | 1, mid, nr, ql, qr, il_r, ir_r);
        }
        return res;
    }

public:
    WeightedMergeSortTree() = default;

    explicit WeightedMergeSortTree(const std::vector<Elem>& init) {
        n_ = (int)init.size();
        size_ = ceil_pow2(n_);
        seg_.assign(size_ * 2, NodeData{});

        for (int i = 0; i < n_; ++i) {
            seg_[size_ + i] = make_leaf(init[i]);
        }

        for (int i = size_ - 1; i >= 1; --i) {
            seg_[i] = merge_node(seg_[i << 1], seg_[i << 1 | 1]);
        }
    }

    int size() const {
        return n_;
    }

    // [l, r) の中で xl <= key < xr を満たす要素数
    // 計算量: O(log N)
    int count(int l, int r, const Key& xl, const Key& xr) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xl) - root.keys.begin());
        int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xr) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) の中で xl <= key < xr を満たす要素の weight 和
    // 計算量: O(log N)
    Weight sum(int l, int r, const Key& xl, const Key& xr) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Weight{};

        const NodeData& root = seg_[1];
        int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xl) - root.keys.begin());
        int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xr) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) の中で key < x を満たす要素数
    int count_less(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key <= x を満たす要素数
    int count_less_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key == x を満たす要素数
    int count_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());
        int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) の中で key >= x を満たす要素数
    int count_greater_equal(int l, int r, const Key& x) const {
        return (r - l) - count_less(l, r, x);
    }

    // [l, r) の中で key > x を満たす要素数
    int count_greater(int l, int r, const Key& x) const {
        return (r - l) - count_less_equal(l, r, x);
    }

    // [l, r) の中で key < x を満たす要素の weight 和
    Weight sum_less(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Weight{};

        const NodeData& root = seg_[1];
        int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key <= x を満たす要素の weight 和
    Weight sum_less_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Weight{};

        const NodeData& root = seg_[1];
        int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key == x を満たす要素の weight 和
    Weight sum_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Weight{};

        const NodeData& root = seg_[1];
        int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());
        int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, il, ir);
    }
};