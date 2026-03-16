#include <iostream>
#include <vector>
#include <cassert>
#include "iramina/FunctionalGraph/FunctionalGraphDoubling.hpp"

using namespace iramina;

// Sum
struct SumMonoid {
    using value_type = long long;
    static value_type op(value_type a, value_type b) {
        return a + b;
    }
    static value_type e() {
        return 0LL;
    }
};

// Affine
struct AffineMonoid {
    struct F {
        long long a, b; // 表示上 a*x + b と解釈する
    };
    using value_type = F;

    static F op(const F& f, const F& g) {
        // 先に f を適用し、次に g を適用する
        // x -> f.a*x + f.b
        // f(x) -> g.a*(f.a*x + f.b) + g.b
        //      = (g.a * f.a) * x + (g.a * f.b + g.b)
        return { g.a * f.a, g.a * f.b + g.b };
    }

    static F e() {
        return {1, 0};
    }
};

void test_sum() {
    // 1つのサイクル
    std::vector<int> to = {1, 2, 0};
    std::vector<long long> w = {10, 20, 30};

    FunctionalGraphDoubling<SumMonoid> dbl(to, w, 10);

    auto res0 = dbl.jump(0, 0);
    assert(res0.v == 0 && res0.prod == 0);

    auto res1 = dbl.jump(0, 1);
    assert(res1.v == 1 && res1.prod == 10);

    auto res2 = dbl.jump(0, 2);
    assert(res2.v == 2 && res2.prod == 30); // 10 + 20

    auto res3 = dbl.jump(0, 3);
    assert(res3.v == 0 && res3.prod == 60); // 10 + 20 + 30

    auto res4 = dbl.jump(0, 4);
    assert(res4.v == 1 && res4.prod == 70); // 10 + 20 + 30 + 10

    // 木付き functional graph のテスト
    std::vector<int> to2 = {1, 2, 2, 2};
    std::vector<long long> w2 = {3, 4, 5, 6};
    FunctionalGraphDoubling<SumMonoid> dbl2(to2, w2, 10);

    assert(dbl2.jump(0, 1).v == 1 && dbl2.jump(0, 1).prod == 3);
    assert(dbl2.jump(0, 2).v == 2 && dbl2.jump(0, 2).prod == 7);
    assert(dbl2.jump(0, 5).v == 2 && dbl2.jump(0, 5).prod == 22); // 3 + 4 + 5 + 5 + 5

    // auxiliary methods
    assert(dbl2.kth_next(0, 5) == 2);
    assert(dbl2.path_prod(0, 5) == 22);
    
    std::cout << "test_sum ok" << std::endl;
}

void test_affine() {
    // Affine: f(x) = a*x + b
    std::vector<int> to = {1, 2, 0};
    std::vector<AffineMonoid::F> w = {
        {2, 1},  // 0 -> 1: 2x+1
        {3, 2},  // 1 -> 2: 3x+2
        {1, 5}   // 2 -> 0: x+5
    };
    
    FunctionalGraphDoubling<AffineMonoid> dbl(to, w, 10);

    // 0 から 2ステップ (0->1->2)
    // 最初のステップ: x -> 2x+1
    // 次のステップ: (2x+1) -> 3(2x+1)+2 = 6x+5
    // a=6, b=5 になるはず
    auto res2 = dbl.jump(0, 2);
    assert(res2.v == 2);
    assert(res2.prod.a == 6 && res2.prod.b == 5);

    // 0 から 0ステップ
    auto res0 = dbl.jump(0, 0);
    assert(res0.v == 0);
    assert(res0.prod.a == 1 && res0.prod.b == 0); // e()

    std::cout << "test_affine ok" << std::endl;
}

void test_zero_max_k() {
    std::vector<int> to = {1, 2, 0};
    std::vector<long long> w = {10, 20, 30};

    FunctionalGraphDoubling<SumMonoid> dbl(to, w, 0);
    
    assert(dbl.jump(0, 0).v == 0);
    assert(dbl.jump(0, 0).prod == 0);

    std::cout << "test_zero_max_k ok" << std::endl;
}

int main() {
    test_sum();
    test_affine();
    test_zero_max_k();
    std::cout << "All FunctionalGraphDoubling tests passed!" << std::endl;
    return 0;
}
