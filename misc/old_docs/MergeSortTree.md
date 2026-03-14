# ライブラリ概要

#### 説明

`MergeSortTree` は、配列 index の区間と値 `key` の区間を同時に扱う静的データ構造です。

各要素は `(key, weight)` を持ち、次のクエリをサポートします。

* 区間 `[l, r)` において
* `xl <= key < xr` を満たす要素

について

* 個数
* `weight` の総和

を取得できます。

内部構造は

* **Segment Tree**
* 各ノードに **key 昇順配列 + weight 累積和**

で構成されています。

---

#### 使い方

```cpp
using MST = MergeSortTree<int,long long>;

vector<MST::Elem> init = {
    {5,10},
    {2,7},
    {8,1},
};

MST seg(init);

seg.count(0,3,0,10); // 個数
seg.sum(0,3,0,10);   // weight総和
```

---

# ライブラリ詳細

#### `Elem`

* **引数 / 返り値**

```cpp
struct Elem {
    Key key;
    Weight weight;
};
```

* **説明**

配列要素を表す構造体。

* `key`

  * 値域クエリの判定に使用
* `weight`

  * `sum` クエリで加算される値

---

#### `MergeSortTree(const vector<Elem>& init)`

* **引数 / 返り値**

```cpp
MergeSortTree(const vector<Elem>& init)
```

* **説明**

初期配列 `init` から構築する。

* `init[i]` が配列位置 `i` の要素となる。

* **制約**

なし

* **計算量 / 定数倍**

```
構築: O(N log N)
```

---

#### `size`

* **引数 / 返り値**

```cpp
int size() const
```

* **説明**

配列長 `N` を返す。

* **制約**

なし

* **計算量 / 定数倍**

```
O(1)
```

---

# 対応クエリ

#### 区間個数クエリ

* **説明**

```cpp
count(l,r,xl,xr)
```

次を同時に満たす要素数を返す。

```
l ≤ i < r
xl ≤ key < xr
```

* **計算量**

```
O(log^2 N)
```

* **コード**

```cpp
int ans = seg.count(l,r,xl,xr);
```

---

#### 区間和クエリ

* **説明**

```cpp
sum(l,r,xl,xr)
```

次を同時に満たす要素の `weight` 総和を返す。

```
l ≤ i < r
xl ≤ key < xr
```

* **計算量**

```
O(log^2 N)
```

* **コード**

```cpp
long long ans = seg.sum(l,r,xl,xr);
```

---

# 応用

#### Merge Sort Tree

* **説明**

配列 index に対して segment tree を構築し、
各ノードにその区間に含まれる要素の **key 昇順配列** を持つデータ構造。

クエリでは

1. 区間 `[l,r)` を `O(log N)` 個のノードに分解
2. 各ノードで `binary search` により `[xl,xr)` を数える

ことで結果を得る。

---

#### 2次元範囲クエリ

* **説明**

index を x 座標、`key` を y 座標とみなすことで、

```
[l,r) × [xl,xr)
```

の矩形領域に含まれる要素の

* 個数
* weight総和

を取得できる。

これは **静的 2D range tree** とみなすことができる。

---

# 注意点

#### 範囲は半開区間

すべての範囲は半開区間。

```
index : [l,r)
value : [xl,xr)
```

---

#### 更新はできない

この `MergeSortTree` は **静的構造**であり、更新はサポートしない。

更新が必要な場合は

```
DynamicMergeSortTree
```

を使用する。

---

#### key の重複

同じ `key` の要素は複数存在してよい。

内部ではそのまま配列として保持される。

---

#### 空区間

```
l == r
```

の場合

```
count = 0
sum = 0
```

となる。

