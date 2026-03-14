// random_test_implicit_treap.cpp
// 前提:
// - ImplicitTreap 本体が見えていること
// - range add / range sum 用に具体化して使う
//
// 使い方:
//   1. このファイルと ImplicitTreap を一緒にコンパイル
//   2. 実行
//   3. mismatch があればその場で内容を出して終了
//
// 備考:
// - swap_ranges は「非重複区間のみ」だけ生成
// - max_right / min_left は sum <= K の単調 predicate で検証
// - split / merge もランダムに検証
//
// AtCoder 相当 C++17/20 を想定

#include <bits/stdc++.h>
using namespace std;

// =========================
// range add / range sum 用具体化
// =========================

using T = long long;
using S = long long;
using F = long long;

S op(const S& a, const S& b) { return a + b; }
S e() { return 0LL; }
S lift(const T& x) { return x; }
T mapping_point(const F& f, const T& x) { return x + f; }
S mapping(const F& f, const S& x, int len) { return x + f * len; }
F composition(const F& f, const F& g) { return f + g; } // 先に g, 後で f
F id() { return 0LL; }
S reverse_prod(const S& x) { return x; }

// =========================
// ここで ImplicitTreap を include
// =========================

#include <iramina/ImplicitTreap/ImplicitTreap.hpp>

using Treap = ImplicitTreap<
    T, S, F,
    op, e, lift, mapping_point, mapping, composition, id, reverse_prod
>;

// =========================
// utility
// =========================

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

long long rand_ll(long long l, long long r) {
    uniform_int_distribution<long long> dist(l, r);
    return dist(rng);
}

int rand_int(int l, int r) {
    uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}

template <class X>
void dump_vec_err(const vector<X>& v, const string& name) {
    cerr << name << " = {";
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i) cerr << ", ";
        cerr << v[i];
    }
    cerr << "}\n";
}

template <class X, class Y>
void die_mismatch(const string& msg, const X& a, const Y& b) {
    cerr << "Mismatch: " << msg << "\n";
    cerr << "actual   = " << a << "\n";
    cerr << "expected = " << b << "\n";
    exit(1);
}

void die_msg(const string& msg) {
    cerr << "Mismatch: " << msg << "\n";
    exit(1);
}

// =========================
// naive model helpers
// =========================

long long naive_prod(const vector<long long>& v, int l, int r) {
    long long s = 0;
    for (int i = l; i < r; ++i) s += v[i];
    return s;
}

void naive_apply(vector<long long>& v, int l, int r, long long f) {
    for (int i = l; i < r; ++i) v[i] += f;
}

void naive_reverse(vector<long long>& v, int l, int r) {
    reverse(v.begin() + l, v.begin() + r);
}

void naive_rotate(vector<long long>& v, int l, int m, int r) {
    // [l,m) + [m,r) -> [m,r) + [l,m)
    vector<long long> tmp;
    tmp.reserve(r - l);
    for (int i = m; i < r; ++i) tmp.push_back(v[i]);
    for (int i = l; i < m; ++i) tmp.push_back(v[i]);
    for (int i = 0; i < r - l; ++i) v[l + i] = tmp[i];
}

void naive_swap_ranges(vector<long long>& v, int l1, int r1, int l2, int r2) {
    if (l2 < l1) {
        swap(l1, l2);
        swap(r1, r2);
    }
    // 非重複前提
    vector<long long> a(v.begin(), v.begin() + l1);
    vector<long long> b(v.begin() + l1, v.begin() + r1);
    vector<long long> c(v.begin() + r1, v.begin() + l2);
    vector<long long> d(v.begin() + l2, v.begin() + r2);
    vector<long long> e(v.begin() + r2, v.end());

    vector<long long> res;
    res.reserve(v.size());
    res.insert(res.end(), a.begin(), a.end());
    res.insert(res.end(), d.begin(), d.end());
    res.insert(res.end(), c.begin(), c.end());
    res.insert(res.end(), b.begin(), b.end());
    res.insert(res.end(), e.begin(), e.end());
    v.swap(res);
}

int naive_max_right(const vector<long long>& v, int l, long long K) {
    long long s = 0;
    int r = l;
    while (r < (int)v.size() && s + v[r] <= K) {
        s += v[r];
        ++r;
    }
    return r;
}

int naive_min_left(const vector<long long>& v, int r, long long K) {
    long long s = 0;
    int l = r;
    while (l > 0 && v[l - 1] + s <= K) {
        --l;
        s += v[l];
    }
    return l;
}

void check_equal(const Treap& /*unused*/, Treap& tr, const vector<long long>& model, const string& where) {
    vector<long long> got = tr.dump();
    if (got != model) {
        cerr << "Mismatch at " << where << "\n";
        dump_vec_err(got, "treap");
        dump_vec_err(model, "model");
        exit(1);
    }
    long long got_sum = tr.all_prod();
    long long expected_sum = naive_prod(model, 0, (int)model.size());
    if (got_sum != expected_sum) {
        cerr << "Mismatch at " << where << " (all_prod)\n";
        cerr << "treap    = " << got_sum << "\n";
        cerr << "expected = " << expected_sum << "\n";
        dump_vec_err(got, "treap_dump");
        dump_vec_err(model, "model");
        exit(1);
    }
    if (tr.size() != (int)model.size()) {
        cerr << "Mismatch at " << where << " (size)\n";
        cerr << "treap    = " << tr.size() << "\n";
        cerr << "expected = " << model.size() << "\n";
        exit(1);
    }
    if (tr.empty() != model.empty()) {
        cerr << "Mismatch at " << where << " (empty)\n";
        exit(1);
    }
}

// =========================
// split/merge helper check
// =========================

void random_check_split_merge(Treap& tr, vector<long long>& model) {
    int n = (int)model.size();
    int k = rand_int(0, n);

    Treap work(std::move(tr));
    auto [a, b] = work.split(k);

    vector<long long> left_model(model.begin(), model.begin() + k);
    vector<long long> right_model(model.begin() + k, model.end());

    if (a.dump() != left_model) {
        cerr << "split left mismatch, k=" << k << "\n";
        dump_vec_err(a.dump(), "a");
        dump_vec_err(left_model, "left_model");
        exit(1);
    }
    if (b.dump() != right_model) {
        cerr << "split right mismatch, k=" << k << "\n";
        dump_vec_err(b.dump(), "b");
        dump_vec_err(right_model, "right_model");
        exit(1);
    }
    if (!work.empty()) {
        die_msg("split source should become empty");
    }

    a.merge(b);
    tr = std::move(a);

    check_equal(tr, tr, model, "after split+merge restore");
}

// =========================
// main random test
// =========================

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int OUTER = 200;
    constexpr int OPS_PER_CASE = 2000;
    constexpr int INIT_N_MAX = 30;
    constexpr long long VAL_L = 0;
    constexpr long long VAL_R = 20;
    constexpr long long ADD_L = 0;
    constexpr long long ADD_R = 10;

    for (int tc = 1; tc <= OUTER; ++tc) {
        int init_n = rand_int(0, INIT_N_MAX);
        vector<long long> model(init_n);
        for (int i = 0; i < init_n; ++i) model[i] = rand_ll(VAL_L, VAL_R);

        Treap tr(model);
        check_equal(tr, tr, model, "initial");

        for (int step = 1; step <= OPS_PER_CASE; ++step) {
            int n = (int)model.size();

            // たまに split/merge を単独で検証
            if (rand_int(0, 19) == 0) {
                random_check_split_merge(tr, model);
                continue;
            }

            int op_id = rand_int(0, 11);

            switch (op_id) {
                case 0: { // insert
                    int k = rand_int(0, n);
                    long long x = rand_ll(VAL_L, VAL_R);
                    tr.insert(k, x);
                    model.insert(model.begin() + k, x);
                    check_equal(tr, tr, model, "insert");
                    break;
                }
                case 1: { // erase
                    if (n == 0) break;
                    int k = rand_int(0, n - 1);
                    tr.erase(k);
                    model.erase(model.begin() + k);
                    check_equal(tr, tr, model, "erase");
                    break;
                }
                case 2: { // get
                    if (n == 0) break;
                    int k = rand_int(0, n - 1);
                    long long a = tr.get(k);
                    long long b = model[k];
                    if (a != b) {
                        cerr << "get mismatch: k=" << k << "\n";
                        die_mismatch("get", a, b);
                    }
                    break;
                }
                case 3: { // set
                    if (n == 0) break;
                    int k = rand_int(0, n - 1);
                    long long x = rand_ll(VAL_L, VAL_R);
                    tr.set(k, x);
                    model[k] = x;
                    check_equal(tr, tr, model, "set");
                    break;
                }
                case 4: { // apply
                    int l = rand_int(0, n);
                    int r = rand_int(l, n);
                    long long f = rand_ll(ADD_L, ADD_R);
                    tr.apply(l, r, f);
                    naive_apply(model, l, r, f);
                    check_equal(tr, tr, model, "apply");
                    break;
                }
                case 5: { // prod
                    int l = rand_int(0, n);
                    int r = rand_int(l, n);
                    long long a = tr.prod(l, r);
                    long long b = naive_prod(model, l, r);
                    if (a != b) {
                        cerr << "prod mismatch: [" << l << "," << r << ")\n";
                        die_mismatch("prod", a, b);
                    }
                    break;
                }
                case 6: { // reverse
                    int l = rand_int(0, n);
                    int r = rand_int(l, n);
                    tr.reverse(l, r);
                    naive_reverse(model, l, r);
                    check_equal(tr, tr, model, "reverse");
                    break;
                }
                case 7: { // rotate
                    int l = rand_int(0, n);
                    int m = rand_int(l, n);
                    int r = rand_int(m, n);
                    tr.rotate(l, m, r);
                    naive_rotate(model, l, m, r);
                    check_equal(tr, tr, model, "rotate");
                    break;
                }
                case 8: { // swap_ranges (非重複)
                    if (n == 0) break;
                    int l1 = rand_int(0, n);
                    int r1 = rand_int(l1, n);
                    int l2 = rand_int(0, n);
                    int r2 = rand_int(l2, n);

                    if (l2 < l1) {
                        swap(l1, l2);
                        swap(r1, r2);
                    }
                    if (!(r1 <= l2)) break; // 非重複のみ

                    tr.swap_ranges(l1, r1, l2, r2);
                    naive_swap_ranges(model, l1, r1, l2, r2);
                    check_equal(tr, tr, model, "swap_ranges");
                    break;
                }
                case 9: { // max_right
                    int l = rand_int(0, n);
                    long long K = rand_ll(0, 100);
                    int a = tr.max_right(l, [&](long long s) { return s <= K; });
                    int b = naive_max_right(model, l, K);
                    if (a != b) {
                        cerr << "max_right mismatch: l=" << l << ", K=" << K << "\n";
                        die_mismatch("max_right", a, b);
                    }
                    break;
                }
                case 10: { // min_left
                    int r = rand_int(0, n);
                    long long K = rand_ll(0, 100);
                    int a = tr.min_left(r, [&](long long s) { return s <= K; });
                    int b = naive_min_left(model, r, K);
                    if (a != b) {
                        cerr << "min_left mismatch: r=" << r << ", K=" << K << "\n";
                        die_mismatch("min_left", a, b);
                    }
                    break;
                }
                case 11: { // dump / all_prod の軽確認
                    check_equal(tr, tr, model, "dump/all_prod");
                    break;
                }
            }
        }

        if (tc % 20 == 0) {
            cerr << "passed cases: " << tc << "/" << OUTER << "\n";
        }
    }

    cerr << "All random tests passed.\n";
    cout << "OK\n";
    return 0;
}