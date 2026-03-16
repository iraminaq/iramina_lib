# FunctionalGraphToolkit

FunctionalGraphToolkit は、FunctionalGraph および FunctionalGraphDoubling を組み合わせ、パスが 10進巨大整数（文字列指定）でも計算可能な万能のジャンプラッパークラスです。

## 使用例

```cpp
vector<int> to = {1, 2, 0};
vector<long long> weight = {10, 20, 30};
FunctionalGraphToolkit<> tk(to, weight);

// 通常のジャンプ
auto res_small = tk.jump(0, 100);

// 文字列で指定する巨大ステップのジャンプ
auto res_huge = tk.jump_decimal(0, "1000000000000000000"); 
```

## コンストラクタ

```cpp
template<class Monoid = FunctionalGraphSumMonoid<long long>>
FunctionalGraphToolkit()
FunctionalGraphToolkit(const std::vector<int>& to, const std::vector<T>& one_step_value)
```

- 文字列でのジャンプも可能なため、内部のダブリングは $2 \cdot 10^{18}$ までのテーブル固定で初期化されます。
- `to` が FunctionalGraph の構造をなし、`one_step_value` が各パスの重みを持ちます。

**制約**

- $0 \leq to[v] < N$

**計算量**

- $O(N \log \max(10^{18}))$（固定のダブリング構築に比例）

## jump / jump_decimal

```cpp
Result jump(int v, long long k) const
Result jump_decimal(int v, std::string_view k) const
```

頂点 $v$ から $k$ 回（あるいは文字列 $k$ によって指定される回数）遷移したとき、$v$ の行先とモノイド積を `Result { int v, T prod }` で返します。

**計算量**

- $O(\log k)$
- 文字列の場合、$O(L + \text{文字列長} + \log k)$ のように、ダブリング長と余剰へのべき乗演算が内部で評価されます

## kth_next / path_prod 等の簡易クエリ

```cpp
int kth_next(int v, long long k) const
int kth_next_decimal(int v, std::string_view k) const

T path_prod(int v, long long k) const
T path_prod_decimal(int v, std::string_view k) const
```

それぞれ `jump` オブジェクトから $v$ あるいは $prod$ だけを戻り値にするラッパー関数群です。

## jump_to_cycle / first_cycle_vertex

```cpp
Result jump_to_cycle(int v) const
int first_cycle_vertex(int v) const
```

グラフがサイクルに向かう木の形状を成しているとき、$v$ から始まって最初に到達する「パス上のサイクル側の頂点」を返し、あるいはそれに至るまでのモノイドのパス積も返します。
すでにサイクル上にある場合は `0` 移動で判定されます。

**計算量**

- $O(\log N)$

## cycle_length_of / is_on_cycle

```cpp
int cycle_length_of(int v) const
bool is_on_cycle(int v) const
```

頂点が所属しているサイクルの長さを取得したり、$v$ 自身がサイクル内を構成しているのか（木の中継地点ではないか）を O(1) で判定します。
