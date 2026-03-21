# SimpleImplicitTreap

既存のフル汎用版 `ImplicitTreap` のテンプレート引数を極大まで簡素化し、競技プログラミングのコンテスト中でも記述しやすい形に最適化したラッパー（エイリアス）です。

内部実装は元の `ImplicitTreap` をそのまま使っているため、対応するメソッド（`insert`, `erase`, `prod`, `apply`, `reverse`, `split`, `merge` 等）の仕様や計算量はすべて同一です。詳細なメソッド群については [ImplicitTreap のドキュメント](ImplicitTreap.md) を参照してください。

---

## Traits の要件

`SimpleImplicitTreap` は、1つの構造体 `Traits` を引数に取ります。
`T = S`（要素の型と集約値の型が同一）を大前提としており、**利用したい機能に応じて追加の関数を書くだけ**でSFINAEによる静的ディスパッチによって自動構築されます。

### 基本構成（1. 最低限必要なもの）
遅延評価や、反転にともなう特殊な演算が不要な場合は、この3つだけを満たせば動きます。
```cpp
struct Traits {
    using T = 扱うデータ型;
    static T op(T a, T b) { return ...; } // 左右の子の合成
    static T e() { return ...; }          // 単位元
};
```

### 区間更新・遅延評価を利用する場合（2. Lazyあり）
`use_lazy = true` をフラグとして持たせ、以下の4つを追記します。
**（注：フル版では別々に定義させられていた点作用と区間作用は `apply` コマンド1つに統合されました。長さ `len` が与えられるのでそこで判定します）**
```cpp
struct Traits {
    using T = ...;
    static T op(T a, T b) { ... }
    static T e() { ... }

    // ---------- 以下を追記 ----------
    using F = 作用素の型;
    static constexpr bool use_lazy = true;

    // f を値 x (長さ len) に適用した結果
    static T apply(F f, T x, int len) { return ...; }
    // すでに存在する作用 g に、新しい作用 f を合成する (f(g(x)))
    static F composition(F f, F g) { return ...; }
    // 作用素の恒等写像
    static F id() { return ...; }
};
```

### 反転時に集約値が変化する場合（3. Reverseあり）
`use_reverse = true` をフラグとして持たせ、反転用の関数を追記します。
（※和や最大値など、左右を反転させても集約値から得られる代表値が変わらない演算の場合は、この指定は一切不要です。内部で自動的に `x -> x` が補完されます）
```cpp
struct Traits {
    // ... 基本設定 ...

    // ---------- 以下を追記 ----------
    static constexpr bool use_reverse = true;
    
    // 区間を反転した場合の集約値を返す
    static T reverse_prod(T x) { return ...; }
};
```

---

## 使用例

### Lazy なし版 (例: 区間和)
```cpp
#include "iramina/BinaryTree/SimpleImplicitTreap.hpp"

struct TraitsNoLazy {
    using T = long long;
    static T op(T a, T b) { return a + b; }
    static T e() { return 0; }
};
using Treap = iramina::SimpleImplicitTreap<TraitsNoLazy>;

int main() {
    Treap treap(std::vector<long long>{1, 2, 3, 4, 5});
    long long sm = treap.prod(1, 4); // 9 (2+3+4)
}
```

### Lazy あり版 (例: 区間加算・区間和)
```cpp
#include "iramina/BinaryTree/SimpleImplicitTreap.hpp"

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
using Treap = iramina::SimpleImplicitTreap<TraitsLazy>;

int main() {
    Treap treap(std::vector<long long>{1, 2, 3, 4, 5});
    treap.apply(1, 4, 10);
    long long sm = treap.prod(1, 4); // 39
}
```

### Reverse あり版 (例: 行列積など非可換なもの)
※下記はイメージです
```cpp
#include "iramina/BinaryTree/SimpleImplicitTreap.hpp"

struct TraitsReverse {
    using T = Matrix;
    static constexpr bool use_reverse = true;

    static T op(T a, T b) { return a * b; }
    static T e() { return Matrix::identity(); }
    
    // 行列の積の順序を取り決める
    static T reverse_prod(T x) { return x.transposed_or_swapped(); } 
};
using Treap = iramina::SimpleImplicitTreap<TraitsReverse>;
```
