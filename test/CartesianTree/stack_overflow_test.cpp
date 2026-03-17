#include <iostream>
#include <vector>
#include <chrono>
#include "iramina/BinaryTree/CartesianTree.hpp"

using namespace std;

int main() {
    int n = 300000;
    vector<int> a(n);
    for(int i = 0; i < n; i++) {
        a[i] = i; // strictly increasing -> Completely unbalanced right
    }
    
    try {
        CartesianTree<int> ct(a);
        ct.build_lca();
        cout << "Strictly increasing OK" << endl;
    } catch(...) {
        cout << "Strictly increasing Failed" << endl;
    }

    for(int i = 0; i < n; i++) {
        a[i] = n - i; // strictly decreasing -> Completely unbalanced left
    }
    
    try {
        CartesianTree<int> ct2(a);
        ct2.build_lca();
        cout << "Strictly decreasing OK" << endl;
    } catch(...) {
        cout << "Strictly decreasing Failed" << endl;
    }

    return 0;
}
