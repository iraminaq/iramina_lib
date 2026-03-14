# ImplicitTreap

配列の任意位置への要素の挿入・削除や、区間の反転、区間に対する作用 (Lazy propagation) などを高速にサポートする遅延評価つき暗黙的Treap (Implicit Treap) です。
デフォルトでは `long long` 型で区間加算・区間和を管理しますが、`op`, `e`, `mapping`, `composition`, `id` などの関数ポインタを指定することで任意のモノイドや作用素に変更できます (セグメント木 `lazy_segtree` に似たAPI構造)。

## コンストラクタ

```cpp
ImplicitTreap<T, S, F, op, e, lift, mapping_point, mapping, composition, id, reverse_prod> treap()
ImplicitTreap(...) treap(const vector<T>& v)
```

- 初期状態として配列 $v$ を構築します。初期化の際、線形時間 $O(n)$ で Implicit Treap の形を作ります。
- カスタムなモノイドと作用素を定義する場合は、テンプレート引数（またはデフォルト関数に対するオーバーロード）で動作を変更します。

**制約**

- $0 \leq n \leq 10^8$
- 内部的に `NodePool` を使用しており、破棄されたノードのメモリ領域は再利用されます。

**計算量**

- $O(n)$ (構築)

## size / empty / clear

```cpp
int treap.size()
bool treap.empty()
void treap.clear()
```

要素数、空かどうかの判定、および全要素の削除を行います。`clear()` を呼ぶとメモリプールにノードが返却されます。

**計算量**

- `size`, `empty`: $O(1)$
- `clear`: $O(n)$

## insert / erase

```cpp
void treap.insert(int k, const T& x)
void treap.erase(int k)
```

位置 $k$ に要素 $x$ を挿入します。また、位置 $k$ の要素を削除します。

**制約**

- `insert`: $0 \leq k \leq n$
- `erase`: $0 \leq k < n$

**計算量**

- $O(\log n)$

## get / set

```cpp
T treap.get(int k)
void treap.set(int k, const T& x)
```

位置 $k$ の要素を取得、または $x$ で上書きします。

**制約**

- $0 \leq k < n$

**計算量**

- $O(\log n)$

## apply / prod / all_prod

```cpp
void treap.apply(int l, int r, const F& f)
S treap.prod(int l, int r)
S treap.all_prod()
```

区間 $[l, r)$ に対して作用 $f$ を適用し、区間 $[l, r)$ の部分和を取得します。
また、全体の部分和を $O(1)$ で取得可能です。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- `apply`, `prod`: $O(\log n)$
- `all_prod`: $O(1)$

## reverse

```cpp
void treap.reverse(int l, int r)
```

区間 $[l, r)$ の要素の順序を反転させます。（内部で Reverse フラグを遅延評価します）

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

## dump

```cpp
vector<T> treap.dump()
```

Treap の持つ全列の要素を `std::vector<T>` に変換して返します。

**計算量**

- $O(n)$

## split / merge

```cpp
pair<ImplicitTreap, ImplicitTreap> treap.split(int k)
void treap.merge(ImplicitTreap& other)
```

- `split(int k)`: 現在の Treap を位置 $k$ で2つに分割し、それぞれを新しい `ImplicitTreap` として返します。分割後、元の Treap は空になります。
- `merge(ImplicitTreap& other)`: 別の Treap `other` を現在の Treap の右側に結合します。結合後、`other` は空になります。

**制約**

- `split`: $0 \leq k \leq n$

**計算量**

- $O(\log n)$

## rotate / swap_ranges

```cpp
void treap.rotate(int l, int m, int r)
void treap.swap_ranges(int l1, int r1, int l2, int r2)
```

- `rotate`: 区間 $[l, m)$ と区間 $[m, r)$ を入れ替えます。
- `swap_ranges`: 区間 $[l1, r1)$ と区間 $[l2, r2)$ （互いに重ならないこと）の位置を入れ替えます。

**制約**

- `rotate`: $0 \leq l \leq m \leq r \leq n$
- `swap_ranges`: 各区間は適正で互いに素であること。

**計算量**

- $O(\log n)$

## max_right / min_left

```cpp
template <class Pred> int treap.max_right(int l, Pred pred)
template <class Pred> int treap.min_left(int r, Pred pred)
```

セグメント木の二分探索 `max_right`, `min_left` と同じ働きをします。区間の端から条件を満たす最大の範囲を求めます。

**制約**

- `max_right`: $0 \leq l \leq n$
- `min_left`: $0 \leq r \leq n$
- `pred(e())` は `true` を返すこと。

**計算量**

- $O(\log n)$
