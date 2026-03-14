#include <cassert>
#include <iostream>
#include <vector>
#include <random>
using namespace std;

// TreapSum, DynamicMergeSortTree を include 済み想定
#include <iramina/TreapSum/TreapSum.hpp>
#include <iramina/DynamicMergeSortTree/DynamicMergeSortTree.hpp>

void test_single_element() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
    };
    DMST seg(init);

    assert(seg.size() == 1);

    auto e = seg.get(0);
    assert(e.key == 5);
    assert(e.weight == 10);

    assert(seg.count(0, 1, 0, 10) == 1);
    assert(seg.sum(0, 1, 0, 10) == 10);

    assert(seg.count(0, 1, 5, 6) == 1);
    assert(seg.sum(0, 1, 5, 6) == 10);

    assert(seg.count(0, 1, 0, 5) == 0);
    assert(seg.sum(0, 1, 0, 5) == 0);

    assert(seg.count(0, 1, 6, 10) == 0);
    assert(seg.sum(0, 1, 6, 10) == 0);
}

void test_basic_multiple_keys() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {2, 7},
        {8, 1},
        {4, 6},
    };
    DMST seg(init);

    assert(seg.size() == 4);

    assert(seg.get(0).key == 5 && seg.get(0).weight == 10);
    assert(seg.get(1).key == 2 && seg.get(1).weight == 7);
    assert(seg.get(2).key == 8 && seg.get(2).weight == 1);
    assert(seg.get(3).key == 4 && seg.get(3).weight == 6);

    assert(seg.count(0, 4, 0, 10) == 4);
    assert(seg.sum(0, 4, 0, 10) == 24);

    assert(seg.count(0, 4, 0, 5) == 2);
    assert(seg.sum(0, 4, 0, 5) == 13);

    assert(seg.count(0, 4, 5, 9) == 2);
    assert(seg.sum(0, 4, 5, 9) == 11);
}

void test_index_range_queries() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {2, 7},
        {8, 1},
        {4, 6},
        {3, 9},
    };
    DMST seg(init);

    assert(seg.count(1, 4, 0, 10) == 3);
    assert(seg.sum(1, 4, 0, 10) == 14);

    assert(seg.count(1, 4, 0, 5) == 2);
    assert(seg.sum(1, 4, 0, 5) == 13);

    assert(seg.count(1, 4, 5, 10) == 1);
    assert(seg.sum(1, 4, 5, 10) == 1);
}

void test_duplicate_keys() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {5, 3},
        {5, 7},
        {2, 4},
    };
    DMST seg(init);

    assert(seg.count(0, 4, 5, 6) == 3);
    assert(seg.sum(0, 4, 5, 6) == 20);

    assert(seg.count(0, 4, 2, 6) == 4);
    assert(seg.sum(0, 4, 2, 6) == 24);

    assert(seg.count(1, 3, 5, 6) == 2);
    assert(seg.sum(1, 3, 5, 6) == 10);
}

void test_duplicate_keys_different_weights() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {5, 100},
        {5, -3},
        {1, 8},
    };
    DMST seg(init);

    assert(seg.count(0, 4, 5, 6) == 3);
    assert(seg.sum(0, 4, 5, 6) == 107);

    assert(seg.count(0, 4, 0, 5) == 1);
    assert(seg.sum(0, 4, 0, 5) == 8);
}

void test_half_open_value_range() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {2, 10},
        {5, 7},
        {8, 1},
    };
    DMST seg(init);

    assert(seg.count(0, 3, 2, 2) == 0);
    assert(seg.sum(0, 3, 2, 2) == 0);

    assert(seg.count(0, 3, 2, 5) == 1);
    assert(seg.sum(0, 3, 2, 5) == 10);

    assert(seg.count(0, 3, 2, 6) == 2);
    assert(seg.sum(0, 3, 2, 6) == 17);

    assert(seg.count(0, 3, 5, 8) == 1);
    assert(seg.sum(0, 3, 5, 8) == 7);

    assert(seg.count(0, 3, 8, 9) == 1);
    assert(seg.sum(0, 3, 8, 9) == 1);
}

void test_half_open_index_range() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {2, 10},
        {5, 7},
        {8, 1},
        {4, 6},
    };
    DMST seg(init);

    assert(seg.count(1, 1, 0, 10) == 0);
    assert(seg.sum(1, 1, 0, 10) == 0);

    assert(seg.count(1, 3, 0, 10) == 2);
    assert(seg.sum(1, 3, 0, 10) == 8);

    assert(seg.count(1, 3, 0, 8) == 1);
    assert(seg.sum(1, 3, 0, 8) == 7);
}

void test_set_change_key() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {2, 7},
        {8, 1},
    };
    DMST seg(init);

    seg.set(1, 6, 20);

    auto e = seg.get(1);
    assert(e.key == 6);
    assert(e.weight == 20);

    assert(seg.count(0, 3, 0, 10) == 3);
    assert(seg.sum(0, 3, 0, 10) == 31);

    assert(seg.count(0, 3, 0, 6) == 1);
    assert(seg.sum(0, 3, 0, 6) == 10);

    assert(seg.count(0, 3, 6, 9) == 2);
    assert(seg.sum(0, 3, 6, 9) == 21);
}

void test_set_change_weight_only() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {2, 7},
        {8, 1},
    };
    DMST seg(init);

    seg.set(0, 5, 100);

    auto e = seg.get(0);
    assert(e.key == 5);
    assert(e.weight == 100);

    assert(seg.count(0, 3, 0, 10) == 3);
    assert(seg.sum(0, 3, 0, 10) == 108);

    assert(seg.count(0, 3, 5, 6) == 1);
    assert(seg.sum(0, 3, 5, 6) == 100);
}

void test_set_increase_duplicate_key() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {2, 7},
        {8, 1},
    };
    DMST seg(init);

    seg.set(2, 5, 3);

    assert(seg.count(0, 3, 5, 6) == 2);
    assert(seg.sum(0, 3, 5, 6) == 13);

    assert(seg.count(0, 3, 0, 10) == 3);
    assert(seg.sum(0, 3, 0, 10) == 20);
}

void test_set_decrease_duplicate_key() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {5, 3},
        {2, 7},
    };
    DMST seg(init);

    seg.set(1, 8, 1);

    assert(seg.count(0, 3, 5, 6) == 1);
    assert(seg.sum(0, 3, 5, 6) == 10);

    assert(seg.count(0, 3, 8, 9) == 1);
    assert(seg.sum(0, 3, 8, 9) == 1);
}

void test_multiple_updates_same_position() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    DMST seg(init);

    seg.set(1, 5, 10);
    seg.set(1, 2, 7);
    seg.set(1, -1, 4);

    auto e = seg.get(1);
    assert(e.key == -1);
    assert(e.weight == 4);

    assert(seg.count(0, 3, -10, 10) == 3);
    assert(seg.sum(0, 3, -10, 10) == 8);

    assert(seg.count(0, 3, -10, 0) == 1);
    assert(seg.sum(0, 3, -10, 0) == 4);
}

void test_negative_key() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {-5, 10},
        {0, 3},
        {2, 7},
        {-1, 4},
    };
    DMST seg(init);

    assert(seg.count(0, 4, -10, 0) == 2);
    assert(seg.sum(0, 4, -10, 0) == 14);

    assert(seg.count(0, 4, 0, 3) == 2);
    assert(seg.sum(0, 4, 0, 3) == 10);
}

void test_negative_weight() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {2, 10},
        {5, -3},
        {8, 7},
        {4, -1},
    };
    DMST seg(init);

    assert(seg.count(0, 4, 0, 10) == 4);
    assert(seg.sum(0, 4, 0, 10) == 13);

    assert(seg.count(0, 4, 4, 9) == 3);
    assert(seg.sum(0, 4, 4, 9) == 3);

    seg.set(1, 5, 100);

    assert(seg.count(0, 4, 4, 9) == 3);
    assert(seg.sum(0, 4, 4, 9) == 106);
}

void test_after_updates_consistency() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {2, 7},
        {8, 1},
        {4, 6},
        {3, 9},
    };
    DMST seg(init);

    seg.set(1, 100, 20);
    seg.set(3, 5, 1);

    assert(seg.count(0, 5, 5, 9) == 3);
    assert(seg.sum(0, 5, 5, 9) == 12);

    assert(seg.count(1, 4, 0, 10) == 2);
    assert(seg.sum(1, 4, 0, 10) == 2);
}

void test_value_range_becomes_empty() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {5, 10},
        {5, 3},
        {5, 7},
    };
    DMST seg(init);

    seg.set(0, 1, 1);
    seg.set(1, 2, 2);
    seg.set(2, 3, 3);

    assert(seg.count(0, 3, 5, 6) == 0);
    assert(seg.sum(0, 3, 5, 6) == 0);
}

void test_new_value_range_appears() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    DMST seg(init);

    seg.set(1, 10, 100);

    assert(seg.count(0, 3, 10, 11) == 1);
    assert(seg.sum(0, 3, 10, 11) == 100);
}

void test_large_values() {
    using DMST = DynamicMergeSortTree<int, long long>;
    vector<DMST::Elem> init = {
        {1000000000, 1000000000000LL},
        {-1000000000, -1000000000000LL},
        {0, 5},
    };
    DMST seg(init);

    assert(seg.count(0, 3, -1000000000, 1000000001) == 3);
    assert(seg.sum(0, 3, -1000000000, 1000000001) == 5);

    assert(seg.count(0, 3, 0, 1000000001) == 2);
    assert(seg.sum(0, 3, 0, 1000000001) == 1000000000005LL);

    seg.set(2, 1000000000, 7);

    assert(seg.count(0, 3, 1000000000, 1000000001) == 2);
    assert(seg.sum(0, 3, 1000000000, 1000000001) == 1000000000007LL);
}

void test_random() {
    using DMST = DynamicMergeSortTree<int, long long>;
    mt19937 rng(42);
    int N = 100;
    vector<DMST::Elem> init(N);
    for(int i = 0; i < N; ++i) {
        init[i] = { (int)(rng() % 100), (long long)(rng() % 1000) };
    }
    DMST seg(init);
    auto naive_state = init;

    for(int test_c = 0; test_c < 500; ++test_c) {
        int type = rng() % 3;
        if (type == 0) { // set
            int p = rng() % N;
            int k = rng() % 100;
            long long w = rng() % 1000;
            seg.set(p, k, w);
            naive_state[p] = {k, w};
        } else { // query
            int l = rng() % (N + 1);
            int r = rng() % (N + 1);
            if(l > r) swap(l, r);

            int xl = rng() % 100;
            int xr = rng() % 100;
            if(xl > xr) swap(xl, xr);
            if(xl == xr) xr++;

            int expected_count = 0;
            long long expected_sum = 0;
            for(int i = l; i < r; ++i) {
                if(naive_state[i].key >= xl && naive_state[i].key < xr) {
                    expected_count++;
                    expected_sum += naive_state[i].weight;
                }
            }
            int actual_count = seg.count(l, r, xl, xr);
            long long actual_sum = seg.sum(l, r, xl, xr);
            if (actual_count != expected_count || actual_sum != expected_sum) {
                cerr << "Mismatch found!\n";
                cerr << "Query: l=" << l << ", r=" << r << ", xl=" << xl << ", xr=" << xr << "\n";
                cerr << "Expected: count=" << expected_count << ", sum=" << expected_sum << "\n";
                cerr << "Actual: count=" << actual_count << ", sum=" << actual_sum << "\n";
            }
            assert(actual_count == expected_count);
            assert(actual_sum == expected_sum);
        }
    }
}

int main() {
    test_single_element();
    test_basic_multiple_keys();
    test_index_range_queries();
    test_duplicate_keys();
    test_duplicate_keys_different_weights();
    test_half_open_value_range();
    test_half_open_index_range();
    test_set_change_key();
    test_set_change_weight_only();
    test_set_increase_duplicate_key();
    test_set_decrease_duplicate_key();
    test_multiple_updates_same_position();
    test_negative_key();
    test_negative_weight();
    test_after_updates_consistency();
    test_value_range_becomes_empty();
    test_new_value_range_appears();
    test_large_values();
    test_random();

    cout << "OK\n";
    return 0;
}