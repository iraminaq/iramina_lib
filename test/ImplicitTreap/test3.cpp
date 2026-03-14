#include <bits/stdc++.h>
using namespace std;

// ImplicitTreap.hpp を include して使う想定
#include <iramina/ImplicitTreap/ImplicitTreap.hpp>

void test_default_construct() {
    ImplicitTreap<> tr;
    assert(tr.empty());
    assert(tr.size() == 0);
    assert(tr.all_prod() == 0LL);
}

void test_default_vector_construct() {
    vector<long long> a = {1, 2, 3, 4, 5};
    ImplicitTreap<> tr(a);

    assert(!tr.empty());
    assert(tr.size() == 5);
    assert(tr.all_prod() == 15LL);
    assert(tr.dump() == a);
}

void test_default_insert_get_set() {
    ImplicitTreap<> tr;

    tr.insert(0, 10);
    tr.insert(1, 20);
    tr.insert(1, 15);

    assert(tr.size() == 3);
    assert(tr.get(0) == 10LL);
    assert(tr.get(1) == 15LL);
    assert(tr.get(2) == 20LL);
    assert((tr.dump() == vector<long long>{10, 15, 20}));
    assert(tr.all_prod() == 45LL);

    tr.set(1, 100);
    assert(tr.get(1) == 100LL);
    assert((tr.dump() == vector<long long>{10, 100, 20}));
    assert(tr.all_prod() == 130LL);
}

void test_default_erase() {
    vector<long long> a = {1, 2, 3, 4, 5};
    ImplicitTreap<> tr(a);

    tr.erase(2);
    assert((tr.dump() == vector<long long>{1, 2, 4, 5}));
    assert(tr.size() == 4);
    assert(tr.all_prod() == 12LL);

    tr.erase(0);
    assert((tr.dump() == vector<long long>{2, 4, 5}));
    assert(tr.size() == 3);
    assert(tr.all_prod() == 11LL);

    tr.erase(2);
    assert((tr.dump() == vector<long long>{2, 4}));
    assert(tr.size() == 2);
    assert(tr.all_prod() == 6LL);
}

void test_default_prod() {
    vector<long long> a = {1, 2, 3, 4, 5};
    ImplicitTreap<> tr(a);

    assert(tr.prod(0, 0) == 0LL);
    assert(tr.prod(0, 5) == 15LL);
    assert(tr.prod(1, 4) == 9LL);
    assert(tr.prod(2, 3) == 3LL);
    assert(tr.prod(4, 5) == 5LL);
}

void test_default_apply_range_add() {
    vector<long long> a = {1, 2, 3, 4, 5};
    ImplicitTreap<> tr(a);

    tr.apply(1, 4, 10);  // [1,4): +10
    assert((tr.dump() == vector<long long>{1, 12, 13, 14, 5}));
    assert(tr.prod(0, 5) == 45LL);
    assert(tr.prod(1, 4) == 39LL);
    assert(tr.get(1) == 12LL);
    assert(tr.get(3) == 14LL);

    tr.apply(0, 5, -2);
    assert((tr.dump() == vector<long long>{-1, 10, 11, 12, 3}));
    assert(tr.all_prod() == 35LL);
}

void test_default_reverse_sum_invariant() {
    vector<long long> a = {1, 2, 3, 4, 5, 6};
    ImplicitTreap<> tr(a);

    long long before = tr.all_prod();
    tr.reverse(1, 5);
    assert((tr.dump() == vector<long long>{1, 5, 4, 3, 2, 6}));
    assert(tr.all_prod() == before);
    assert(tr.prod(1, 5) == 14LL);
}

void test_default_split_merge() {
    vector<long long> a = {1, 2, 3, 4, 5};
    ImplicitTreap<> tr(a);

    auto [left, right] = tr.split(2);
    assert(left.dump() == vector<long long>({1, 2}));
    assert(right.dump() == vector<long long>({3, 4, 5}));
    assert(left.all_prod() == 3LL);
    assert(right.all_prod() == 12LL);

    left.merge(right);
    assert(left.dump() == a);
    assert(left.all_prod() == 15LL);
    assert(right.empty());
}

void test_default_rotate() {
    vector<long long> a = {1, 2, 3, 4, 5, 6};
    ImplicitTreap<> tr(a);

    // [l,m)=[1,3), [m,r)=[3,5) を入れ替え
    // 1 | 2 3 | 4 5 | 6  -> 1 | 4 5 | 2 3 | 6
    tr.rotate(1, 3, 5);
    assert((tr.dump() == vector<long long>{1, 4, 5, 2, 3, 6}));
    assert(tr.all_prod() == 21LL);
}

void test_default_swap_ranges() {
    vector<long long> a = {1, 2, 3, 4, 5, 6, 7, 8};
    ImplicitTreap<> tr(a);

    // [1,3)={2,3} と [5,8)={6,7,8} を swap
    tr.swap_ranges(1, 3, 5, 8);
    assert((tr.dump() == vector<long long>{1, 6, 7, 8, 4, 5, 2, 3}));
    assert(tr.all_prod() == 36LL);
}

void test_default_max_right() {
    vector<long long> a = {2, 3, 5, 7, 11};
    ImplicitTreap<> tr(a);

    // sum([0,r)) <= 10 となる最大 r
    int r1 = tr.max_right(0, [](long long x) { return x <= 10; });
    assert(r1 == 3);  // 2+3+5=10, 次を含めると17

    // sum([1,r)) <= 8
    int r2 = tr.max_right(1, [](long long x) { return x <= 8; });
    assert(r2 == 3);  // 3+5=8
}

void test_default_min_left() {
    vector<long long> a = {2, 3, 5, 7, 11};
    ImplicitTreap<> tr(a);

    // sum([l,5)) <= 18 となる最小 l
    int l1 = tr.min_left(5, [](long long x) { return x <= 18; });
    assert(l1 == 3);  // 7+11=18

    // sum([l,4)) <= 15 となる最小 l
    int l2 = tr.min_left(4, [](long long x) { return x <= 15; });
    assert(l2 == 1);  // 3+5+7=15
}

void test_default_type_inference() {
    static_assert(std::is_same_v<decltype(std::declval<ImplicitTreap<>>().get(0)), long long>);
    static_assert(std::is_same_v<decltype(std::declval<ImplicitTreap<>>().prod(0, 0)), long long>);
    static_assert(std::is_same_v<decltype(std::declval<ImplicitTreap<>>().all_prod()), long long>);
}

int main() {
    test_default_construct();
    test_default_vector_construct();
    test_default_insert_get_set();
    test_default_erase();
    test_default_prod();
    test_default_apply_range_add();
    test_default_reverse_sum_invariant();
    test_default_split_merge();
    test_default_rotate();
    test_default_swap_ranges();
    test_default_max_right();
    test_default_min_left();
    test_default_type_inference();

    cout << "All default-template tests passed.\n";
    return 0;
}