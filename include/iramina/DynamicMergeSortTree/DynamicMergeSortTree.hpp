#pragma once
#include <vector>
#include <iramina/TreapSum/TreapSum.hpp>

template <class Key, class Weight>
class DynamicMergeSortTree {
public:
    struct Elem {
        Key key;
        Weight weight;
    };

private:
    int n_ = 0;
    int size_ = 1;
    std::vector<Elem> data_;
    std::vector<TreapSum<Key, Weight>> seg_;

    static int ceil_pow2(int n) {
        int x = 1;
        while (x < n) x <<= 1;
        return x;
    }

    void insert_point(int p, const Elem& e) {
        for (int k = p + size_; k >= 1; k >>= 1) {
            seg_[k].insert(e.key, e.weight);
        }
    }

    void erase_point(int p, const Elem& e) {
        for (int k = p + size_; k >= 1; k >>= 1) {
            seg_[k].erase_one(e.key, e.weight);
        }
    }

public:
    DynamicMergeSortTree() = default;

    explicit DynamicMergeSortTree(const std::vector<Elem>& init)
        : n_((int)init.size()),
          size_(ceil_pow2(n_)),
          data_(init),
          seg_(size_ * 2) {
        for (int i = 0; i < n_; ++i) {
            insert_point(i, data_[i]);
        }
    }

    int size() const {
        return n_;
    }

    Elem get(int p) const {
        return data_[p];
    }

    void set(int p, const Elem& e) {
        const Elem old = data_[p];
        erase_point(p, old);
        insert_point(p, e);
        data_[p] = e;
    }

    void set(int p, const Key& key, const Weight& weight) {
        set(p, Elem{key, weight});
    }

    int count(int l, int r, const Key& xl, const Key& xr) const {
        int res = 0;
        l += size_;
        r += size_;
        while (l < r) {
            if (l & 1) res += seg_[l++].count_range(xl, xr);
            if (r & 1) res += seg_[--r].count_range(xl, xr);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }

    Weight sum(int l, int r, const Key& xl, const Key& xr) const {
        Weight res = Weight{};
        l += size_;
        r += size_;
        while (l < r) {
            if (l & 1) res += seg_[l++].sum_range(xl, xr);
            if (r & 1) res += seg_[--r].sum_range(xl, xr);
            l >>= 1;
            r >>= 1;
        }
        return res;
    }
};