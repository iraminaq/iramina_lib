#include <iostream>
#include <vector>
#include <cassert>
#include "iramina/FunctionalGraph/FunctionalGraph.hpp"

using namespace iramina;

void test_all_in_one_cycle() {
    // 1. 全頂点が1つのサイクル
    std::vector<int> to = {1, 2, 3, 0};
    FunctionalGraph fg(to);

    assert(fg.n == 4);
    assert(fg.cycle_count() == 1);
    assert(fg.is_on_cycle(0) && fg.is_on_cycle(1) && fg.is_on_cycle(2) && fg.is_on_cycle(3));
    assert(fg.cycle_size(0) == 4);
    assert(fg.dist_to_cycle[0] == 0);
    assert(fg.dist_to_cycle[1] == 0);
    assert(fg.dist_to_cycle[2] == 0);
    assert(fg.dist_to_cycle[3] == 0);
    assert(fg.cycle_id[0] == 0);
    assert(fg.cycle_id[1] == 0);
    assert(fg.cycle_id[2] == 0);
    assert(fg.cycle_id[3] == 0);

    // サイクル上の位置チェック (サイクル列がトポロジカルに並んでいるか)
    for (int i = 0; i < 4; ++i) {
        assert(fg.root_cycle_vertex[i] == i);
    }

    std::cout << "test_all_in_one_cycle passed\n";
}

void test_cycle_with_trees() {
    // 2. サイクル + 木
    std::vector<int> to = {1, 2, 0, 2, 3};
    FunctionalGraph fg(to);

    assert(fg.cycle_count() == 1);
    // 0 -> 1 -> 2 -> 0 はサイクル。
    // 4 -> 3 -> 2 で 2に流れ込む
    assert(fg.dist_to_cycle[0] == 0); // cycle
    assert(fg.dist_to_cycle[1] == 0); // cycle
    assert(fg.dist_to_cycle[2] == 0); // cycle
    assert(fg.dist_to_cycle[3] == 1); // 3 -> 2
    assert(fg.dist_to_cycle[4] == 2); // 4 -> 3 -> 2
    
    assert(fg.root_cycle_vertex[0] == 0);
    assert(fg.root_cycle_vertex[1] == 1);
    assert(fg.root_cycle_vertex[2] == 2);
    assert(fg.root_cycle_vertex[3] == 2);
    assert(fg.root_cycle_vertex[4] == 2);
    
    assert(fg.cycle_pos[3] == -1);
    assert(fg.cycle_pos[4] == -1);

    std::cout << "test_cycle_with_trees passed\n";
}

void test_multiple_cycles() {
    // 3. 複数サイクル
    // to = {1,0,3,2,5,4}
    std::vector<int> to = {1, 0, 3, 2, 5, 4};
    FunctionalGraph fg(to);

    assert(fg.cycle_count() == 3);
    for(int i = 0; i < 6; ++i) {
        assert(fg.dist_to_cycle[i] == 0);
        assert(fg.is_on_cycle(i));
    }
    
    assert(fg.cycle_id[0] == fg.cycle_id[1]);
    assert(fg.cycle_id[2] == fg.cycle_id[3]);
    assert(fg.cycle_id[4] == fg.cycle_id[5]);
    assert(fg.cycle_id[0] != fg.cycle_id[2]);

    std::cout << "test_multiple_cycles passed\n";
}

void test_self_loops() {
    // 4. 自己ループあり
    // to = {0,0,1,2}
    std::vector<int> to = {0, 0, 1, 2};
    FunctionalGraph fg(to);

    // 0 -> 0 : サイクル
    // 1 -> 0
    // 2 -> 1 -> 0
    // 3 -> 2 -> 1 -> 0
    assert(fg.cycle_count() == 1);
    assert(fg.cycle_size(0) == 1);
    assert(fg.is_on_cycle(0));
    assert(!fg.is_on_cycle(1));
    assert(!fg.is_on_cycle(2));
    assert(!fg.is_on_cycle(3));

    assert(fg.dist_to_cycle[0] == 0);
    assert(fg.dist_to_cycle[1] == 1);
    assert(fg.dist_to_cycle[2] == 2);
    assert(fg.dist_to_cycle[3] == 3);

    for (int i = 0; i < 4; ++i) {
        assert(fg.root_cycle_vertex[i] == 0);
        assert(fg.cycle_id[i] == fg.cycle_id[0]);
    }

    std::cout << "test_self_loops passed\n";
}

void test_star_graph() {
    // 5. 星型にサイクルへ流れ込むケース
    // to = {1,2,2,2,2}
    std::vector<int> to = {1, 2, 2, 2, 2};
    FunctionalGraph fg(to);

    // 2 -> 2 はサイクル
    // 1 -> 2
    // 0 -> 1 -> 2
    // 3 -> 2
    // 4 -> 2
    assert(fg.cycle_count() == 1);
    assert(fg.cycle_size(0) == 1);
    assert(fg.is_on_cycle(2));

    assert(fg.dist_to_cycle[2] == 0);
    assert(fg.dist_to_cycle[1] == 1);
    assert(fg.dist_to_cycle[3] == 1);
    assert(fg.dist_to_cycle[4] == 1);
    assert(fg.dist_to_cycle[0] == 2);

    for (int i = 0; i < 5; ++i) {
        assert(fg.root_cycle_vertex[i] == 2);
    }

    std::cout << "test_star_graph passed\n";
}

void test_complex_spec_example() {
    // 仕様書の簡単な使用例イメージ
    std::vector<int> to = {1, 2, 0, 4, 5, 4};
    FunctionalGraph fg(to);

    assert(fg.cycle_count() == 2);
    assert(fg.dist_to_cycle[3] == 1);
    assert(fg.root_cycle_vertex[3] == 4);
    assert(fg.cycle_id[3] == fg.cycle_id[4]);
    assert(fg.cycle_pos[3] == -1);

    std::cout << "test_complex_spec_example passed\n";
}

int main() {
    test_all_in_one_cycle();
    test_cycle_with_trees();
    test_multiple_cycles();
    test_self_loops();
    test_star_graph();
    test_complex_spec_example();
    std::cout << "All FunctionalGraph tests passed!\n";
    return 0;
}
