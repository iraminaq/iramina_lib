# ユーティリティ (ira::util)

便利な数学関数やユーティリティ関数等の集まりです。
各関数は名前空間 `ira` の下で使用できます。

## pow

```cpp
__int128 ira::pow(long long x, long long n)
```

$x^n$ を計算します。オーバーフロー対策として `__int128` 型で結果を返します。

**計算量**

- $O(\log n)$

## mod

```cpp
long long ira::mod(long long x, long long m)
```

C++標準の `%` 演算子と異なり、負の数に対しても正しく数学的な剰余（$0 \le x \pmod m < m$）を返します。

**計算量**

- $O(1)$

## powMod

```cpp
__int128 ira::powMod(long long x, long long n, long long mod)
```

$x^n \pmod{mod}$ を計算します。途中の乗算は `__int128` で行われるため、法 `mod` が $10^{18}$ 程度でもオーバーフローしません。

**計算量**

- $O(\log n)$

## floorLog2 / ceilLog2

```cpp
int ira::floorLog2(int x)
int ira::floorLog2(long long x)
int ira::ceilLog2(int x)
int ira::ceilLog2(long long x)
```

$\lfloor \log_2(x) 
floor$ および $\lceil \log_2(x) 
ceil$ を計算します。
組み込み関数 `__builtin_clz`, `__builtin_clzll` 等を用いて高速に計算されます。

**制約**

- $x > 0$ であること

**計算量**

- $O(1)$

## digits

```cpp
int ira::digits(long long x)
```

整数 $x$ を10進数で表したときの桁数を返します。$x = 0$ の場合は 1 を返します。

**計算量**

- $O(\log_{10} x)$

## floor_div / ceil_div

```cpp
long long ira::floor_div(long long a, long long b)
long long ira::ceil_div(long long a, long long b)
```

それぞれ $\lfloor a / b 
floor$ (負の無限大方向への丸め)、$\lceil a / b 
ceil$ (正の無限大方向への丸め) を計算します。
$a, b$ の符号が異なる場合でも正しく丸められます。

**制約**

- $b 
eq 0$

**計算量**

- $O(1)$

## C2

```cpp
long long ira::C2(long long n)
```

組み合わせの数 $\binom{n}{2} = \frac{n(n-1)}{2}$ を計算します。$n < 2$ の場合は 0 を返します。

**計算量**

- $O(1)$

## ssize

```cpp
template <class C> long long ira::ssize(const C& c)
```

コンテナ `c` のサイズ（`c.size()`）を符号付き整数（`long long`）にキャストして返します。
`size_t` のまま `- 1` などの演算をして起こるアンダーフローバグを防ぐのに役立ちます。

**計算量**

- $O(1)$

## chmin / chmax

```cpp
template<typename T> bool ira::chmin(T& a, T b)
template<typename T> bool ira::chmax(T& a, T b)
```

それぞれ、`a > b`、`a < b` である場合、`a` を `b` に更新し `true` を返します。更新されなかった場合は `false` を返します。
DP（動的計画法）などで最大値・最小値を更新する際にコードを簡潔に書けます。

**計算量**

- $O(1)$
