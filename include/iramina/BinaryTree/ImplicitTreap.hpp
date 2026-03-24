#pragma once
#include <bits/stdc++.h>
using namespace std;

// =========================
// default: long long sum / range add / reverse-invariant
// =========================
inline long long implicit_treap_default_op_ll(const long long& a, const long long& b) {
    return a + b;
}
inline long long implicit_treap_default_e_ll() {
    return 0LL;
}
inline long long implicit_treap_default_lift_ll(const long long& x) {
    return x;
}
inline long long implicit_treap_default_mapping_point_ll(const long long& f, const long long& x) {
    return x + f;
}
inline long long implicit_treap_default_mapping_ll(const long long& f, const long long& x, int len) {
    return x + f * len;
}
inline long long implicit_treap_default_composition_ll(const long long& f, const long long& g) {
    // first g, then f
    return f + g;
}
inline long long implicit_treap_default_id_ll() {
    return 0LL;
}
inline long long implicit_treap_default_reverse_prod_ll(const long long& x) {
    return x;
}

template <
    class T = long long,
    class S = long long,
    class F = long long,
    S (*op)(const S&, const S&) = implicit_treap_default_op_ll,
    S (*e)() = implicit_treap_default_e_ll,
    S (*lift)(const T&) = implicit_treap_default_lift_ll,
    T (*mapping_point)(const F&, const T&) = implicit_treap_default_mapping_point_ll,
    S (*mapping)(const F&, const S&, int) = implicit_treap_default_mapping_ll,
    F (*composition)(const F&, const F&) = implicit_treap_default_composition_ll,
    F (*id)() = implicit_treap_default_id_ll,
    S (*reverse_prod)(const S&) = implicit_treap_default_reverse_prod_ll>
class ImplicitTreap {
private:
    struct Node {
        T val;
        S prod;
        F lazy;
        bool has_lazy = false;
        bool rev = false;
        int sz = 1;
        uint32_t pri = 0;
        Node* l = nullptr;
        Node* r = nullptr;
        Node* next_free = nullptr;
    };

    struct NodePool {
        static constexpr int BLOCK_SIZE = 1 << 15;

        vector<unique_ptr<Node[]>> blocks;
        Node* free_head = nullptr;

        Node* allocate_raw() {
            if (!free_head) add_block();
            Node* t = free_head;
            free_head = free_head->next_free;
            return t;
        }

        void release_raw(Node* t) {
            t->next_free = free_head;
            free_head = t;
        }

    private:
        void add_block() {
            unique_ptr<Node[]> block(new Node[BLOCK_SIZE]);
            for (int i = 0; i < BLOCK_SIZE; ++i) {
                block[i].next_free = free_head;
                free_head = &block[i];
            }
            blocks.push_back(std::move(block));
        }
    };

    static NodePool& pool() {
        static NodePool p;
        return p;
    }

    static uint64_t& rng_state() {
        static uint64_t x = 0x9e3779b97f4a7c15ULL;
        return x;
    }

    static uint64_t splitmix64() {
        uint64_t z = (rng_state() += 0x9e3779b97f4a7c15ULL);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
        return z ^ (z >> 31);
    }

    static uint32_t rnd_priority() {
        return static_cast<uint32_t>(splitmix64());
    }

    static int sz(Node* t) {
        return t ? t->sz : 0;
    }

    static S prod_all(Node* t) {
        return t ? t->prod : e();
    }

    static Node* new_node(const T& x) {
        Node* t = pool().allocate_raw();
        t->val = x;
        t->prod = lift(x);
        t->lazy = id();
        t->has_lazy = false;
        t->rev = false;
        t->sz = 1;
        t->pri = rnd_priority();
        t->l = nullptr;
        t->r = nullptr;
        t->next_free = nullptr;
        return t;
    }

    static void apply_lazy(Node* t, const F& f) {
        if (!t) return;
        t->val = mapping_point(f, t->val);
        t->prod = mapping(f, t->prod, t->sz);
        if (t->has_lazy) {
            t->lazy = composition(f, t->lazy);
        } else {
            t->lazy = f;
            t->has_lazy = true;
        }
    }

    static void apply_rev(Node* t) {
        if (!t) return;
        std::swap(t->l, t->r);
        t->prod = reverse_prod(t->prod);
        t->rev ^= 1;
    }

    static void pull(Node* t) {
        if (!t) return;
        t->sz = 1 + sz(t->l) + sz(t->r);
        t->prod = op(op(prod_all(t->l), lift(t->val)), prod_all(t->r));
    }

    static void push(Node* t) {
        if (!t) return;
        if (t->rev) {
            apply_rev(t->l);
            apply_rev(t->r);
            t->rev = false;
        }
        if (t->has_lazy) {
            apply_lazy(t->l, t->lazy);
            apply_lazy(t->r, t->lazy);
            t->lazy = id();
            t->has_lazy = false;
        }
    }

    static Node* merge_node(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->pri > b->pri) {
            push(a);
            a->r = merge_node(a->r, b);
            pull(a);
            return a;
        } else {
            push(b);
            b->l = merge_node(a, b->l);
            pull(b);
            return b;
        }
    }

    static pair<Node*, Node*> split_node(Node* t, int k) {
        if (!t) return {nullptr, nullptr};
        push(t);
        if (k <= sz(t->l)) {
            auto [a, b] = split_node(t->l, k);
            t->l = b;
            pull(t);
            return {a, t};
        } else {
            auto [a, b] = split_node(t->r, k - sz(t->l) - 1);
            t->r = a;
            pull(t);
            return {t, b};
        }
    }

    static void release_subtree(Node* t) {
        if (!t) return;
        vector<Node*> st;
        st.push_back(t);
        while (!st.empty()) {
            Node* v = st.back();
            st.pop_back();
            if (v->l) st.push_back(v->l);
            if (v->r) st.push_back(v->r);
            v->l = nullptr;
            v->r = nullptr;
            v->next_free = nullptr;
            pool().release_raw(v);
        }
    }

    static Node* build_from_vector_linear(const vector<T>& v) {
        if (v.empty()) return nullptr;

        vector<Node*> nodes;
        nodes.reserve(v.size());
        for (const T& x : v) nodes.push_back(new_node(x));

        vector<Node*> st;
        st.reserve(v.size());

        for (Node* cur : nodes) {
            Node* last = nullptr;
            while (!st.empty() && st.back()->pri < cur->pri) {
                last = st.back();
                st.pop_back();
            }
            cur->l = last;
            if (!st.empty()) st.back()->r = cur;
            st.push_back(cur);
        }

        Node* root = st.front();

        vector<pair<Node*, bool>> dfs;
        dfs.reserve(v.size() * 2);
        dfs.push_back({root, false});
        while (!dfs.empty()) {
            auto [u, visited] = dfs.back();
            dfs.pop_back();
            if (!u) continue;
            if (visited) {
                pull(u);
            } else {
                dfs.push_back({u, true});
                dfs.push_back({u->r, false});
                dfs.push_back({u->l, false});
            }
        }
        return root;
    }

    template <class Pred>
    static int max_right_sub(Node* t, S& acc, Pred pred) {
        if (!t) return 0;
        push(t);

        int left_sz = sz(t->l);

        if (t->l) {
            S nxt = op(acc, prod_all(t->l));
            if (!pred(nxt)) {
                return max_right_sub(t->l, acc, pred);
            }
            acc = nxt;
        }

        S with_me = op(acc, lift(t->val));
        if (!pred(with_me)) {
            return left_sz;
        }
        acc = with_me;

        return left_sz + 1 + max_right_sub(t->r, acc, pred);
    }

    template <class Pred>
    static int min_left_sub(Node* t, S& acc, Pred pred) {
        if (!t) return 0;
        push(t);

        int left_sz = sz(t->l);

        if (t->r) {
            S nxt = op(prod_all(t->r), acc);
            if (!pred(nxt)) {
                return left_sz + 1 + min_left_sub(t->r, acc, pred);
            }
            acc = nxt;
        }

        S with_me = op(lift(t->val), acc);
        if (!pred(with_me)) {
            return left_sz + 1;
        }
        acc = with_me;

        return min_left_sub(t->l, acc, pred);
    }

private:
    Node* root_ = nullptr;

public:
    ImplicitTreap() = default;

    explicit ImplicitTreap(const vector<T>& v) : root_(build_from_vector_linear(v)) {}

    ~ImplicitTreap() {
        clear();
    }

    ImplicitTreap(const ImplicitTreap&) = delete;
    ImplicitTreap& operator=(const ImplicitTreap&) = delete;

    ImplicitTreap(ImplicitTreap&& other) noexcept : root_(other.root_) {
        other.root_ = nullptr;
    }

    ImplicitTreap& operator=(ImplicitTreap&& other) noexcept {
        if (this == &other) return *this;
        clear();
        root_ = other.root_;
        other.root_ = nullptr;
        return *this;
    }

    int size() const {
        return sz(root_);
    }

    bool empty() const {
        return root_ == nullptr;
    }

    void clear() {
        release_subtree(root_);
        root_ = nullptr;
    }

    void insert(int k, const T& x) {
        assert(0 <= k && k <= size());
        auto [a, b] = split_node(root_, k);
        root_ = merge_node(merge_node(a, new_node(x)), b);
    }

    void erase(int k) {
        assert(0 <= k && k < size());
        auto [a, bc] = split_node(root_, k);
        auto [b, c] = split_node(bc, 1);
        release_subtree(b);
        root_ = merge_node(a, c);
    }

    T get(int k) {
        assert(0 <= k && k < size());
        auto [a, bc] = split_node(root_, k);
        auto [b, c] = split_node(bc, 1);
        T ans = b->val;
        root_ = merge_node(a, merge_node(b, c));
        return ans;
    }

    void set(int k, const T& x) {
        assert(0 <= k && k < size());
        auto [a, bc] = split_node(root_, k);
        auto [b, c] = split_node(bc, 1);
        b->val = x;
        pull(b);
        root_ = merge_node(a, merge_node(b, c));
    }

    void apply(int l, int r, const F& f) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split_node(root_, l);
        auto [b, c] = split_node(bc, r - l);
        apply_lazy(b, f);
        root_ = merge_node(a, merge_node(b, c));
    }

    S prod(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split_node(root_, l);
        auto [b, c] = split_node(bc, r - l);
        S ans = prod_all(b);
        root_ = merge_node(a, merge_node(b, c));
        return ans;
    }

    S all_prod() const {
        return prod_all(root_);
    }

    void reverse(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        auto [a, bc] = split_node(root_, l);
        auto [b, c] = split_node(bc, r - l);
        apply_rev(b);
        root_ = merge_node(a, merge_node(b, c));
    }

    vector<T> dump() {
        vector<T> res;
        res.reserve(size());

        vector<Node*> st;
        Node* cur = root_;
        while (cur || !st.empty()) {
            while (cur) {
                push(cur);
                st.push_back(cur);
                cur = cur->l;
            }
            cur = st.back();
            st.pop_back();
            res.push_back(cur->val);
            cur = cur->r;
        }
        return res;
    }

    pair<ImplicitTreap, ImplicitTreap> split(int k) {
        assert(0 <= k && k <= size());
        auto [a, b] = split_node(root_, k);
        root_ = nullptr;

        ImplicitTreap left, right;
        left.root_ = a;
        right.root_ = b;
        return {std::move(left), std::move(right)};
    }

    void merge(ImplicitTreap& other) {
        if (this == &other) return;
        root_ = merge_node(root_, other.root_);
        other.root_ = nullptr;
    }

    void rotate(int l, int m, int r) {
        assert(0 <= l && l <= m && m <= r && r <= size());
        auto [a, bcd] = split_node(root_, l);
        auto [b, cd] = split_node(bcd, m - l);
        auto [c, d] = split_node(cd, r - m);
        root_ = merge_node(a, merge_node(c, merge_node(b, d)));
    }

    void swap_point(int k1, int k2) {
        assert(0 <= k1 && k1 < size());
        assert(0 <= k2 && k2 < size());
        if (k1 == k2) return;
        swap_ranges(k1, k1 + 1, k2, k2 + 1);
    }

    T front() {
        assert(!empty());
        return get(0);
    }

    T back() {
        assert(!empty());
        return get(size() - 1);
    }

    void push_back(const T& x) {
        insert(size(), x);
    }

    void push_front(const T& x) {
        insert(0, x);
    }

    void pop_back() {
        assert(!empty());
        erase(size() - 1);
    }

    void pop_front() {
        assert(!empty());
        erase(0);
    }

    void erase_range(int l, int r) {
        assert(0 <= l && l <= r && r <= size());
        if (l == r) return;
        auto [a, bc] = split_node(root_, l);
        auto [b, c] = split_node(bc, r - l);
        release_subtree(b);
        root_ = merge_node(a, c);
    }

    void swap_ranges(int l1, int r1, int l2, int r2) {
        assert(0 <= l1 && l1 <= r1 && r1 <= size());
        assert(0 <= l2 && l2 <= r2 && r2 <= size());

        if (l2 < l1) {
            std::swap(l1, l2);
            std::swap(r1, r2);
        }
        assert(r1 <= l2);

        auto [a, rest1] = split_node(root_, l1);
        auto [b, rest2] = split_node(rest1, r1 - l1);
        auto [c, rest3] = split_node(rest2, l2 - r1);
        auto [d, e2] = split_node(rest3, r2 - l2);

        root_ = merge_node(a, merge_node(d, merge_node(c, merge_node(b, e2))));
    }

    template <class Pred>
    int max_right(int l, Pred pred) {
        assert(0 <= l && l <= size());
        assert(pred(e()));

        auto [a, b] = split_node(root_, l);
        S acc = e();
        int add = max_right_sub(b, acc, pred);
        root_ = merge_node(a, b);
        return l + add;
    }

    template <class Pred>
    int min_left(int r, Pred pred) {
        assert(0 <= r && r <= size());
        assert(pred(e()));

        auto [a, b] = split_node(root_, r);
        S acc = e();
        int ans = min_left_sub(a, acc, pred);
        root_ = merge_node(a, b);
        return ans;
    }
};