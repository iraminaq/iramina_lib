# FunctionalGraphDoubling

functional graph 上でのジャンプ・モノイド集約をダブリング（doubling）によって高速に行うテンプレクラスです。

## 使用例

```cpp
vector<int> to = {1, 2, 0};
vector<long long> weight = {10, 20, 30};
FunctionalGraphDoubling<> dbl(to, weight, 100);
auto res = dbl.jump(0, 2); // 頂点 0 から 2ステップ
// res.v == 2, res.prod == 30 (10 + 20)
```

## コンストラクタ

```cpp
template<class Monoid = FunctionalGraphSumMonoid<long long>>
FunctionalGraphDoubling()
FunctionalGraphDoubling(const std::vector<int>& to, const std::vector<T>& one_step_value, long long max_k)
```

- 長さ $N$ の配列 `to` から機能付きグラフを構築します。
- `to[v]` は頂点 $v$ から1歩進んだ先の頂点を表します。
- `one_step_value[v]` は頂点 $v \to to[v]$ の1ステップに対応するモノイド元です。
- `max_k` は想定されるジャンプの最大ステップ数です（$k > max\_k$ に対するダブリングは行われません）。

### テンプレート引数（モノイド `Monoid`）の要件

`Monoid` には値の型と、二項演算、単位元を定義した構造体（クラス）をテンプレートで渡します。
構造体は以下の要素を持っている必要があります。

```cpp
struct MyMonoid {
    using value_type = ...;
    static value_type op(const value_type& a, const value_type& b) {
        // 先に a のパスを通り、その後に b のパスを通る場合の集約処理
    }
    static value_type e() {
        // 例: 合計なら 0、積なら 1、アフィン合成なら恒等写像に該当する元
        return ...;
    }
};
```

特にジャンプやダブリングの都合上、演算方向 `op(a, b)` は常に **「先に適応させたい区間の値 (`a`)」を第一引数として、「後から通過する経路 (`b`)」を重ねる** という向きの処理を記述するようにしてください。（例：アフィン変換 $f(x) = cx+d$, $g(x) = ax+b$ を渡す場合、$a$ 側に先に通る $f$ の情報を設定し、演算結果が $g(f(x))$ となるように合成します）

**制約**

- $0 \leq to[v] < N$
- `max_k >= 0`

**計算量**

- $O(N \log (\max(1, \max\_k)))$

## init

```cpp
void init(const std::vector<int>& to, const std::vector<T>& one_step_value, long long max_k)
```

構築を行います。`FunctionalGraphDoubling` オブジェクトの再利用時に用います。

**計算量**

- $O(N \log (\max(1, \max\_k)))$

## jump

```cpp
Result jump(int v, long long k) const
```

頂点 $v$ からちょうど $k$ 回遷移した先である頂点 $u$、およびその間のモノイド積の合計を `Result` 型で返します。

- 演算順序は、「先に適用された辺」に「後の辺の元」を右からかけるモノイド要件（$A \cdot B$ のように順序依存の結合）に従います。

**計算量**

- $O(\log k)$

## kth_next / path_prod

```cpp
int kth_next(int v, long long k) const
T path_prod(int v, long long k) const
```

`jump` によって計算される、行き先の頂点番号のみ、および対応するモノイド積のみを返すラッパーです。

**計算量**

- $O(\log k)$
