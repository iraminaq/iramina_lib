# CartesianTree

配列から Cartesian Tree を構築する静的データ構造です。
`build_lca()` を行うことで Euler Tour + RMQ (Sparse Table) を構築し、区間に対する極値や LCA、祖先判定などを高速に処理できます。

## 使用例

```cpp
vector<int> a = {3, 1, 4, 1, 5};
CartesianTree<int> ct(a);
ct.build_lca();
int min_idx = ct.query_index(0, 3); // 区間 [0, 3) の最小値のインデックスは 1
```

## コンストラクタ

```cpp
CartesianTree<T, Comp> ct()
CartesianTree<T, Comp> ct(const vector<T>& a, Comp comp = Comp())
```

- 長さ $n$ の配列 $a$ から Cartesian Tree を構築します。デフォルトコンストラクタで初期化した場合は空の木になります。
- デフォルトの `Comp` は `std::less<T>` であり、min-cartesian tree を構築します。`std::greater<T>` を渡すと max-cartesian tree になります。

**制約**

- $T$ は比較可能な型

**計算量**

- $O(n)$ (構築)

## size

```cpp
int ct.size()
```

配列の要素数 $n$ を返します。

**計算量**

- $O(1)$

## root / parent / left_child / right_child

```cpp
int ct.root()
int ct.parent(int v)
int ct.left_child(int v)
int ct.right_child(int v)
```

それぞれ、木の根の頂点番号、頂点 $v$ の親、左の子、右の子を返します。存在しない場合は `-1` を返します。

**制約**

- $0 \leq v < n$

**計算量**

- $O(1)$

## build_lca

```cpp
void ct.build_lca()
```

LCA やクエリ関数を使うための前処理 (Euler Tour + Sparse Table) を行います。
これを呼ぶことで `query_index` や `lca` 等の関数が使用可能になります。

**計算量**

- $O(n \log n)$

## query_index / query

```cpp
int ct.query_index(int l, int r)
const T& ct.query(int l, int r)
```

区間 $[l, r)$ における極値（`Comp` による最小/最大）のインデックス、およびその要素自体を返します。
同じ値が複数ある場合は最も左のインデックスを返します。

**制約**

- `build_lca()` 実行後のみ使用可能
- $0 \leq l < r \leq n$

**計算量**

- $O(1)$

## lca / depth / is_ancestor

```cpp
int ct.lca(int u, int v)
int ct.depth(int v)
bool ct.is_ancestor(int u, int v)
```

$u, v$ の Lowest Common Ancestor、頂点 $v$ の深さ、および $u$ が $v$ の祖先（$u=v$ を含む）であるか判定します。

**制約**

- `build_lca()` 実行後のみ使用可能
- $0 \leq u, v < n$

**計算量**

- $O(1)$

## subtree_range / subtree_size

```cpp
pair<int, int> ct.subtree_range(int v)
int ct.subtree_size(int v)
```

頂点 $v$ の部分木が対応する元の配列の連続区間 $[l, r)$ と、そのサイズを返します。

**制約**

- $0 \leq v < n$

**計算量**

- $O(1)$