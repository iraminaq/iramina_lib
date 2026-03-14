#undef NDEBUG
#include <cassert>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>

#include "../include/iramina/CartesianTree/CartesianTree.hpp"

using namespace std;

// 1. 空配列
void test_1() {
    vector<int> a = {};
    CartesianTree<int> ct(a);
    assert(ct.size() == 0);
    assert(ct.root() == -1);
    ct.build_lca(); // Should not crash
}

// 2. 要素数 1
void test_2() {
    vector<int> a = {42};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.size() == 1);
    assert(ct.root() == 0);
    assert(ct.parent(0) == -1);
    assert(ct.left_child(0) == -1);
    assert(ct.right_child(0) == -1);
    assert(ct.value(0) == 42);
    auto range = ct.subtree_range(0);
    assert(range.first == 0 && range.second == 1);
    assert(ct.subtree_size(0) == 1);
    assert(ct.query_index(0, 1) == 0);
}

// 3. strictly increasing（min-cartesian）
void test_3() {
    vector<int> a = {1, 2, 3, 4, 5};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.root() == 0);
    assert(ct.parent(0) == -1);
    assert(ct.right_child(0) == 1);
    assert(ct.right_child(1) == 2);
    assert(ct.right_child(2) == 3);
    assert(ct.right_child(3) == 4);
    for(int i = 0; i < 5; i++) assert(ct.left_child(i) == -1);
    
    assert(ct.query_index(0, 5) == 0);
    assert(ct.query_index(1, 5) == 1);
    assert(ct.query_index(2, 4) == 2);
    assert(ct.query_index(4, 5) == 4);
    
    auto r0 = ct.subtree_range(0); assert(r0.first == 0 && r0.second == 5);
    auto r1 = ct.subtree_range(1); assert(r1.first == 1 && r1.second == 5);
    auto r2 = ct.subtree_range(2); assert(r2.first == 2 && r2.second == 5);
    assert(ct.subtree_size(0) == 5);
    assert(ct.subtree_size(3) == 2);
}

// 4. strictly decreasing（min-cartesian）
void test_4() {
    vector<int> a = {5, 4, 3, 2, 1};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.root() == 4);
    assert(ct.parent(4) == -1);
    assert(ct.left_child(4) == 3);
    assert(ct.left_child(3) == 2);
    assert(ct.left_child(2) == 1);
    assert(ct.left_child(1) == 0);
    for(int i = 0; i < 5; i++) assert(ct.right_child(i) == -1);
    
    assert(ct.query_index(0, 5) == 4);
    assert(ct.query_index(0, 4) == 3);
    assert(ct.query_index(0, 3) == 2);
    assert(ct.query_index(1, 2) == 1);
}

// 5. strictly increasing（max-cartesian）
void test_5() {
    vector<int> a = {1, 2, 3, 4, 5};
    CartesianTree<int, greater<int>> ct(a);
    ct.build_lca();
    assert(ct.root() == 4);
    assert(ct.query_index(0, 5) == 4);
    assert(ct.query_index(0, 4) == 3);
    assert(ct.query_index(1, 3) == 2);
}

// 6. strictly decreasing（max-cartesian）
void test_6() {
    vector<int> a = {5, 4, 3, 2, 1};
    CartesianTree<int, greater<int>> ct(a);
    ct.build_lca();
    assert(ct.root() == 0);
    assert(ct.query_index(0, 5) == 0);
    assert(ct.query_index(1, 5) == 1);
    assert(ct.query_index(2, 4) == 2);
}

// 7. 一般的な配列
void test_7() {
    vector<int> a = {1, 3, 4, 2, 6};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.query_index(0, 5) == 0);
    assert(ct.query_index(1, 4) == 3);
    assert(ct.query_index(1, 3) == 1);
    assert(ct.query_index(2, 5) == 3);
    assert(ct.query_index(4, 5) == 4);
    
    for(int v = 0; v < 5; v++) {
        auto r = ct.subtree_range(v);
        assert(r.second - r.first == ct.subtree_size(v));
    }
}

// 8. 重複値のみ
void test_8() {
    vector<int> a = {5, 5, 5, 5};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.root() == 0);
    assert(ct.query_index(0, 4) == 0);
    assert(ct.query_index(1, 4) == 1);
}

// 9. 重複を含む一般ケース
void test_9() {
    vector<int> a = {2, 1, 1, 3, 1};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.query_index(0, 5) == 1);
    assert(ct.query_index(1, 3) == 1);
    assert(ct.query_index(2, 5) == 2);
}

// 10. `subtree_range` の整合性
void test_10() {
    vector<int> a = {4, 2, 5, 1, 3};
    CartesianTree<int> ct(a);
    int n = a.size();
    for(int v = 0; v < n; v++) {
        auto r = ct.subtree_range(v);
        assert(0 <= r.first && r.first < r.second && r.second <= n);
        assert(ct.subtree_size(v) == r.second - r.first);
        assert(r.first <= v && v < r.second);
    }
}

// 11. 親子関係の整合性
void test_11() {
    vector<int> a = {4, 2, 5, 1, 3};
    CartesianTree<int> ct(a);
    int root = ct.root();
    assert(ct.parent(root) == -1);
    for(int v = 0; v < (int)a.size(); v++) {
        int lc = ct.left_child(v);
        if (lc != -1) assert(ct.parent(lc) == v);
        int rc = ct.right_child(v);
        if (rc != -1) assert(ct.parent(rc) == v);
    }
}

// 12. inorder が元配列順になること
void test_12_dfs(int v, const CartesianTree<int>& ct, vector<int>& inorder) {
    if (v == -1) return;
    test_12_dfs(ct.left_child(v), ct, inorder);
    inorder.push_back(v);
    test_12_dfs(ct.right_child(v), ct, inorder);
}
void test_12() {
    vector<int> a = {4, 2, 5, 1, 3};
    CartesianTree<int> ct(a);
    vector<int> inorder;
    test_12_dfs(ct.root(), ct, inorder);
    for(int i = 0; i < (int)a.size(); i++) {
        assert(inorder[i] == i);
    }
}

// 13. comparator 条件の確認
void test_13() {
    vector<int> a = {4, 2, 5, 1, 3};
    CartesianTree<int> ct_min(a);
    for(int i = 0; i < (int)a.size(); i++) {
        int p = ct_min.parent(i);
        if (p != -1) assert(a[p] <= a[i]);
    }
    CartesianTree<int, greater<int>> ct_max(a);
    for(int i = 0; i < (int)a.size(); i++) {
        int p = ct_max.parent(i);
        if (p != -1) assert(a[p] >= a[i]);
    }
}

// 14. `build_lca()` を複数回呼ぶ
void test_14() {
    vector<int> a = {3, 1, 4, 2};
    CartesianTree<int> ct(a);
    ct.build_lca();
    int res1 = ct.query_index(0, 4);
    ct.build_lca();
    ct.build_lca();
    int res2 = ct.query_index(0, 4);
    assert(res1 == res2);
}

// 15. `build_lca()` 前後で基本 API が変わらない
void test_15() {
    vector<int> a = {3, 1, 4, 2};
    CartesianTree<int> ct(a);
    int ro = ct.root();
    vector<int> p(4), lc(4), rc(4), ss(4), val(4);
    vector<pair<int, int>> sr(4);
    for(int i=0; i<4; i++) {
        p[i] = ct.parent(i); lc[i] = ct.left_child(i); rc[i] = ct.right_child(i);
        ss[i] = ct.subtree_size(i); val[i] = ct.value(i); sr[i] = ct.subtree_range(i);
    }
    ct.build_lca();
    assert(ro == ct.root());
    for(int i=0; i<4; i++) {
        assert(p[i] == ct.parent(i)); assert(lc[i] == ct.left_child(i)); assert(rc[i] == ct.right_child(i));
        assert(ss[i] == ct.subtree_size(i)); assert(val[i] == ct.value(i)); assert(sr[i] == ct.subtree_range(i));
    }
}

// 16. 小区間クエリ
void test_16() {
    vector<int> a = {7, 2, 9, 1, 5};
    CartesianTree<int> ct(a);
    ct.build_lca();
    assert(ct.query_index(0, 1) == 0);
    assert(ct.query_index(1, 2) == 1);
    assert(ct.query_index(4, 5) == 4);
    assert(ct.query_index(0, 2) == 1);
    assert(ct.query_index(2, 4) == 3);
    assert(ct.query_index(3, 5) == 3);
}

// 17. 全区間総当たりチェック
void test_17() {
    vector<int> a = {4, 1, 3, 2};
    CartesianTree<int> ct_min(a);
    ct_min.build_lca();
    int n = a.size();
    for(int l = 0; l < n; l++) {
        for(int r = l + 1; r <= n; r++) {
            int idx = ct_min.query_index(l, r);
            int expected = min_element(a.begin() + l, a.begin() + r) - a.begin();
            assert(idx == expected);
        }
    }
    CartesianTree<int, greater<int>> ct_max(a);
    ct_max.build_lca();
    for(int l = 0; l < n; l++) {
        for(int r = l + 1; r <= n; r++) {
            int idx = ct_max.query_index(l, r);
            int expected = max_element(a.begin() + l, a.begin() + r) - a.begin();
            assert(idx == expected);
        }
    }
}

// 18. ランダムテスト
void test_18(int n) {
    mt19937 rng(42);
    vector<int> a(n);
    for(int i = 0; i < n; i++) a[i] = rng() % 100;

    auto verify_tree = [&](auto& ct, auto comp) {
        vector<int> inorder;
        auto dfs = [&](auto dfs, int v) -> void {
            if (v == -1) return;
            dfs(dfs, ct.left_child(v));
            inorder.push_back(v);
            dfs(dfs, ct.right_child(v));
        };
        dfs(dfs, ct.root());
        for(int i=0; i<n; i++) assert(inorder[i] == i);

        for(int i=0; i<n; i++) {
            int p = ct.parent(i);
            if(p != -1) {
                assert(!comp(a[i], a[p]));
                assert(ct.depth(i) == ct.depth(p) + 1);
            } else {
                assert(ct.depth(i) == 0);
            }
            auto r = ct.subtree_range(i);
            assert(ct.subtree_size(i) == r.second - r.first);
            assert(r.first <= i && i < r.second);
            
            auto [lc, rc] = ct.children(i);
            assert(lc == ct.left_child(i));
            assert(rc == ct.right_child(i));
        }

        for(int u=0; u<n; u++) {
            for(int v=0; v<n; v++) {
                assert(ct.is_ancestor(u, v) == (ct.lca(u, v) == u));
            }
        }

        for(int i=0; i<n; i++) {
            int p = ct.parent(i);
            if(p != -1) {
                assert(!comp(a[i], a[p]));
            }
            auto r = ct.subtree_range(i);
            assert(ct.subtree_size(i) == r.second - r.first);
            assert(r.first <= i && i < r.second);
        }
    };

    CartesianTree<int> ct_min(a);
    ct_min.build_lca();
    verify_tree(ct_min, less<int>{});
    for(int l = 0; l < n; l++) {
        for(int r = l + 1; r <= n; r++) {
            int idx = ct_min.query_index(l, r);
            int exp = l;
            for(int i=l+1; i<r; i++) if(a[i] < a[exp]) exp = i;
            assert(idx == exp);
            assert(ct_min.lca(l, r - 1) == idx);
            assert(ct_min.query(l, r) == a[idx]);
        }
    }

    CartesianTree<int, greater<int>> ct_max(a);
    ct_max.build_lca();
    verify_tree(ct_max, greater<int>{});
    for(int l = 0; l < n; l++) {
        for(int r = l + 1; r <= n; r++) {
            int idx = ct_max.query_index(l, r);
            int exp = l;
            for(int i=l+1; i<r; i++) if(a[i] > a[exp]) exp = i;
            assert(idx == exp);
            assert(ct_max.lca(l, r - 1) == idx);
            assert(ct_max.query(l, r) == a[idx]);
        }
    }
}
void test_18_wrapper() {
    for(int i=1; i<=20; i++) test_18(i);
}

// 19. 大きめ入力での性能確認
void test_19() {
    int n = 200000;
    vector<int> a(n);
    mt19937 rng(1337);
    for(int i=0; i<n; i++) a[i] = rng();
    
    auto t1 = chrono::high_resolution_clock::now();
    CartesianTree<int> ct(a);
    auto t2 = chrono::high_resolution_clock::now();
    ct.build_lca();
    auto t3 = chrono::high_resolution_clock::now();
    
    long long volatile sum = 0; // prevent optimization
    for(int l=0; l<n-10; l++) {
        sum += ct.query_index(l, l+10);
    }
    auto t4 = chrono::high_resolution_clock::now();
    
    long long t_build = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    long long t_lca = chrono::duration_cast<chrono::milliseconds>(t3 - t2).count();
    long long t_query = chrono::duration_cast<chrono::milliseconds>(t4 - t3).count();
    
    // cout << "build_tree (200k): " << t_build << " ms\n";
    // cout << "build_lca (200k): " << t_lca << " ms\n";
    // cout << "query 200k times: " << t_query << " ms\n";
}

// 21. lca(u, v) のテスト
void test_21() {
    vector<int> a1 = {42};
    CartesianTree<int> ct1(a1); ct1.build_lca();
    assert(ct1.lca(0, 0) == 0);

    vector<int> a2 = {1, 2, 3, 4, 5};
    CartesianTree<int> ct2(a2); ct2.build_lca();
    assert(ct2.lca(0, 0) == 0); assert(ct2.lca(0, 4) == 0);
    assert(ct2.lca(1, 4) == 1); assert(ct2.lca(2, 3) == 2);

    vector<int> a3 = {5, 4, 3, 2, 1};
    CartesianTree<int> ct3(a3); ct3.build_lca();
    assert(ct3.lca(4, 4) == 4); assert(ct3.lca(0, 4) == 4);
    assert(ct3.lca(0, 3) == 3); assert(ct3.lca(1, 2) == 2);

    vector<int> a4 = {1, 3, 4, 2, 6};
    CartesianTree<int> ct4(a4); ct4.build_lca();
    assert(ct4.lca(1, 2) == 1); assert(ct4.lca(1, 3) == 3);
    assert(ct4.lca(2, 4) == 3); assert(ct4.lca(0, 4) == 0);

    for(int l = 0; l < 5; l++) {
        for(int r = l + 1; r <= 5; r++) {
            assert(ct4.query_index(l, r) == ct4.lca(l, r - 1));
        }
    }
}

// 22. depth(v) のテスト
void test_22() {
    vector<int> a1 = {42};
    CartesianTree<int> ct1(a1); ct1.build_lca();
    assert(ct1.depth(0) == 0);

    vector<int> a2 = {1, 2, 3, 4, 5};
    CartesianTree<int> ct2(a2); ct2.build_lca();
    for(int i = 0; i < 5; i++) assert(ct2.depth(i) == i);

    vector<int> a3 = {5, 4, 3, 2, 1};
    CartesianTree<int> ct3(a3); ct3.build_lca();
    for(int i = 0; i < 5; i++) assert(ct3.depth(i) == 4 - i);

    vector<int> a4 = {1, 3, 4, 2, 6};
    CartesianTree<int> ct4(a4); ct4.build_lca();
    assert(ct4.depth(0) == 0); assert(ct4.depth(3) == 1);
    assert(ct4.depth(1) == 2); assert(ct4.depth(2) == 3);
    assert(ct4.depth(4) == 2);
    for(int i = 0; i < 5; i++) {
        int p = ct4.parent(i);
        if (p == -1) assert(ct4.depth(i) == 0);
        else assert(ct4.depth(i) == ct4.depth(p) + 1);
    }
}

// 23. children(v) のテスト
void test_23() {
    vector<int> a1 = {42};
    CartesianTree<int> ct1(a1);
    assert(ct1.children(0) == make_pair(-1, -1));

    vector<int> a2 = {1, 2, 3, 4, 5};
    CartesianTree<int> ct2(a2);
    assert(ct2.children(0) == make_pair(-1, 1));
    assert(ct2.children(1) == make_pair(-1, 2));
    assert(ct2.children(4) == make_pair(-1, -1));

    vector<int> a3 = {5, 4, 3, 2, 1};
    CartesianTree<int> ct3(a3);
    assert(ct3.children(4) == make_pair(3, -1));
    assert(ct3.children(3) == make_pair(2, -1));
    assert(ct3.children(0) == make_pair(-1, -1));

    vector<int> a4 = {1, 3, 4, 2, 6};
    CartesianTree<int> ct4(a4);
    assert(ct4.children(0) == make_pair(-1, 3));
    assert(ct4.children(3) == make_pair(1, 4));
    assert(ct4.children(1) == make_pair(-1, 2));
    assert(ct4.children(2) == make_pair(-1, -1));
    assert(ct4.children(4) == make_pair(-1, -1));

    for(int i = 0; i < 5; i++) {
        auto [lc, rc] = ct4.children(i);
        assert(lc == ct4.left_child(i));
        assert(rc == ct4.right_child(i));
    }
}

// 24. is_ancestor(u, v) のテスト
void test_24() {
    vector<int> a1 = {42};
    CartesianTree<int> ct1(a1); ct1.build_lca();
    assert(ct1.is_ancestor(0, 0));

    vector<int> a2 = {1, 2, 3, 4, 5};
    CartesianTree<int> ct2(a2); ct2.build_lca();
    assert(ct2.is_ancestor(0, 4)); assert(ct2.is_ancestor(1, 4));
    assert(ct2.is_ancestor(2, 2)); assert(!ct2.is_ancestor(3, 1));

    vector<int> a3 = {5, 4, 3, 2, 1};
    CartesianTree<int> ct3(a3); ct3.build_lca();
    assert(ct3.is_ancestor(4, 0)); assert(ct3.is_ancestor(3, 1));
    assert(!ct3.is_ancestor(2, 4));

    vector<int> a4 = {1, 3, 4, 2, 6};
    CartesianTree<int> ct4(a4); ct4.build_lca();
    assert(ct4.is_ancestor(0, 4)); assert(ct4.is_ancestor(3, 1));
    assert(ct4.is_ancestor(3, 2)); assert(ct4.is_ancestor(1, 2));
    assert(!ct4.is_ancestor(1, 4)); assert(!ct4.is_ancestor(4, 1));

    for(int u = 0; u < 5; u++) {
        for(int v = 0; v < 5; v++) {
            assert(ct4.is_ancestor(u, v) == (ct4.lca(u, v) == u));
        }
    }
}

// 25. query(l, r) のテスト
void test_25() {
    vector<int> a1 = {42};
    CartesianTree<int> ct1(a1); ct1.build_lca();
    assert(ct1.query(0, 1) == 42);

    vector<int> a2 = {1, 2, 3, 4, 5};
    CartesianTree<int> ct2(a2); ct2.build_lca();
    assert(ct2.query(0, 5) == 1); assert(ct2.query(1, 5) == 2);
    assert(ct2.query(2, 4) == 3); assert(ct2.query(4, 5) == 5);

    vector<int> a3 = {5, 4, 3, 2, 1};
    CartesianTree<int> ct3(a3); ct3.build_lca();
    assert(ct3.query(0, 5) == 1); assert(ct3.query(0, 4) == 2);
    assert(ct3.query(1, 3) == 3);

    CartesianTree<int, greater<int>> ct_max(a2); ct_max.build_lca();
    assert(ct_max.query(0, 5) == 5); assert(ct_max.query(0, 4) == 4);
    assert(ct_max.query(1, 3) == 3);

    vector<int> a5 = {1, 3, 4, 2, 6};
    CartesianTree<int> ct5(a5); ct5.build_lca();
    assert(ct5.query(0, 5) == 1); assert(ct5.query(1, 4) == 2);
    assert(ct5.query(1, 3) == 3); assert(ct5.query(4, 5) == 6);

    vector<int> a6 = {5, 5, 5, 5};
    CartesianTree<int> ct6(a6); ct6.build_lca();
    assert(ct6.query(0, 4) == 5); assert(ct6.query(1, 3) == 5);

    for(int l = 0; l < 5; l++) {
        for(int r = l + 1; r <= 5; r++) {
            assert(ct5.query(l, r) == ct5.value(ct5.query_index(l, r)));
        }
    }
}

// 20. 不正入力の扱い (手動確認のみ。C++ の assert はデフォルトでプログラムを終了させるため、
// ここでは自動テストに組み込まずコメントアウトしておく)
/*
void test_20() {
    vector<int> a = {1, 2, 3};
    CartesianTree<int> ct(a);
    ct.query_index(2, 2); // Should trigger assert
}
*/

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
    test_17();
    test_18_wrapper();
    test_19();
    test_21();
    test_22();
    test_23();
    test_24();
    test_25();
    
    cout << "All tests passed successfully!" << endl;
    return 0;
}
