#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>
#include "iramina/BinaryTree/TreapSum.hpp"

using namespace std;

void test_basic_operations() {
    TreapSum<int, long long> t;
    assert(t.empty() == true);
    assert(t.size() == 0);

    t.insert(10, 100);
    assert(t.empty() == false);
    assert(t.size() == 1);
    assert(t.count(10) == 1);
    assert(t.sum_key(10) == 100);

    t.insert(10, 50);
    assert(t.size() == 2);
    assert(t.count(10) == 2);
    assert(t.sum_key(10) == 150);

    bool erased = t.erase_one(10, 50);
    assert(erased == true);
    assert(t.size() == 1);
    assert(t.count(10) == 1);
    assert(t.sum_key(10) == 100);

    erased = t.erase_one(10, 100);
    assert(erased == true);
    assert(t.empty() == true);
    assert(t.size() == 0);
}

void test_queries() {
    TreapSum<int, long long> t;
    t.insert(30, 300);
    t.insert(10, 100);
    t.insert(40, 400);
    t.insert(20, 200);
    t.insert(10, 50);

    // Elements: (10, 100), (10, 50), (20, 200), (30, 300), (40, 400)
    // 10: cnt=2, sum=150
    // 20: cnt=1, sum=200
    // 30: cnt=1, sum=300
    // 40: cnt=1, sum=400
    
    assert(t.count_less(20) == 2);
    assert(t.sum_less(20) == 150);
    
    assert(t.count_less_equal(20) == 3);
    assert(t.sum_less_equal(20) == 350);

    assert(t.count_less_equal(40) == 5);
    assert(t.sum_less_equal(40) == 1050);

    assert(t.count_range(10, 30) == 3);
    assert(t.sum_range(10, 30) == 350);

    assert(t.count_range(20, 40) == 2);
    assert(t.sum_range(20, 40) == 500);
}

void test_kth() {
    TreapSum<int, long long> t;
    t.insert(30, 300);
    t.insert(10, 100);
    t.insert(50, 500);
    t.insert(10, 50);

    // Sorted elements: 10(100), 10(50), 30(300), 50(500)
    TreapSum<int, long long>::Entry e;
    
    assert(t.kth(0, e) == true);
    assert(e.key == 10 && e.cnt == 2 && e.weight_sum == 150);

    assert(t.kth(1, e) == true);
    assert(e.key == 10);

    assert(t.kth(2, e) == true);
    assert(e.key == 30 && e.weight_sum == 300);

    assert(t.kth(3, e) == true);
    assert(e.key == 50 && e.weight_sum == 500);

    assert(t.kth(4, e) == false);
    assert(t.kth(-1, e) == false);
}

void test_binary_search() {
    TreapSum<int, long long> t;
    t.insert(10, 100);
    t.insert(30, 300);
    t.insert(50, 500);

    TreapSum<int, long long>::Entry e;
    assert(t.lower_bound(30, e) == true && e.key == 30);
    assert(t.lower_bound(20, e) == true && e.key == 30);
    assert(t.lower_bound(50, e) == true && e.key == 50);
    assert(t.lower_bound(60, e) == false);

    assert(t.upper_bound(30, e) == true && e.key == 50);
    assert(t.upper_bound(10, e) == true && e.key == 30);
    assert(t.upper_bound(50, e) == false);
}

void test_stress() {
    const int N = 10000;
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> key_dist(0, 1000);
    std::uniform_int_distribution<long long> weight_dist(1, 10000);
    
    TreapSum<int, long long> t;
    
    struct Element { int key; long long weight; };
    std::vector<Element> vec;

    for (int i = 0; i < N * 2; i++) {
        int op = rng() % 3;
        if (op <= 1) { // Insert (approx 66%)
            int key = key_dist(rng);
            long long weight = weight_dist(rng);
            t.insert(key, weight);
            vec.push_back({key, weight});
        } else if (op == 2 && !vec.empty()) { // Erase (approx 33%)
            int idx = rng() % vec.size();
            Element to_erase = vec[idx];
            bool erased = t.erase_one(to_erase.key, to_erase.weight);
            assert(erased);
            vec.erase(vec.begin() + idx);
        }

        if (i % 100 == 0) { // Verify periodically
            assert(t.size() == (int)vec.size());
            
            // Query verifications
            int test_key = key_dist(rng);
            
            int expected_count_less = 0;
            long long expected_sum_less = 0;
            int expected_count = 0;
            for (auto item : vec) {
                if (item.key < test_key) {
                    expected_count_less++;
                    expected_sum_less += item.weight;
                }
                if (item.key == test_key) {
                    expected_count++;
                }
            }

            assert(t.count_less(test_key) == expected_count_less);
            assert(t.sum_less(test_key) == expected_sum_less);
            assert(t.count(test_key) == expected_count);
        }
    }
}

int main() {
    std::cout << "Running test_basic_operations..." << std::endl;
    test_basic_operations();
    
    std::cout << "Running test_queries..." << std::endl;
    test_queries();

    std::cout << "Running test_kth..." << std::endl;
    test_kth();

    std::cout << "Running test_binary_search..." << std::endl;
    test_binary_search();

    std::cout << "Running test_stress..." << std::endl;
    test_stress();

    std::cout << "All tests passed for TreapSum!" << std::endl;
    return 0;
}
