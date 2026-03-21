#include <iostream>
#include <vector>
#include <cassert>
#include "SimpleImplicitTreap.hpp"

// ============================================
// 1. lazy なしの usage 例 (区間和)
// ============================================
struct TraitsNoLazy {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }
};
using TreapNoLazy = SimpleImplicitTreap<TraitsNoLazy>;

void test_no_lazy() {
    TreapNoLazy treap(std::vector<long long>{1, 2, 3, 4, 5});
    assert(treap.prod(1, 4) == 9); // 2+3+4 = 9
    treap.insert(2, 10);
    assert(treap.prod(1, 4) == 15); // 2+10+3 = 15
    treap.erase(2);
    assert(treap.prod(1, 4) == 9);
    std::cout << "test_no_lazy passed." << std::endl;
}

// ============================================
// 2. lazy ありの usage 例 (区間加算・区間和)
// ============================================
struct TraitsLazy {
    using T = long long;
    using F = long long;
    static constexpr bool use_lazy = true;

    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }

    static T apply(F f, T x, int len) { return x + f * len; }
    static F composition(F f, F g) { return f + g; }
    static F id() { return 0; }
};
using TreapLazy = SimpleImplicitTreap<TraitsLazy>;

void test_lazy() {
    TreapLazy treap(std::vector<long long>{1, 2, 3, 4, 5});
    treap.apply(1, 4, 10); // index 1, 2, 3 に +10
    // => {1, 12, 13, 14, 5}
    assert(treap.prod(1, 4) == 39);
    assert(treap.get(2) == 13);
    std::cout << "test_lazy passed." << std::endl;
}

// ============================================
// 3. reverse ありの usage 例 (区間反転付き・最大値)
// ============================================
struct TraitsReverse {
    using T = long long;
    static constexpr bool use_lazy = false; 
    static constexpr bool use_reverse = true;

    static T op(T a, T b) { return std::max(a, b); }
    static T e() { return -1e18; }
    static T reverse_prod(T x) { return x; } // 最大値なので反転しても結果は同じ
};
using TreapReverse = SimpleImplicitTreap<TraitsReverse>;

void test_reverse() {
    TreapReverse treap(std::vector<long long>{1, 5, 2, 4, 3});
    assert(treap.prod(1, 4) == 5); 
    treap.reverse(1, 4); // {5, 2, 4} -> {4, 2, 5} => 全体: {1, 4, 2, 5, 3}
    assert(treap.get(1) == 4);
    assert(treap.get(2) == 2);
    assert(treap.get(3) == 5);
    std::cout << "test_reverse passed." << std::endl;
}

int main() {
    test_no_lazy();
    test_lazy();
    test_reverse();
    std::cout << "All SimpleImplicitTreap tests passed!" << std::endl;
    return 0;
}
