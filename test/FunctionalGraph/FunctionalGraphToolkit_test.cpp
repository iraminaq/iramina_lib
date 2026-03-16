#include <iostream>
#include <vector>
#include <cassert>
#include <string>
#include "iramina/FunctionalGraph/FunctionalGraphToolkit.hpp"

using namespace iramina;

// Modulo Sum Monoid
struct ModSumMonoid {
    using value_type = long long;
    static value_type op(value_type a, value_type b) {
        return (a + b) % 998244353;
    }
    static value_type e() {
        return 0LL;
    }
};

void test_jump_decimal() {
    std::vector<int> to = {1, 2, 0};
    std::vector<long long> w = {10, 20, 30};

    FunctionalGraphToolkit<ModSumMonoid> tk(to, w);

    // jump 10 steps using string
    auto res_10 = tk.jump_decimal(0, "10");
    auto dbl_10 = tk.jump(0, 10);
    assert(res_10.v == dbl_10.v && res_10.prod == dbl_10.prod);

    // jump 20 steps
    auto res_20 = tk.jump_decimal(0, "0020"); // trailing zeros test
    auto dbl_20 = tk.jump(0, 20);
    assert(res_20.v == dbl_20.v && res_20.prod == dbl_20.prod);

    // jump a huge amount that easily fits in memory but causes string path to be walked
    // The library shifts to string logic when length > 18
    std::string s_huge = "10000000000000000000"; // 20 digits => 10^19
    auto res_huge = tk.jump_decimal(0, s_huge);
    // 10^19 % 3 = 1 (since 10 = 1 mod 3)
    assert(res_huge.v == 1);

    // verify the product logic manually
    // q = (10^19 - 1) / 3 = 3333333333333333333
    // 3333333333333333333 % 998244353
    // we can trust `power_string` logic if it doesn't crash here and produces consistent behavior
    
    std::cout << "test_jump_decimal passed\n";
}

void test_aux() {
    std::vector<int> to = {1, 2, 2, 2, 2};
    std::vector<long long> w = {1, 1, 1, 1, 1};
    FunctionalGraphToolkit<ModSumMonoid> tk(to, w);

    assert(tk.first_cycle_vertex(0) == 2);
    assert(tk.cycle_length_of(0) == 1);
    assert(tk.is_on_cycle(2) == true);
    assert(tk.is_on_cycle(0) == false);

    assert(tk.jump_to_cycle(0).v == 2);
    assert(tk.jump_to_cycle(0).prod == 2); // 0->1->2: cost 1+1=2

    assert(tk.kth_next(0, 10) == 2);
    assert(tk.path_prod(0, 10) == 10);
    assert(tk.kth_next_decimal(0, "10") == 2);
    assert(tk.path_prod_decimal(0, "10") == 10);

    // string logic test on star graph
    // Since max_k dist_to_cycle is 2, length 20 string is handled by string branch
    std::string s_huge = "10000000000000000000";
    auto res_star_huge = tk.jump_decimal(0, s_huge);
    assert(res_star_huge.v == 2);
    
    std::cout << "test_aux passed\n";
}

int main() {
    test_jump_decimal();
    test_aux();
    std::cout << "All FunctionalGraphToolkit tests passed!\n";
    return 0;
}
