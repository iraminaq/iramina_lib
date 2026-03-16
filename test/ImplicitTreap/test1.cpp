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
// テストケース 1
// =========================

void test_empty_construct() {
    Treap tr;
    expect_eq(tr.size(), 0, "empty construct size");
    expect_true(tr.empty(), "empty construct empty");
    expect_vec_eq(tr.dump(), {}, "empty construct dump");
    expect_eq(tr.all_prod(), 0LL, "empty construct all_prod");
    print_ok("empty_construct");
}

void test_build_from_vector() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        expect_eq(tr.size(), 4, "build size");
        expect_true(!tr.empty(), "build not empty");
        expect_vec_eq(tr.dump(), {1, 2, 3, 4}, "build dump");
        expect_eq(tr.all_prod(), 10LL, "build all_prod");
    }
    {
        Treap tr(vector<long long>{5});
        expect_eq(tr.size(), 1, "build single size");
        expect_vec_eq(tr.dump(), {5}, "build single dump");
        expect_eq(tr.all_prod(), 5LL, "build single all_prod");
        expect_eq(tr.get(0), 5LL, "build single get");
    }
    print_ok("build_from_vector");
}

void test_get_set() {
    {
        Treap tr(vector<long long>{10, 20, 30, 40});
        expect_eq(tr.get(0), 10LL, "get 0");
        expect_eq(tr.get(1), 20LL, "get 1");
        expect_eq(tr.get(3), 40LL, "get 3");
    }
    {
        Treap tr(vector<long long>{10, 20, 30, 40});
        tr.set(2, 99);
        expect_vec_eq(tr.dump(), {10, 20, 99, 40}, "set dump");
        expect_eq(tr.get(2), 99LL, "set get");
        expect_eq(tr.prod(0, 4), 169LL, "set prod all");
        expect_eq(tr.prod(1, 3), 119LL, "set prod mid");
    }
    {
        Treap tr(vector<long long>{7});
        tr.set(0, -3);
        expect_vec_eq(tr.dump(), {-3}, "set single dump");
        expect_eq(tr.get(0), -3LL, "set single get");
        expect_eq(tr.all_prod(), -3LL, "set single all_prod");
    }
    print_ok("get_set");
}

void test_insert() {
    {
        Treap tr(vector<long long>{2, 3, 4});
        tr.insert(0, 1);
        expect_vec_eq(tr.dump(), {1, 2, 3, 4}, "insert front");
        expect_eq(tr.size(), 4, "insert front size");
        expect_eq(tr.all_prod(), 10LL, "insert front sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        tr.insert(3, 4);
        expect_vec_eq(tr.dump(), {1, 2, 3, 4}, "insert back");
        expect_eq(tr.size(), 4, "insert back size");
        expect_eq(tr.all_prod(), 10LL, "insert back sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 4, 5});
        tr.insert(2, 3);
        expect_vec_eq(tr.dump(), {1, 2, 3, 4, 5}, "insert middle");
        expect_eq(tr.size(), 5, "insert middle size");
        expect_eq(tr.all_prod(), 15LL, "insert middle sum");
    }
    {
        Treap tr;
        tr.insert(0, 42);
        expect_vec_eq(tr.dump(), {42}, "insert into empty");
        expect_eq(tr.size(), 1, "insert into empty size");
        expect_eq(tr.all_prod(), 42LL, "insert into empty sum");
    }
    print_ok("insert");
}

void test_erase() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.erase(0);
        expect_vec_eq(tr.dump(), {2, 3, 4}, "erase front");
        expect_eq(tr.size(), 3, "erase front size");
        expect_eq(tr.all_prod(), 9LL, "erase front sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.erase(3);
        expect_vec_eq(tr.dump(), {1, 2, 3}, "erase back");
        expect_eq(tr.size(), 3, "erase back size");
        expect_eq(tr.all_prod(), 6LL, "erase back sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.erase(2);
        expect_vec_eq(tr.dump(), {1, 2, 4, 5}, "erase middle");
        expect_eq(tr.size(), 4, "erase middle size");
        expect_eq(tr.all_prod(), 12LL, "erase middle sum");
    }
    {
        Treap tr(vector<long long>{7});
        tr.erase(0);
        expect_vec_eq(tr.dump(), {}, "erase single dump");
        expect_eq(tr.size(), 0, "erase single size");
        expect_true(tr.empty(), "erase single empty");
        expect_eq(tr.all_prod(), 0LL, "erase single all_prod");
    }
    print_ok("erase");
}

void test_prod_all_prod() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        expect_eq(tr.prod(0, 5), 15LL, "prod all");
        expect_eq(tr.prod(0, 3), 6LL, "prod prefix");
        expect_eq(tr.prod(2, 5), 12LL, "prod suffix");
        expect_eq(tr.prod(2, 2), 0LL, "prod empty");
        expect_eq(tr.all_prod(), 15LL, "all_prod");
    }
    {
        Treap tr(vector<long long>{-2, 5, -1, 3});
        expect_eq(tr.prod(0, 4), 5LL, "prod mixed all");
        expect_eq(tr.prod(1, 3), 4LL, "prod mixed mid");
        expect_eq(tr.prod(0, 1), -2LL, "prod mixed one");
        expect_eq(tr.prod(3, 4), 3LL, "prod mixed tail");
    }
    print_ok("prod_all_prod");
}

void test_apply() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.apply(0, 4, 10);
        expect_vec_eq(tr.dump(), {11, 12, 13, 14}, "apply all dump");
        expect_eq(tr.all_prod(), 50LL, "apply all sum");
        expect_eq(tr.prod(1, 3), 25LL, "apply all mid");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.apply(1, 4, 10);
        expect_vec_eq(tr.dump(), {1, 12, 13, 14, 5}, "apply part dump");
        expect_eq(tr.prod(0, 5), 45LL, "apply part all");
        expect_eq(tr.prod(1, 4), 39LL, "apply part inside");
        expect_eq(tr.get(0), 1LL, "apply part get left");
        expect_eq(tr.get(4), 5LL, "apply part get right");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        tr.apply(1, 1, 100);
        expect_vec_eq(tr.dump(), {1, 2, 3}, "apply empty range");
        expect_eq(tr.all_prod(), 6LL, "apply empty range sum");
    }
    {
        Treap tr(vector<long long>{0, 0, 0, 0});
        tr.apply(0, 4, 1);
        tr.apply(1, 3, 2);
        tr.apply(2, 4, 3);
        expect_vec_eq(tr.dump(), {1, 3, 6, 4}, "apply multiple dump");
        expect_eq(tr.all_prod(), 14LL, "apply multiple sum");
        expect_eq(tr.prod(1, 3), 9LL, "apply multiple mid");
    }
    print_ok("apply");
}

void test_reverse() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.reverse(0, 5);
        expect_vec_eq(tr.dump(), {5, 4, 3, 2, 1}, "reverse all");
        expect_eq(tr.all_prod(), 15LL, "reverse all sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6});
        tr.reverse(1, 5);
        expect_vec_eq(tr.dump(), {1, 5, 4, 3, 2, 6}, "reverse part");
        expect_eq(tr.all_prod(), 21LL, "reverse part sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        tr.reverse(1, 1);
        tr.reverse(2, 3);
        expect_vec_eq(tr.dump(), {1, 2, 3}, "reverse len0/1");
        expect_eq(tr.all_prod(), 6LL, "reverse len0/1 sum");
    }
    print_ok("reverse");
}

void test_apply_reverse_mix() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.apply(1, 4, 10);   // {1,12,13,14,5}
        tr.reverse(0, 5);     // {5,14,13,12,1}
        expect_vec_eq(tr.dump(), {5, 14, 13, 12, 1}, "apply then reverse");
        expect_eq(tr.all_prod(), 45LL, "apply then reverse sum");
        expect_eq(tr.prod(1, 4), 39LL, "apply then reverse prod");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6});
        tr.reverse(1, 5);     // {1,5,4,3,2,6}
        tr.apply(2, 6, 1);    // {1,5,5,4,3,7}
        expect_vec_eq(tr.dump(), {1, 5, 5, 4, 3, 7}, "reverse then apply");
        expect_eq(tr.all_prod(), 25LL, "reverse then apply sum");
        expect_eq(tr.prod(2, 5), 12LL, "reverse then apply prod");
    }
    print_ok("apply_reverse_mix");
}

void test_dump() {
    {
        Treap tr(vector<long long>{3, 1, 4, 1, 5});
        auto v = tr.dump();
        expect_vec_eq(v, {3, 1, 4, 1, 5}, "dump plain");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4});
        tr.apply(1, 4, 5);    // {1,7,8,9}
        tr.reverse(0, 3);     // {8,7,1,9}
        tr.insert(2, 100);    // {8,7,100,1,9}
        tr.erase(4);          // {8,7,100,1}
        expect_vec_eq(tr.dump(), {8, 7, 100, 1}, "dump after mixed ops");
        expect_eq(tr.all_prod(), 116LL, "dump after mixed ops sum");
    }
    print_ok("dump");
}

void test_split() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        auto [a, b] = tr.split(2);
        expect_vec_eq(a.dump(), {1, 2}, "split mid left");
        expect_vec_eq(b.dump(), {3, 4, 5}, "split mid right");
        expect_eq(a.all_prod(), 3LL, "split mid left sum");
        expect_eq(b.all_prod(), 12LL, "split mid right sum");
        expect_true(tr.empty(), "split source empty");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        auto [a, b] = tr.split(0);
        expect_vec_eq(a.dump(), {}, "split zero left");
        expect_vec_eq(b.dump(), {1, 2, 3}, "split zero right");
        expect_true(tr.empty(), "split zero source empty");
    }
    {
        Treap tr(vector<long long>{1, 2, 3});
        auto [a, b] = tr.split(3);
        expect_vec_eq(a.dump(), {1, 2, 3}, "split n left");
        expect_vec_eq(b.dump(), {}, "split n right");
        expect_true(tr.empty(), "split n source empty");
    }
    print_ok("split");
}

void test_merge() {
    {
        Treap a(vector<long long>{1, 2});
        Treap b(vector<long long>{3, 4, 5});
        a.merge(b);
        expect_vec_eq(a.dump(), {1, 2, 3, 4, 5}, "merge basic");
        expect_eq(a.all_prod(), 15LL, "merge basic sum");
        expect_true(b.empty(), "merge basic other empty");
    }
    {
        Treap a;
        Treap b(vector<long long>{7, 8});
        a.merge(b);
        expect_vec_eq(a.dump(), {7, 8}, "merge empty with nonempty");
        expect_true(b.empty(), "merge empty with nonempty other empty");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5, 6});
        auto [a, b] = tr.split(4);
        a.merge(b);
        expect_vec_eq(a.dump(), {1, 2, 3, 4, 5, 6}, "split merge restore");
        expect_eq(a.all_prod(), 21LL, "split merge restore sum");
        expect_true(b.empty(), "split merge restore other empty");
    }
    print_ok("merge");
}

void test_split_merge_with_lazy() {
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.apply(1, 5, 10);   // {1,12,13,14,15}
        auto [a, b] = tr.split(3);
        expect_vec_eq(a.dump(), {1, 12, 13}, "split lazy left");
        expect_vec_eq(b.dump(), {14, 15}, "split lazy right");
        expect_eq(a.all_prod(), 26LL, "split lazy left sum");
        expect_eq(b.all_prod(), 29LL, "split lazy right sum");
    }
    {
        Treap tr(vector<long long>{1, 2, 3, 4, 5});
        tr.reverse(1, 5);     // {1,5,4,3,2}
        auto [a, b] = tr.split(2);
        a.merge(b);
        expect_vec_eq(a.dump(), {1, 5, 4, 3, 2}, "split merge after reverse");
        expect_eq(a.all_prod(), 15LL, "split merge after reverse sum");
    }
    print_ok("split_merge_with_lazy");
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    test_empty_construct();
    test_build_from_vector();
    test_get_set();
    test_insert();
    test_erase();
    test_prod_all_prod();
    test_apply();
    test_reverse();
    test_apply_reverse_mix();
    test_dump();
    test_split();
    test_merge();
    test_split_merge_with_lazy();

    cerr << "All test case set 1 passed.\n";
    cout << "OK\n";
    return 0;
}