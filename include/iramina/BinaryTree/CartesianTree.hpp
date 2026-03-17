#include <bits/stdc++.h>
using namespace std;

template <class T, class Comp = std::less<T>>
struct CartesianTree {
private:
    int n_;
    int root_;
    vector<T> a_;

    vector<int> parent_;
    vector<int> left_;
    vector<int> right_;

    vector<int> sub_l_;
    vector<int> sub_r_;
    vector<int> sub_size_;

    Comp comp_;

    bool has_lca_ = false;

    // Euler Tour + RMQ for LCA
    vector<int> first_;          // first occurrence in euler_
    vector<int> node_depth_;     // depth of each node
    vector<int> euler_;          // node ids
    vector<int> euler_depth_;    // depth along euler tour
    vector<int> lg_;             // log table
    vector<vector<int>> st_;     // sparse table: stores index in euler_ with minimum depth

private:
    void build_tree() {
        parent_.assign(n_, -1);
        left_.assign(n_, -1);
        right_.assign(n_, -1);

        vector<int> st;
        st.reserve(n_);

        for (int i = 0; i < n_; i++) {
            int last = -1;
            while (!st.empty() && comp_(a_[i], a_[st.back()])) {
                last = st.back();
                st.pop_back();
            }
            if (!st.empty()) {
                right_[st.back()] = i;
                parent_[i] = st.back();
            }
            if (last != -1) {
                left_[i] = last;
                parent_[last] = i;
            }
            st.push_back(i);
        }

        root_ = -1;
        for (int i = 0; i < n_; i++) {
            if (parent_[i] == -1) {
                root_ = i;
                break;
            }
        }
    }

    void dfs_subtree(int start_root) {
        vector<int> order;
        order.reserve(n_);
        vector<int> st;
        if (start_root != -1) st.push_back(start_root);
        while(!st.empty()) {
            int v = st.back();
            st.pop_back();
            order.push_back(v);
            if (left_[v] != -1) st.push_back(left_[v]);
            if (right_[v] != -1) st.push_back(right_[v]);
        }
        
        for(int i = n_ - 1; i >= 0; i--) {
            int v = order[i];
            sub_l_[v] = v;
            sub_r_[v] = v + 1;
            sub_size_[v] = 1;
            if (left_[v] != -1) {
                sub_l_[v] = sub_l_[left_[v]];
                sub_size_[v] += sub_size_[left_[v]];
            }
            if (right_[v] != -1) {
                sub_r_[v] = sub_r_[right_[v]];
                sub_size_[v] += sub_size_[right_[v]];
            }
        }
    }

    void dfs_euler(int root) {
        if (root == -1) return;
        vector<pair<int, int>> st;
        st.reserve(n_);
        st.push_back({root, 0});
        int d = 0;
        
        while(!st.empty()) {
            auto& [v, state] = st.back();
            if (state == 0) {
                node_depth_[v] = d;
                first_[v] = (int)euler_.size();
                euler_.push_back(v);
                euler_depth_.push_back(d);
                state = 1;
                if (left_[v] != -1) {
                    d++;
                    st.push_back({left_[v], 0});
                }
            } else if (state == 1) {
                if (left_[v] != -1) {
                    euler_.push_back(v);
                    euler_depth_.push_back(d);
                }
                state = 2;
                if (right_[v] != -1) {
                    d++;
                    st.push_back({right_[v], 0});
                }
            } else {
                if (right_[v] != -1) {
                    euler_.push_back(v);
                    euler_depth_.push_back(d);
                }
                st.pop_back();
                d--;
            }
        }
    }

    void build_sparse_table() {
        int m = (int)euler_.size();
        lg_.assign(m + 1, 0);
        for (int i = 2; i <= m; i++) lg_[i] = lg_[i >> 1] + 1;

        int K = lg_[m] + 1;
        st_.assign(K, vector<int>(m));

        for (int i = 0; i < m; i++) st_[0][i] = i;

        for (int k = 1; k < K; k++) {
            int len = 1 << k;
            int half = len >> 1;
            for (int i = 0; i + len <= m; i++) {
                int x = st_[k - 1][i];
                int y = st_[k - 1][i + half];
                st_[k][i] = (euler_depth_[x] <= euler_depth_[y] ? x : y);
            }
        }
    }

    int lca_index_(int u, int v) const {
        assert(has_lca_);
        int l = first_[u];
        int r = first_[v];
        if (l > r) swap(l, r);
        int len = r - l + 1;
        int k = lg_[len];
        int x = st_[k][l];
        int y = st_[k][r - (1 << k) + 1];
        return (euler_depth_[x] <= euler_depth_[y] ? euler_[x] : euler_[y]);
    }

public:
    CartesianTree() : n_(0), root_(-1) {}

    explicit CartesianTree(const vector<T>& a, Comp comp = Comp())
        : n_((int)a.size()), root_(-1), a_(a), comp_(comp) {
        if (n_ == 0) return;
        build_tree();

        sub_l_.assign(n_, -1);
        sub_r_.assign(n_, -1);
        sub_size_.assign(n_, 0);
        dfs_subtree(root_);
    }

    int size() const {
        return n_;
    }

    int root() const {
        return root_;
    }

    int parent(int v) const {
        return parent_[v];
    }

    int left_child(int v) const {
        return left_[v];
    }

    int right_child(int v) const {
        return right_[v];
    }

    pair<int, int> subtree_range(int v) const {
        return {sub_l_[v], sub_r_[v]};
    }

    int subtree_size(int v) const {
        return sub_size_[v];
    }

    const T& value(int v) const {
        return a_[v];
    }

    void build_lca() {
        if (has_lca_ || n_ == 0) return;

        first_.assign(n_, -1);
        node_depth_.assign(n_, 0);
        euler_.clear();
        euler_depth_.clear();
        euler_.reserve(2 * n_ - 1);
        euler_depth_.reserve(2 * n_ - 1);

        dfs_euler(root_);
        build_sparse_table();
        has_lca_ = true;
    }

    // returns index of extremum on [l, r)
    int query_index(int l, int r) const {
        assert(0 <= l && l < r && r <= n_);
        assert(has_lca_);
        if (l + 1 == r) return l;
        return lca_index_(l, r - 1);
    }

    bool has_lca() const {
        return has_lca_;
    }

    //追加分
        int lca(int u, int v) const {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        assert(has_lca_);
        return lca_index_(u, v);
    }

    int depth(int v) const {
        assert(0 <= v && v < n_);
        assert(has_lca_);
        return node_depth_[v];
    }

    pair<int, int> children(int v) const {
        assert(0 <= v && v < n_);
        return {left_[v], right_[v]};
    }

    bool is_ancestor(int u, int v) const {
        assert(0 <= u && u < n_);
        assert(0 <= v && v < n_);
        assert(has_lca_);
        return lca_index_(u, v) == u;
    }

    const T& query(int l, int r) const {
        return a_[query_index(l, r)];
    }
};