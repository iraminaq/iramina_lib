# WeightedMergeSortTree

区間内にあるキーの個数や、その重みの合計（およびそれらの閾値付きクエリ）を高速に処理するデータ構造です。
`Merge Sort Tree` 上に各頂点でのキーおよび対応する重みの累積和を保持します。

## 使用例

```cpp
vector<Elem> init = {{10, 100}, {20, 200}, {30, 300}};
WeightedMergeSortTree<int, long long> mst(init);
long long sm = mst.sum_less(0, 3, 25); // 区間 [0, 3) でキーが 25 未満のもの (10, 20) の重みの和 -> 300
```

## コンストラクタ

```cpp
WeightedMergeSortTree<Key, Weight> ms()
WeightedMergeSortTree<Key, Weight> ms(const vector<Elem>& init)
```

- 初期化子として `Elem`（キーと重みを持つ）の配列を渡します。完全二分木を構築して `merge sort tree` と同様のデータを格納します。

**制約**

- `init.size()` の上限は構築時間に依存します（一般的には $10^5 \sim 10^6$ 程度）。空間計算量 $O(n \log n)$。

**計算量**

- $O(n \log n)$ (構築)

## size

```cpp
int ms.size()
```

要素数 $n$ を返します。

**計算量**

- $O(1)$

## count / sum

```cpp
int ms.count(int l, int r, const Key& xl, const Key& xr)
Weight ms.sum(int l, int r, const Key& xl, const Key& xr)
```

区間 $[l, r)$ 内の要素のうち、キーが $xl \leq \text{key} < xr$ を満たすものの要素数、および重みの和を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$ (二分探索は各ノードで `to_left` を使って Fractional Cascading / 状態分岐で枝刈りされているため)

## count_less / sum_less

```cpp
int ms.count_less(int l, int r, const Key& x)
Weight ms.sum_less(int l, int r, const Key& x)
```

区間 $[l, r)$ において、キーが $x$ 未満である要素数および重みの和を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

## count_less_equal / count_equal / count_greater_equal / count_greater

```cpp
int ms.count_less_equal(int l, int r, const Key& x)
int ms.count_equal(int l, int r, const Key& x)
int ms.count_greater_equal(int l, int r, const Key& x)
int ms.count_greater(int l, int r, const Key& x)
```

区間 $[l, r)$ において、それぞれキーが $x$ 以下、$x$ と等しい、$x$ 以上、$x$ より大きい要素数を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

## sum_less_equal / sum_equal

```cpp
Weight ms.sum_less_equal(int l, int r, const Key& x)
Weight ms.sum_equal(int l, int r, const Key& x)
```

区間 $[l, r)$ において、それぞれキーが $x$ 以下、$x$ と等しい要素の重みの和を返します。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$
