#undef NDEBUG
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "../../include/iramina/BinaryTree/WeightedMergeSortTree.hpp"

using namespace std;

using Key = long long;
using Weight = long long;
using MST = WeightedMergeSortTree<Key, Weight>;
using Elem = MST::Elem;

// 1. 空配列
void test_1() {
    vector<Elem> init = {};
    MST mst(init);
    assert(mst.size() == 0);
    assert(mst.count(0, 0, 0, 10) == 0);
    assert(mst.sum(0, 0, 0, 10) == 0);
}

// 2. 1 要素
void test_2() {
    vector<Elem> init = {{5, 10}};
    MST mst(init);
    assert(mst.size() == 1);
    assert(mst.count(0, 1, 5, 6) == 1);
    assert(mst.sum(0, 1, 5, 6) == 10);
    assert(mst.count(0, 1, 0, 5) == 0);
    assert(mst.sum(0, 1, 0, 5) == 0);
    assert(mst.count(0, 1, 6, 10) == 0);
    assert(mst.sum(0, 1, 6, 10) == 0);
}

// 3. 異なる key を持つ基本例
void test_3() {
    vector<Elem> init = { {5,10}, {2,7}, {8,1}, {4,6} };
    MST mst(init);
    assert(mst.count(0, 4, 0, 10) == 4);
    assert(mst.sum(0, 4, 0, 10) == 24);
    assert(mst.count(0, 4, 0, 5) == 2);
    assert(mst.sum(0, 4, 0, 5) == 13);
    assert(mst.count(0, 4, 5, 9) == 2);
    assert(mst.sum(0, 4, 5, 9) == 11);
}

// 4. index 範囲の切り出し
void test_4() {
    vector<Elem> init = { {5,10}, {2,7}, {8,1}, {4,6}, {3,9} };
    MST mst(init);
    assert(mst.count(1, 4, 0, 10) == 3);
    assert(mst.sum(1, 4, 0, 10) == 14);
    assert(mst.count(1, 4, 0, 5) == 2);
    assert(mst.sum(1, 4, 0, 5) == 13);
    assert(mst.count(1, 4, 5, 10) == 1);
    assert(mst.sum(1, 4, 5, 10) == 1);
}

// 5. 同じ key が複数ある
void test_5() {
    vector<Elem> init = { {5,10}, {5,3}, {5,7}, {2,4} };
    MST mst(init);
    assert(mst.count(0, 4, 5, 6) == 3);
    assert(mst.sum(0, 4, 5, 6) == 20);
    assert(mst.count(0, 4, 2, 6) == 4);
    assert(mst.sum(0, 4, 2, 6) == 24);
    assert(mst.count(0, 4, 0, 5) == 1);
    assert(mst.sum(0, 4, 0, 5) == 4);
}

// 6. 同じ key でも weight が異なる
void test_6() {
    vector<Elem> init = { {5,10}, {5,100}, {5,-3}, {1,8} };
    MST mst(init);
    assert(mst.count(0, 4, 5, 6) == 3);
    assert(mst.sum(0, 4, 5, 6) == 107);
    assert(mst.count(0, 4, 0, 5) == 1);
    assert(mst.sum(0, 4, 0, 5) == 8);
}

// 7. 半開区間 [xl, xr) の確認
void test_7() {
    vector<Elem> init = { {2,10}, {5,7}, {8,1} };
    MST mst(init);
    assert(mst.count(0, 3, 2, 2) == 0);
    assert(mst.sum(0, 3, 2, 2) == 0);
    assert(mst.count(0, 3, 2, 5) == 1);
    assert(mst.sum(0, 3, 2, 5) == 10);
    assert(mst.count(0, 3, 2, 6) == 2);
    assert(mst.sum(0, 3, 2, 6) == 17);
    assert(mst.count(0, 3, 5, 8) == 1);
    assert(mst.sum(0, 3, 5, 8) == 7);
    assert(mst.count(0, 3, 8, 9) == 1);
    assert(mst.sum(0, 3, 8, 9) == 1);
}

// 8. 半開区間 [l, r) の確認
void test_8() {
    vector<Elem> init = { {2,10}, {5,7}, {8,1}, {4,6} };
    MST mst(init);
    assert(mst.count(1, 1, 0, 10) == 0);
    assert(mst.sum(1, 1, 0, 10) == 0);
    assert(mst.count(1, 3, 0, 10) == 2);
    assert(mst.sum(1, 3, 0, 10) == 8);
    assert(mst.count(1, 3, 0, 8) == 1);
    assert(mst.sum(1, 3, 0, 8) == 7);
}

// 9. 負の key
void test_9() {
    vector<Elem> init = { {-5,10}, {0,3}, {2,7}, {-1,4} };
    MST mst(init);
    assert(mst.count(0, 4, -10, 0) == 2);
    assert(mst.sum(0, 4, -10, 0) == 14);
    assert(mst.count(0, 4, 0, 3) == 2);
    assert(mst.sum(0, 4, 0, 3) == 10);
}

// 10. 負の weight
void test_10() {
    vector<Elem> init = { {2,10}, {5,-3}, {8,7}, {4,-1} };
    MST mst(init);
    assert(mst.count(0, 4, 0, 10) == 4);
    assert(mst.sum(0, 4, 0, 10) == 13);
    assert(mst.count(0, 4, 4, 9) == 3);
    assert(mst.sum(0, 4, 4, 9) == 3);
}

// 11. key は同じで index が違う
void test_11() {
    vector<Elem> init = { {5,1}, {5,2}, {5,3}, {5,4} };
    MST mst(init);
    assert(mst.count(0, 4, 5, 6) == 4);
    assert(mst.sum(0, 4, 5, 6) == 10);
    assert(mst.count(1, 3, 5, 6) == 2);
    assert(mst.sum(1, 3, 5, 6) == 5);
}

// 12. value 範囲に 1 件も入らない
void test_12() {
    vector<Elem> init = { {2,10}, {5,7}, {8,1} };
    MST mst(init);
    assert(mst.count(0, 3, 100, 200) == 0);
    assert(mst.sum(0, 3, 100, 200) == 0);
    assert(mst.count(0, 3, -100, -1) == 0);
    assert(mst.sum(0, 3, -100, -1) == 0);
}

// 13. index 範囲に 1 件も入らない
void test_13() {
    vector<Elem> init = { {2,10}, {5,7}, {8,1} };
    MST mst(init);
    assert(mst.count(2, 2, 0, 10) == 0);
    assert(mst.sum(2, 2, 0, 10) == 0);
    assert(mst.count(3, 3, 0, 10) == 0);
    assert(mst.sum(3, 3, 0, 10) == 0);
}

// 14. 大きい値
void test_14() {
    vector<Elem> init = { {1000000000LL, 1000000000000LL}, {-1000000000LL, -1000000000000LL}, {0, 5} };
    MST mst(init);
    assert(mst.count(0, 3, -1000000000LL, 1000000001LL) == 3);
    assert(mst.sum(0, 3, -1000000000LL, 1000000001LL) == 5LL);
    assert(mst.count(0, 3, 0, 1000000001LL) == 2);
    assert(mst.sum(0, 3, 0, 1000000001LL) == 1000000000005LL);
}

// 15. 順不同入力でも正しく動く
void test_15() {
    vector<Elem> init = { {8,1}, {2,7}, {5,10}, {4,6}, {5,3} };
    MST mst(init);
    assert(mst.count(0, 5, 0, 6) == 4);
    assert(mst.sum(0, 5, 0, 6) == 26);
}

// 16. ランダム比較テスト
void test_16() {
    mt19937 rng(42);
    int N = 50;
    vector<Elem> init(N);
    for(int i = 0; i < N; ++i) {
        init[i] = { (long long)(rng() % 100), (long long)(rng() % 1000) };
    }
    MST mst(init);

    for(int test_c = 0; test_c < 100; ++test_c) {
        int l = rng() % (N + 1);
        int r = rng() % (N + 1);
        if(l > r) swap(l, r);

        long long xl = rng() % 100;
        long long xr = rng() % 100;
        if(xl > xr) swap(xl, xr);
        if(xl == xr) xr++;

        int expected_count = 0;
        long long expected_sum = 0;
        for(int i = l; i < r; ++i) {
            if(init[i].key >= xl && init[i].key < xr) {
                expected_count++;
                expected_sum += init[i].weight;
            }
        }
        assert(mst.count(l, r, xl, xr) == expected_count);
        assert(mst.sum(l, r, xl, xr) == expected_sum);
    }
}

int main() {
    test_1();
    test_2();
    test_3();
    test_4();
    test_5();
    test_6();
    test_7();
    test_8();
    test_9();
    test_10();
    test_11();
    test_12();
    test_13();
    test_14();
    test_15();
    test_16();
    cout << "All tests passed successfully!" << endl;
    return 0;
}
