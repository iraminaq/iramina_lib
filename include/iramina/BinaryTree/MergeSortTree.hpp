#pragma once
#include <algorithm>
#include <vector>

template <class Key>
class MergeSortTree {
private:
    struct NodeData {
        std::vector<Key> keys;
        // to_left[i] = keys[0..i) のうち左子由来の個数
        // size = keys.size() + 1
        std::vector<int> to_left;
        // prefix_sum[i] = keys[0..i) の和
        // size = keys.size() + 1
        std::vector<Key> prefix_sum;
    };

    int n_ = 0;
    int size_ = 1;
    std::vector<NodeData> seg_;

    static int ceil_pow2(int n) {
        int x = 1;
        while (x < n) x <<= 1;
        return x;
    }

    static NodeData make_leaf(const Key& key) {
        NodeData node;
        node.keys.push_back(key);
        node.to_left = {0, 0};
        node.prefix_sum.resize(2);
        node.prefix_sum[0] = Key{};
        node.prefix_sum[1] = key;
        return node;
    }

    static NodeData merge_node(const NodeData& a, const NodeData& b) {
        NodeData res;
        const int na = (int)a.keys.size();
        const int nb = (int)b.keys.size();
        const int n = na + nb;

        res.keys.resize(n);
        res.to_left.resize(n + 1);
        res.prefix_sum.resize(n + 1);

        int i = 0, j = 0, k = 0;
        res.to_left[0] = 0;
        res.prefix_sum[0] = Key{};

        while (i < na && j < nb) {
            if (a.keys[i] <= b.keys[j]) {
                res.keys[k] = a.keys[i];
                ++i;
            } else {
                res.keys[k] = b.keys[j];
                ++j;
            }
            ++k;
            res.to_left[k] = i;
            res.prefix_sum[k] = res.prefix_sum[k - 1] + res.keys[k - 1];
        }
        while (i < na) {
            res.keys[k] = a.keys[i];
            ++i;
            ++k;
            res.to_left[k] = i;
            res.prefix_sum[k] = res.prefix_sum[k - 1] + res.keys[k - 1];
        }
        while (j < nb) {
            res.keys[k] = b.keys[j];
            ++j;
            ++k;
            res.to_left[k] = i;
            res.prefix_sum[k] = res.prefix_sum[k - 1] + res.keys[k - 1];
        }
        return res;
    }

    int count_impl(int node, int nl, int nr, int ql, int qr, int il, int ir) const {
        if (qr <= nl || nr <= ql) return 0;
        if (ql <= nl && nr <= qr) return ir - il;

        const int mid = (nl + nr) >> 1;
        const NodeData& cur = seg_[node];

        const int il_l = cur.to_left[il];
        const int ir_l = cur.to_left[ir];
        const int il_r = il - il_l;
        const int ir_r = ir - ir_l;

        int res = 0;
        if (ql < mid) {
            res += count_impl(node << 1, nl, mid, ql, qr, il_l, ir_l);
        }
        if (mid < qr) {
            res += count_impl(node << 1 | 1, mid, nr, ql, qr, il_r, ir_r);
        }
        return res;
    }

    Key sum_impl(int node, int nl, int nr, int ql, int qr, int il, int ir) const {
        if (qr <= nl || nr <= ql) return Key{};
        if (ql <= nl && nr <= qr) {
            return seg_[node].prefix_sum[ir] - seg_[node].prefix_sum[il];
        }

        const int mid = (nl + nr) >> 1;
        const NodeData& cur = seg_[node];

        const int il_l = cur.to_left[il];
        const int ir_l = cur.to_left[ir];
        const int il_r = il - il_l;
        const int ir_r = ir - ir_l;

        Key res = Key{};
        if (ql < mid) {
            res += sum_impl(node << 1, nl, mid, ql, qr, il_l, ir_l);
        }
        if (mid < qr) {
            res += sum_impl(node << 1 | 1, mid, nr, ql, qr, il_r, ir_r);
        }
        return res;
    }

public:
    MergeSortTree() = default;

    explicit MergeSortTree(const std::vector<Key>& init) {
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
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xl) - root.keys.begin());
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xr) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) の中で xl <= key < xr を満たす要素の和
    // 計算量: O(log N)
    Key sum(int l, int r, const Key& xl, const Key& xr) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xl) - root.keys.begin());
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xr) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) 全体の和
    // 計算量: O(log N)
    Key sum_all(int l, int r) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        return sum_impl(1, 0, size_, l, r, 0, (int)root.keys.size());
    }

    // [l, r) の中で key < x を満たす要素数
    // 計算量: O(log N)
    int count_less(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key <= x を満たす要素数
    // 計算量: O(log N)
    int count_less_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key == x を満たす要素数
    // 計算量: O(log N)
    int count_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) の中で key > x を満たす要素数
    // 計算量: O(log N)
    int count_greater(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;
        return (r - l) - count_less_equal(l, r, x);
    }

    // [l, r) の中で key >= x を満たす要素数
    // 計算量: O(log N)
    int count_greater_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;
        return (r - l) - count_less(l, r, x);
    }

    // [l, r) の中で key < x を満たす要素の和
    // 計算量: O(log N)
    Key sum_less(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key <= x を満たす要素の和
    // 計算量: O(log N)
    Key sum_less_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, 0, ir);
    }

    // [l, r) の中で key == x を満たす要素の和
    // 計算量: O(log N)
    Key sum_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, il, ir);
    }

    // [l, r) の中で key > x を満たす要素の和
    // 計算量: O(log N)
    Key sum_greater(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};
        return sum_all(l, r) - sum_less_equal(l, r, x);
    }

    // [l, r) の中で key >= x を満たす要素の和
    // 計算量: O(log N)
    Key sum_greater_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};
        return sum_all(l, r) - sum_less(l, r, x);
    }
};