#include <bits/stdc++.h>
using namespace std;

// ===== MergeSortTree 本体ここから =====
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

    int count(int l, int r, const Key& xl, const Key& xr) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xl) - root.keys.begin());
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xr) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, il, ir);
    }

    Key sum(int l, int r, const Key& xl, const Key& xr) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xl) - root.keys.begin());
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), xr) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, il, ir);
    }

    Key sum_all(int l, int r) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        return sum_impl(1, 0, size_, l, r, 0, (int)root.keys.size());
    }

    int count_less(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, 0, ir);
    }

    int count_less_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, 0, ir);
    }

    int count_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return count_impl(1, 0, size_, l, r, il, ir);
    }

    int count_greater(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;
        return (r - l) - count_less_equal(l, r, x);
    }

    int count_greater_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return 0;
        return (r - l) - count_less(l, r, x);
    }

    Key sum_less(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int ir = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, 0, ir);
    }

    Key sum_less_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, 0, ir);
    }

    Key sum_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};

        const NodeData& root = seg_[1];
        const int il = (int)(std::lower_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());
        const int ir = (int)(std::upper_bound(root.keys.begin(), root.keys.end(), x) - root.keys.begin());

        return sum_impl(1, 0, size_, l, r, il, ir);
    }

    Key sum_greater(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};
        return sum_all(l, r) - sum_less_equal(l, r, x);
    }

    Key sum_greater_equal(int l, int r, const Key& x) const {
        if (l < 0) l = 0;
        if (r > n_) r = n_;
        if (l >= r) return Key{};
        return sum_all(l, r) - sum_less(l, r, x);
    }
};
// ===== MergeSortTree 本体ここまで =====

static std::mt19937 rng(0);

template <class T>
int brute_count(const vector<T>& a, int l, int r, T xl, T xr) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return 0;
    int res = 0;
    for (int i = l; i < r; ++i) {
        if (xl <= a[i] && a[i] < xr) ++res;
    }
    return res;
}

template <class T>
T brute_sum(const vector<T>& a, int l, int r, T xl, T xr) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return T{};
    T res = T{};
    for (int i = l; i < r; ++i) {
        if (xl <= a[i] && a[i] < xr) res += a[i];
    }
    return res;
}

template <class T>
int brute_count_less(const vector<T>& a, int l, int r, T x) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return 0;
    int res = 0;
    for (int i = l; i < r; ++i) if (a[i] < x) ++res;
    return res;
}

template <class T>
int brute_count_less_equal(const vector<T>& a, int l, int r, T x) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return 0;
    int res = 0;
    for (int i = l; i < r; ++i) if (a[i] <= x) ++res;
    return res;
}

template <class T>
int brute_count_equal(const vector<T>& a, int l, int r, T x) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return 0;
    int res = 0;
    for (int i = l; i < r; ++i) if (a[i] == x) ++res;
    return res;
}

template <class T>
T brute_sum_less(const vector<T>& a, int l, int r, T x) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return T{};
    T res = T{};
    for (int i = l; i < r; ++i) if (a[i] < x) res += a[i];
    return res;
}

template <class T>
T brute_sum_less_equal(const vector<T>& a, int l, int r, T x) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return T{};
    T res = T{};
    for (int i = l; i < r; ++i) if (a[i] <= x) res += a[i];
    return res;
}

template <class T>
T brute_sum_equal(const vector<T>& a, int l, int r, T x) {
    l = max(l, 0);
    r = min(r, (int)a.size());
    if (l >= r) return T{};
    T res = T{};
    for (int i = l; i < r; ++i) if (a[i] == x) res += a[i];
    return res;
}

template <class T>
void assert_eq(const T& got, const T& expected, const string& name) {
    if (got != expected) {
        cerr << "Assertion failed: " << name << "\n";
        cerr << "got      = " << got << "\n";
        cerr << "expected = " << expected << "\n";
        exit(1);
    }
}

void test_small_fixed() {
    vector<long long> a = {5, 1, 4, 1, 3, 9, 2, 6};
    MergeSortTree<long long> mst(a);

    assert_eq(mst.size(), (int)a.size(), "size");

    assert_eq(mst.count(0, 8, 1LL, 5LL), 5, "count [0,8) [1,5)");
    assert_eq(mst.sum(0, 8, 1LL, 5LL), 11LL, "sum [0,8) [1,5)");
    assert_eq(mst.sum_all(0, 8), 31LL, "sum_all [0,8)");

    assert_eq(mst.count_less(0, 8, 4LL), 4, "count_less");
    assert_eq(mst.count_less_equal(0, 8, 4LL), 5, "count_less_equal");
    assert_eq(mst.count_equal(0, 8, 1LL), 2, "count_equal");
    assert_eq(mst.count_greater(0, 8, 4LL), 3, "count_greater");
    assert_eq(mst.count_greater_equal(0, 8, 4LL), 4, "count_greater_equal");

    assert_eq(mst.sum_less(0, 8, 4LL), 1LL + 1LL + 3LL + 2LL, "sum_less");
    assert_eq(mst.sum_less_equal(0, 8, 4LL), 1LL + 4LL + 1LL + 3LL + 2LL, "sum_less_equal");
    assert_eq(mst.sum_equal(0, 8, 1LL), 2LL, "sum_equal");
    assert_eq(mst.sum_greater(0, 8, 4LL), 5LL + 9LL + 6LL, "sum_greater");
    assert_eq(mst.sum_greater_equal(0, 8, 4LL), 5LL + 4LL + 9LL + 6LL, "sum_greater_equal");
}

void test_edge_cases() {
    {
        vector<long long> a = {};
        MergeSortTree<long long> mst(a);
        assert_eq(mst.size(), 0, "empty size");
        assert_eq(mst.count(0, 0, 0LL, 10LL), 0, "empty count");
        assert_eq(mst.sum(0, 0, 0LL, 10LL), 0LL, "empty sum");
        assert_eq(mst.sum_all(0, 0), 0LL, "empty sum_all");
    }
    {
        vector<long long> a = {7};
        MergeSortTree<long long> mst(a);
        assert_eq(mst.count(0, 1, 0LL, 10LL), 1, "single count");
        assert_eq(mst.sum(0, 1, 0LL, 10LL), 7LL, "single sum");
        assert_eq(mst.count_equal(0, 1, 7LL), 1, "single count_equal");
        assert_eq(mst.sum_equal(0, 1, 7LL), 7LL, "single sum_equal");
    }
    {
        vector<long long> a = {2, 2, 2, 2};
        MergeSortTree<long long> mst(a);
        assert_eq(mst.count_equal(0, 4, 2LL), 4, "all same count_equal");
        assert_eq(mst.sum_equal(0, 4, 2LL), 8LL, "all same sum_equal");
        assert_eq(mst.count(0, 4, 2LL, 3LL), 4, "all same count range");
        assert_eq(mst.sum(0, 4, 2LL, 3LL), 8LL, "all same sum range");
    }
}

void test_random() {
    for (int tc = 0; tc < 300; ++tc) {
        int n = rng() % 25;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = (long long)((int)rng() % 21 - 10);
        }

        MergeSortTree<long long> mst(a);

        for (int q = 0; q < 300; ++q) {
            int l = (n == 0 ? 0 : (int)rng() % (n + 1));
            int r = (n == 0 ? 0 : (int)rng() % (n + 1));
            if (l > r) swap(l, r);

            long long x = (long long)((int)rng() % 25 - 12);
            long long y = (long long)((int)rng() % 25 - 12);
            if (x > y) swap(x, y);

            {
                int got = mst.count(l, r, x, y);
                int exp = brute_count(a, l, r, x, y);
                if (got != exp) {
                    cerr << "random count failed\n";
                    cerr << "n=" << n << " l=" << l << " r=" << r << " x=" << x << " y=" << y << "\n";
                    exit(1);
                }
            }
            {
                long long got = mst.sum(l, r, x, y);
                long long exp = brute_sum(a, l, r, x, y);
                if (got != exp) {
                    cerr << "random sum failed\n";
                    cerr << "n=" << n << " l=" << l << " r=" << r << " x=" << x << " y=" << y << "\n";
                    exit(1);
                }
            }
            {
                long long got = mst.sum_all(l, r);
                long long exp = 0;
                for (int i = max(l, 0); i < min(r, n); ++i) exp += a[i];
                if (got != exp) {
                    cerr << "random sum_all failed\n";
                    cerr << "n=" << n << " l=" << l << " r=" << r << "\n";
                    exit(1);
                }
            }
            {
                int got = mst.count_less(l, r, x);
                int exp = brute_count_less(a, l, r, x);
                if (got != exp) {
                    cerr << "random count_less failed\n";
                    exit(1);
                }
            }
            {
                int got = mst.count_less_equal(l, r, x);
                int exp = brute_count_less_equal(a, l, r, x);
                if (got != exp) {
                    cerr << "random count_less_equal failed\n";
                    exit(1);
                }
            }
            {
                int got = mst.count_equal(l, r, x);
                int exp = brute_count_equal(a, l, r, x);
                if (got != exp) {
                    cerr << "random count_equal failed\n";
                    exit(1);
                }
            }
            {
                int got = mst.count_greater(l, r, x);
                int exp = max(0, min(r, n) - max(l, 0)) - brute_count_less_equal(a, l, r, x);
                if (got != exp) {
                    cerr << "random count_greater failed\n";
                    exit(1);
                }
            }
            {
                int got = mst.count_greater_equal(l, r, x);
                int exp = max(0, min(r, n) - max(l, 0)) - brute_count_less(a, l, r, x);
                if (got != exp) {
                    cerr << "random count_greater_equal failed\n";
                    exit(1);
                }
            }
            {
                long long got = mst.sum_less(l, r, x);
                long long exp = brute_sum_less(a, l, r, x);
                if (got != exp) {
                    cerr << "random sum_less failed\n";
                    exit(1);
                }
            }
            {
                long long got = mst.sum_less_equal(l, r, x);
                long long exp = brute_sum_less_equal(a, l, r, x);
                if (got != exp) {
                    cerr << "random sum_less_equal failed\n";
                    exit(1);
                }
            }
            {
                long long got = mst.sum_equal(l, r, x);
                long long exp = brute_sum_equal(a, l, r, x);
                if (got != exp) {
                    cerr << "random sum_equal failed\n";
                    exit(1);
                }
            }
            {
                long long total = 0;
                for (int i = max(l, 0); i < min(r, n); ++i) total += a[i];

                long long got = mst.sum_greater(l, r, x);
                long long exp = total - brute_sum_less_equal(a, l, r, x);
                if (got != exp) {
                    cerr << "random sum_greater failed\n";
                    exit(1);
                }
            }
            {
                long long total = 0;
                for (int i = max(l, 0); i < min(r, n); ++i) total += a[i];

                long long got = mst.sum_greater_equal(l, r, x);
                long long exp = total - brute_sum_less(a, l, r, x);
                if (got != exp) {
                    cerr << "random sum_greater_equal failed\n";
                    exit(1);
                }
            }
        }
    }
}

int main() {
    test_small_fixed();
    test_edge_cases();
    test_random();
    cout << "All tests passed.\n";
    return 0;
}