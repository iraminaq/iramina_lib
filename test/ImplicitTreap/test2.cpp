#include <bits/stdc++.h>
using namespace std;

// =========================
// ImplicitTreap 用具体化
// range add / range sum
// =========================

using T = long long;
using S = long long;
using F = long long;

S op(const S& a, const S& b) {
    return a + b;
}

S e() {
    return 0LL;
}

S lift(const T& x) {
    return x;
}

T mapping_point(const F& f, const T& x) {
    return x + f;
}

S mapping(const F& f, const S& x, int len) {
    return x + f * len;
}

F composition(const F& f, const F& g) {
    // 先に g、その後 f
    return f + g;
}

F id() {
    return 0LL;
}

S reverse_prod(const S& x) {
    return x;
}

// =========================
// ここで ImplicitTreap を include
// =========================

#include <iramina/BinaryTree/ImplicitTreap.hpp>

using Treap = ImplicitTreap<T, S, F,
                            op, e, lift, mapping_point, mapping,
                            composition, id, reverse_prod>;

// =========================
// テスト補助
// =========================

template <class X, class Y>
void expect_eq(const X& actual, const Y& expected, const string& msg = "") {
    if (!(actual == expected)) {
        cerr << "Test failed";
        if (!msg.empty()) cerr << ": " << msg;
        cerr << "\n";
        cerr << "actual   = " << actual << "\n";
        cerr << "expected = " << expected << "\n";
        exit(1);
    }
}

template <class X>
void expect_true(const X& cond, const string& msg = "") {
    if (!cond) {
        cerr << "Test failed";
        if (!msg.empty()) cerr << ": " << msg;
        cerr << "\n";
        exit(1);
    }
}

void expect_vec_eq(const vector<long long>& actual, const vector<long long>& expected, const string& msg = "") {
    if (actual != expected) {
        cerr << "Test failed";
        if (!msg.empty()) cerr << ": " << msg;
        cerr << "\n";
        cerr << "actual   = {";
        for (int i = 0; i < (int)actual.size(); ++i) {
            if (i) cerr << ", ";
            cerr << actual[i];
        }
        cerr << "}\n";
        cerr << "expected = {";
        for (int i = 0; i < (int)expected.size(); ++i) {
            if (i) cerr << ", ";
            cerr << expected[i];
        }
        cerr << "}\n";
        exit(1);
    }
}

void print_ok(const string& name) {
    cerr << "[OK] " << name << "\n";
}

// =========================
// テストケース 2
// rotate / swap_ranges / max_right / min_left
// =========================

void test_rotate() {
    {
        Treap tr(vector<long long>{0, 1, 2, 3, 4, 5});
        tr.rotate(1, 3, 5);
        expect_vec_eq(tr.dump(), {0, 3, 4, 1, 2, 5}, "R-1 rotate basic");
        expect_eq(tr.all_prod(), 15LL, "R-1 rotate basic sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.rotate(0, 2, 5);
        expect_vec_eq(tr.dump(), {3, 4, 5, 1, 2}, "R-2 rotate whole interval");
        expect_eq(tr.all_prod(), 15LL, "R-2 rotate whole interval sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.rotate(1, 1, 3);
        expect_vec_eq(tr.dump(), {1, 2, 3, 4}, "R-3 rotate left length zero");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.rotate(1, 3, 3);
        expect_vec_eq(tr.dump(), {1, 2, 3, 4}, "R-4 rotate right length zero");
    }
    {
        Treap tr(vector<long long>{10, 20, 30, 40});
        tr.rotate(1, 2, 3);
        expect_vec_eq(tr.dump(), {10, 30, 20, 40}, "R-5 rotate length one");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.apply(1, 5, 10);   // {1,12,13,14,15}
        tr.rotate(1, 3, 5);   // {1,14,15,12,13}
        expect_vec_eq(tr.dump(), {1, 14, 15, 12, 13}, "R-6 apply then rotate");
        expect_eq(tr.all_prod(), 55LL, "R-6 apply then rotate sum");
        expect_eq(tr.prod(1, 4), 41LL, "R-6 apply then rotate prod");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6});
        tr.reverse(1, 6);     // {1,6,5,4,3,2}
        tr.rotate(1, 3, 6);   // {1,4,3,2,6,5}
        expect_vec_eq(tr.dump(), {1, 4, 3, 2, 6, 5}, "R-7 reverse then rotate");
        expect_eq(tr.all_prod(), 21LL, "R-7 reverse then rotate sum");
    }
    print_ok("rotate");
}

void test_swap_ranges() {
    {
        Treap tr(vector<long long>{0, 1, 2, 3, 4, 5, 6});
        tr.swap_ranges(1, 3, 5, 7);
        expect_vec_eq(tr.dump(), {0, 5, 6, 3, 4, 1, 2}, "S-1 swap basic");
        expect_eq(tr.all_prod(), 21LL, "S-1 swap basic sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6});
        tr.swap_ranges(0, 2, 4, 6);
        expect_vec_eq(tr.dump(), {5, 6, 3, 4, 1, 2}, "S-2 swap same length");
        expect_eq(tr.all_prod(), 21LL, "S-2 swap same length sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6, 7, 8});
        tr.swap_ranges(1, 2, 4, 7);
        expect_vec_eq(tr.dump(), {1, 5, 6, 7, 3, 4, 2, 8}, "S-3 swap different length");
        expect_eq(tr.all_prod(), 36LL, "S-3 swap different length sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.swap_ranges(1, 1, 3, 4);
        expect_vec_eq(tr.dump(), {1, 4, 2, 3}, "S-4 swap with empty interval");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6});
        tr.apply(1, 5, 10);           // {1,12,13,14,15,6}
        tr.swap_ranges(1, 3, 4, 6);   // {12,13} <-> {15,6}
        expect_vec_eq(tr.dump(), {1, 15, 6, 14, 12, 13}, "S-5 apply then swap");
        expect_eq(tr.all_prod(), 61LL, "S-5 apply then swap sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6, 7});
        tr.reverse(1, 6);             // {1,6,5,4,3,2,7}
        tr.swap_ranges(1, 3, 5, 7);   // {6,5} <-> {2,7}
        expect_vec_eq(tr.dump(), {1, 2, 7, 4, 3, 6, 5}, "S-6 reverse then swap");
        expect_eq(tr.all_prod(), 28LL, "S-6 reverse then swap sum");
    }
    print_ok("swap_ranges");
}

void test_max_right() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        int r = tr.max_right(0, [&](long long s) { return s <= 6; });
        expect_eq(r, 3, "MR-1 basic");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        int r = tr.max_right(2, [&](long long s) { return s <= 7; });
        expect_eq(r, 4, "MR-2 from middle");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        int r = tr.max_right(0, [&](long long s) { return s <= 100; });
        expect_eq(r, 3, "MR-3 all ok");
    }
    {
        Treap tr(vector<long long>{5, 6, 7});
        int r = tr.max_right(0, [&](long long s) { return s <= 4; });
        expect_eq(r, 0, "MR-4 first element fails");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        int r = tr.max_right(3, [&](long long s) { return s <= 0; });
        expect_eq(r, 3, "MR-5 l equals n");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.apply(1, 4, 10);   // {1,12,13,14}
        int r = tr.max_right(1, [&](long long s) { return s <= 25; });
        expect_eq(r, 3, "MR-6 after apply");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.reverse(0, 5);   // {5,4,3,2,1}
        int r = tr.max_right(1, [&](long long s) { return s <= 7; });
        expect_eq(r, 3, "MR-7 after reverse");
    }
    print_ok("max_right");
}

void test_min_left() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        int l = tr.min_left(5, [&](long long s) { return s <= 9; });
        expect_eq(l, 3, "ML-1 basic");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        int l = tr.min_left(4, [&](long long s) { return s <= 7; });
        expect_eq(l, 2, "ML-2 from middle");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        int l = tr.min_left(3, [&](long long s) { return s <= 100; });
        expect_eq(l, 0, "ML-3 all ok");
    }
    {
        Treap tr(vector<long long>{5, 6, 7});
        int l = tr.min_left(3, [&](long long s) { return s <= 6; });
        expect_eq(l, 3, "ML-4 first from right fails");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        int l = tr.min_left(0, [&](long long s) { return s <= 0; });
        expect_eq(l, 0, "ML-5 r equals 0");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.apply(0, 3, 10);   // {11,12,13,4}
        int l = tr.min_left(3, [&](long long s) { return s <= 25; });
        expect_eq(l, 1, "ML-6 after apply");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.reverse(0, 5);   // {5,4,3,2,1}
        int l = tr.min_left(4, [&](long long s) { return s <= 5; });
        expect_eq(l, 2, "ML-7 after reverse");
    }
    print_ok("min_left");
}

void test_bounds_consistency() {
    {
        Treap tr(vector<long long>{2, 2, 2, 2, 2});
        int r = tr.max_right(1, [&](long long s) { return s <= 4; });
        int l = tr.min_left(4, [&](long long s) { return s <= 4; });
        expect_eq(r, 3, "B-1 max_right consistency");
        expect_eq(l, 2, "B-1 min_left consistency");
    }
    {
        Treap tr(vector<long long>{10, 20, 30});
        int r = tr.max_right(1, [&](long long s) { return s == 0; });
        int l = tr.min_left(2, [&](long long s) { return s == 0; });
        expect_eq(r, 1, "B-2 max_right empty only");
        expect_eq(l, 2, "B-2 min_left empty only");
    }
    print_ok("bounds_consistency");
}

void test_complex_case() {
    Treap tr(vector<long long>{1, 2, 3, 4, 5, 6, 7, 8});
    tr.apply(2, 7, 10);            // {1,2,13,14,15,16,17,8}
    tr.rotate(1, 4, 7);            // {1,15,16,17,2,13,14,8}
    tr.swap_ranges(1, 3, 5, 8);    // {1,13,14,8,17,2,15,16}

    expect_vec_eq(tr.dump(), {1, 13, 14, 8, 17, 2, 15, 16}, "C-1 final dump");
    expect_eq(tr.all_prod(), 86LL, "C-1 final sum");
    expect_eq(tr.prod(1, 4), 35LL, "C-1 prod 1,4");
    expect_eq(tr.prod(4, 8), 50LL, "C-1 prod 4,8");

    int r = tr.max_right(1, [&](long long s) { return s <= 35; });
    expect_eq(r, 4, "C-1 max_right");

    int l = tr.min_left(8, [&](long long s) { return s <= 48; });
    expect_eq(l, 5, "C-1 min_left");

    print_ok("complex_case");
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    test_rotate();
    test_swap_ranges();
    test_max_right();
    test_min_left();
    test_bounds_consistency();
    test_complex_case();

    cerr << "All test case set 2 passed.\n";
    cout << "OK\n";
    return 0;
}