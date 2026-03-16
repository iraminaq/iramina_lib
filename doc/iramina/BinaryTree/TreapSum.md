# TreapSum

キー (Key) による平衡二分探索木 (Treap) であり、要素の追加・削除や、指定範囲内のキーのカウントおよび重み和のクエリを処理できます。
各ノードは多重集合の要素として振る舞い（同じキーが複数存在可能）、それぞれの重みを管理します。

## 使用例

```cpp
TreapSum<int, long long> ts;
ts.insert(10, 100);
ts.insert(20, 200);
long long sm = ts.sum_less(15); // キーが 15 未満のもの (10) の重みの和 -> 100
```

## コンストラクタ

```cpp
TreapSum<Key, Weight> ts()
```

- 空のTreapSumを作成します。

**計算量**

- $O(1)$

## insert / erase_one

```cpp
void ts.insert(const Key& key, const Weight& weight)
bool ts.erase_one(const Key& key, const Weight& weight)
```

- `insert`: キーと重みのペアを追加します。
- `erase_one`: 指定したキーを持つ要素を1つだけ取り除きます（指定した重みを取り除きます）。同じキーに異なる重みがあり得るため、利用者が正しい重みを渡す必要があります。要素が存在しなかった・削除できなかった場合は `false` を返します。

**計算量**

- $O(\log n)$

## count / sum_key

```cpp
int ts.count(const Key& key)
Weight ts.sum_key(const Key& key)
```

- 指定したキーに完全一致する要素の個数、およびそれらの重みの合計を返します。

**計算量**

- $O(\log n)$

## count_less / count_less_equal / count_range

```cpp
int ts.count_less(const Key& x)
int ts.count_less_equal(const Key& x)
int ts.count_range(const Key& l, const Key& r)
```

それぞれキーが $x$ 未満のもの、$x$ 以下のもの、$[l, r)$ の範囲に収まるものの要素数を返します。

**計算量**

- $O(\log n)$

## sum_less / sum_less_equal / sum_range

```cpp
Weight ts.sum_less(const Key& x)
Weight ts.sum_less_equal(const Key& x)
Weight ts.sum_range(const Key& l, const Key& r)
```

それぞれキーが $x$ 未満のもの、$x$ 以下のもの、$[l, r)$ の範囲に収まるものの重みの合計を返します。

**計算量**

- $O(\log n)$

## kth

```cpp
bool ts.kth(int k, Entry& out)
```

全体の中で小さい方から見て $k$ 番目 (0-indexed) にあたるキーの実体を `out` にコピーします。
見つからなければ `false`、見つかれば `true` を返します。

**制約**

- $0 \leq k < \text{size}$

**計算量**

- $O(\log n)$

## lower_bound / upper_bound

```cpp
bool ts.lower_bound(const Key& x, Entry& out)
bool ts.upper_bound(const Key& x, Entry& out)
```

キーが $x$ 以上（upper_bound の場合は $x$ より大きい）の最小の要素を取得し、`out` にコピーします。

**計算量**

- $O(\log n)$
