# MergeSortTree

要素の配列から構築し、部分的な区間 $[l, r)$ 内に含まれる特定の値の範囲に応じた、要素の「個数（count）」や「合計値（sum）」を求める静的データ構造です。
`to_left` や `prefix_sum` などの配列を各ノードに持ち、Fractional Cascading と同様のテクニックを用いて多くのクエリを $O(\log N)$ で高速に処理します。

## 使用例

```cpp
vector<int> init = {3, 1, 4, 1, 5, 9, 2};
MergeSortTree<int> mst(init);
int cnt = mst.count(0, 5, 2, 5); // [0, 5) の中で 2 <= key < 5 を満たす個数 (3, 4) -> 2
long long sm = mst.sum_greater(0, 5, 3); // [0, 5) の中で key > 3 (4, 5) の和 -> 9
```

## コンストラクタ

```cpp
template <class Key>
class MergeSortTree;

MergeSortTree()
explicit MergeSortTree(const std::vector<Key>& init)
```

- 初期配列 `init` を与えてマージソート木を構築します。
- `Key` 型は加算演算 `+` および減算演算 `-`、各種比較演算（`<`, `<=`, `>`, `>=`, `==`）をサポートしている必要があります。

**計算量**

- $O(N \log N)$ （時間および空間）

## size

```cpp
int size() const
```

格納されている要素数 $N$ を返します。

**計算量**

- $O(1)$

## count 系クエリ

区間 $[l, r)$ の中で、条件を満たす要素の **個数** を返します。

```cpp
// xl <= key < xr
int count(int l, int r, const Key& xl, const Key& xr) const

// key < x
int count_less(int l, int r, const Key& x) const

// key <= x
int count_less_equal(int l, int r, const Key& x) const

// key == x
int count_equal(int l, int r, const Key& x) const

// key > x
int count_greater(int l, int r, const Key& x) const

// key >= x
int count_greater_equal(int l, int r, const Key& x) const
```

**計算量**

- すべて $O(\log N)$
- 各ノード内部での二分探索はルートノードでのみ行われ、子のノードへは `to_left` を用いて $O(1)$ で状態を伝播させるため非常に高速です。

## sum 系クエリ

区間 $[l, r)$ の中で、条件を満たす要素の **和（合計）** を返します。

```cpp
// xl <= key < xr
Key sum(int l, int r, const Key& xl, const Key& xr) const

// key < x
Key sum_less(int l, int r, const Key& x) const

// key <= x
Key sum_less_equal(int l, int r, const Key& x) const

// key == x
Key sum_equal(int l, int r, const Key& x) const

// key > x
Key sum_greater(int l, int r, const Key& x) const

// key >= x
Key sum_greater_equal(int l, int r, const Key& x) const

// [l, r) のすべての和
Key sum_all(int l, int r) const
```

**計算量**

- すべて $O(\log N)$
- `sum_all` に関しては該当するセグメント木の区間が全てカバーされている場合の和を足し合わせます。
