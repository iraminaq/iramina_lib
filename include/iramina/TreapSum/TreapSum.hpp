#pragma once
#include <cstdint>
#include <utility>
#include <vector>

template <class Key, class Weight>
class TreapSum {
private:
    struct Node {
        Key key;
        uint64_t pri;
        int cnt;                // この key の個数
        int size;               // 部分木全体の個数（重複展開後）
        Weight self_weight_sum; // この key の weight 総和
        Weight sum;             // 部分木全体の weight 総和
        Node* l;
        Node* r;

        Node(const Key& key_, const Weight& weight_, uint64_t pri_)
            : key(key_),
              pri(pri_),
              cnt(1),
              size(1),
              self_weight_sum(weight_),
              sum(weight_),
              l(nullptr),
              r(nullptr) {}
    };

    Node* root_ = nullptr;
    static inline uint64_t rng_state_ = 0x123456789abcdefULL;

    static uint64_t next_rand() {
        uint64_t z = (rng_state_ += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

    static int node_size(Node* t) {
        return t ? t->size : 0;
    }

    static Weight node_sum(Node* t) {
        return t ? t->sum : Weight{};
    }

    static void pull(Node* t) {
        if (!t) return;
        t->size = node_size(t->l) + t->cnt + node_size(t->r);
        t->sum = node_sum(t->l) + t->self_weight_sum + node_sum(t->r);
    }

    // key < x / key >= x
    static std::pair<Node*, Node*> split_less(Node* t, const Key& x) {
        if (!t) return {nullptr, nullptr};
        if (t->key < x) {
            auto [a, b] = split_less(t->r, x);
            t->r = a;
            pull(t);
            return {t, b};
        } else {
            auto [a, b] = split_less(t->l, x);
            t->l = b;
            pull(t);
            return {a, t};
        }
    }

    // key <= x / key > x
    static std::pair<Node*, Node*> split_less_equal(Node* t, const Key& x) {
        if (!t) return {nullptr, nullptr};
        if (!(x < t->key)) {
            auto [a, b] = split_less_equal(t->r, x);
            t->r = a;
            pull(t);
            return {t, b};
        } else {
            auto [a, b] = split_less_equal(t->l, x);
            t->l = b;
            pull(t);
            return {a, t};
        }
    }

    // max(a) < min(b) を仮定
    static Node* merge(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->pri > b->pri) {
            a->r = merge(a->r, b);
            pull(a);
            return a;
        } else {
            b->l = merge(a, b->l);
            pull(b);
            return b;
        }
    }

    static Node* find_node(Node* t, const Key& key) {
        while (t) {
            if (key < t->key) {
                t = t->l;
            } else if (t->key < key) {
                t = t->r;
            } else {
                return t;
            }
        }
        return nullptr;
    }

    static const Node* find_node(const Node* t, const Key& key) {
        while (t) {
            if (key < t->key) {
                t = t->l;
            } else if (t->key < key) {
                t = t->r;
            } else {
                return t;
            }
        }
        return nullptr;
    }

    static int count_less_impl(const Node* t, const Key& x) {
        if (!t) return 0;
        if (t->key < x) {
            return node_size(t->l) + t->cnt + count_less_impl(t->r, x);
        } else {
            return count_less_impl(t->l, x);
        }
    }

    static int count_less_equal_impl(const Node* t, const Key& x) {
        if (!t) return 0;
        if (!(x < t->key)) {
            return node_size(t->l) + t->cnt + count_less_equal_impl(t->r, x);
        } else {
            return count_less_equal_impl(t->l, x);
        }
    }

    static Weight sum_less_impl(const Node* t, const Key& x) {
        if (!t) return Weight{};
        if (t->key < x) {
            return node_sum(t->l) + t->self_weight_sum + sum_less_impl(t->r, x);
        } else {
            return sum_less_impl(t->l, x);
        }
    }

    static Weight sum_less_equal_impl(const Node* t, const Key& x) {
        if (!t) return Weight{};
        if (!(x < t->key)) {
            return node_sum(t->l) + t->self_weight_sum + sum_less_equal_impl(t->r, x);
        } else {
            return sum_less_equal_impl(t->l, x);
        }
    }

    static void destroy(Node* t) {
        if (!t) return;
        destroy(t->l);
        destroy(t->r);
        delete t;
    }

public:
    struct Entry {
        Key key;
        int cnt;
        Weight weight_sum;
    };

    TreapSum() = default;

    TreapSum(const TreapSum&) = delete;
    TreapSum& operator=(const TreapSum&) = delete;

    ~TreapSum() {
        destroy(root_);
    }

    bool empty() const {
        return root_ == nullptr;
    }

    int size() const {
        return node_size(root_);
    }

    Weight sum_all() const {
        return node_sum(root_);
    }

    void insert(const Key& key, const Weight& weight) {
        auto [a, bc] = split_less(root_, key);
        auto [b, c] = split_less_equal(bc, key);

        if (b) {
            b->cnt += 1;
            b->self_weight_sum += weight;
            pull(b);
        } else {
            b = new Node(key, weight, next_rand());
        }

        root_ = merge(merge(a, b), c);
    }

    // 同じ key に異なる weight がありうる前提なので、
    // 利用者側が削除したい weight を正しく渡す想定
    bool erase_one(const Key& key, const Weight& weight) {
        auto [a, bc] = split_less(root_, key);
        auto [b, c] = split_less_equal(bc, key);

        if (!b) {
            root_ = merge(a, c);
            return false;
        }

        if (b->cnt > 1) {
            b->cnt -= 1;
            b->self_weight_sum -= weight;
            pull(b);
            root_ = merge(merge(a, b), c);
        } else {
            delete b;
            root_ = merge(a, c);
        }
        return true;
    }

    bool contains(const Key& key) const {
        return find_node(root_, key) != nullptr;
    }

    int count(const Key& key) const {
        const Node* t = find_node(root_, key);
        return t ? t->cnt : 0;
    }

    Weight sum_key(const Key& key) const {
        const Node* t = find_node(root_, key);
        return t ? t->self_weight_sum : Weight{};
    }

    int count_less(const Key& x) const {
        return count_less_impl(root_, x);
    }

    int count_less_equal(const Key& x) const {
        return count_less_equal_impl(root_, x);
    }

    Weight sum_less(const Key& x) const {
        return sum_less_impl(root_, x);
    }

    Weight sum_less_equal(const Key& x) const {
        return sum_less_equal_impl(root_, x);
    }

    int count_range(const Key& l, const Key& r) const {
        return count_less(r) - count_less(l);
    }

    Weight sum_range(const Key& l, const Key& r) const {
        return sum_less(r) - sum_less(l);
    }

    // 0-indexed kth
    bool kth(int k, Entry& out) const {
        if (k < 0 || k >= size()) return false;
        const Node* t = root_;
        while (t) {
            int ls = node_size(t->l);
            if (k < ls) {
                t = t->l;
            } else if (k < ls + t->cnt) {
                out = Entry{t->key, t->cnt, t->self_weight_sum};
                return true;
            } else {
                k -= ls + t->cnt;
                t = t->r;
            }
        }
        return false;
    }

    bool lower_bound(const Key& x, Entry& out) const {
        const Node* cur = root_;
        const Node* ans = nullptr;
        while (cur) {
            if (!(cur->key < x)) {
                ans = cur;
                cur = cur->l;
            } else {
                cur = cur->r;
            }
        }
        if (!ans) return false;
        out = Entry{ans->key, ans->cnt, ans->self_weight_sum};
        return true;
    }

    bool upper_bound(const Key& x, Entry& out) const {
        const Node* cur = root_;
        const Node* ans = nullptr;
        while (cur) {
            if (x < cur->key) {
                ans = cur;
                cur = cur->l;
            } else {
                cur = cur->r;
            }
        }
        if (!ans) return false;
        out = Entry{ans->key, ans->cnt, ans->self_weight_sum};
        return true;
    }

    int order_of_key(const Key& x) const {
        return count_less(x);
    }

    std::vector<Entry> dump() const {
        std::vector<Entry> res;
        auto dfs = [&](auto&& self, const Node* t) -> void {
            if (!t) return;
            self(self, t->l);
            res.push_back(Entry{t->key, t->cnt, t->self_weight_sum});
            self(self, t->r);
        };
        dfs(dfs, root_);
        return res;
    }
};