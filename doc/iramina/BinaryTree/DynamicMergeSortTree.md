# DynamicMergeSortTree

点の追加・削除、および区間のカウントや重み和の計算をサポートする動的な Merge Sort Tree (2次元領域クエリ) データ構造です。
内部的にはセグメント木と非平衡二分探索木 `TreapSum` を組み合わせて構成されています。

## 使用例

```cpp
vector<Elem> init = {{10, 100}, {20, 200}, {15, 150}};
DynamicMergeSortTree<int, int> dmst(init);
int count = dmst.count(0, 3, 10, 20); // [0, 3)で10<=x<20の要素数 -> 2
dmst.set(1, 25, 250);                 // {20, 200} を {25, 250} に更新
```

## コンストラクタ

```cpp
DynamicMergeSortTree<Key, Weight> dmst()
DynamicMergeSortTree<Key, Weight> dmst(const vector<Elem>& init)
```

- 要素を持たない、または `init` で初期化された DynamicMergeSortTree を作成します。
- `init` は `Key key`, `Weight weight` を持つ構造体 `Elem` の配列です。
- クエリの区間の最大値は初期化時の配列の長さに固定されており、要素の変更(キーと重みの変更)は動的に行えます。

**制約**

- $0 \leq \text{init.size()} \leq 10^8$
- 内部的に `TreapSum` を各ノードに持つため空間計算量に注意

**計算量**

- $O(n \log^2 n)$ (初期化)

## get / set

```cpp
Elem dmst.get(int p)
void dmst.set(int p, const Elem& e)
void dmst.set(int p, const Key& key, const Weight& weight)
```

位置 $p$ の要素を取得、または新しい要素に変更します。変更処理は元の要素を削除してから追加します。

**制約**

- $0 \leq p < n$

**計算量**

- $O(\log^2 n)$ (set)
- $O(1)$ (get)

## count

```cpp
int dmst.count(int l, int r, const Key& xl, const Key& xr)
```

位置 $[l, r)$ 内にあり、かつキーが $[xl, xr)$ に収まる要素の数を計算します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log^2 n)$

## sum

```cpp
Weight dmst.sum(int l, int r, const Key& xl, const Key& xr)
```

位置 $[l, r)$ 内にあり、かつキーが $[xl, xr)$ に収まる要素の重みの合計を計算します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log^2 n)$