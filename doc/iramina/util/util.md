# ユーティリティ (util)

便利な数学関数やユーティリティ関数等の集まりです。

## powI64

```cpp
__int128 powI64(long long x, long long n)
```

$x^n$ を計算します。オーバーフロー対策として `__int128` 型で結果を返します。

**計算量**

- $O(\log n)$

## modNormalize

```cpp
long long modNormalize(long long x, long long m)
```

C++標準の `%` 演算子と異なり、負の数に対しても正しく数学的な剰余（$0 \le x \pmod m < m$）を返します。

**計算量**

- $O(1)$

## powMod

```cpp
__int128 powMod(long long x, long long n, long long mod)
```

$x^n \pmod{mod}$ を計算します。途中の乗算は `__int128` で行われるため、法 `mod` が $10^{18}$ 程度でもオーバーフローしません。

**計算量**

- $O(\log n)$

## floorLog2 / ceilLog2

```cpp
int floorLog2(int x)
int floorLog2(long long x)
int ceilLog2(int x)
int ceilLog2(long long x)
```

$\lfloor \log_2(x) \rfloor$ および $\lceil \log_2(x) \rceil$ を計算します。
組み込み関数 `__builtin_clz`, `__builtin_clzll` 等を用いて高速に計算されます。

**制約**

- $x > 0$ であること

**計算量**

- $O(1)$

## digits

```cpp
int digits(long long x)
```

整数 $x$ を10進数で表したときの桁数を返します。$x = 0$ の場合は 1 を返します。

**計算量**

- $O(\log_{10} x)$

## floorDiv / ceilDiv

```cpp
long long floorDiv(long long a, long long b)
long long ceilDiv(long long a, long long b)
```

それぞれ $\lfloor a / b \rfloor$ (負の無限大方向への丸め)、$\lceil a / b \rceil$ (正の無限大方向への丸め) を計算します。
$a, b$ の符号が異なる場合でも正しく丸められます。

**制約**

- $b \neq 0$

**計算量**

- $O(1)$

## C2

```cpp
long long C2(long long n)
```

組み合わせの数 $\binom{n}{2} = \frac{n(n-1)}{2}$ を計算します。$n < 2$ の場合は 0 を返します。

**計算量**

- $O(1)$

## chMin / chMax

```cpp
template<typename T> bool chMin(T& a, const T& b)
template<typename T> bool chMax(T& a, const T& b)
```

それぞれ、`a > b`、`a < b` である場合、`a` を `b` に更新し `true` を返します。更新されなかった場合は `false` を返します。
DP（動的計画法）などで最大値・最小値を更新する際にコードを簡潔に書けます。

**計算量**

- $O(1)$

## chAbs

```cpp
template<typename T> bool chAbs(T& a)
```

`a < 0` の場合、`a` を `-a`（絶対値）に更新し `true` を返します。更新されなかった場合は `false` を返します。

**計算量**

- $O(1)$

## inRange

```cpp
template<typename T> bool inRange(const T& a, const T& l, const T& r)
```

値 `a` が半開区間 `[l, r)` に含まれるか（`l <= a && a < r`）を判定し、`bool` を返します。

**計算量**

- $O(1)$

## pair の四則演算 (operator+, -, *, /)

```cpp
template<class T, class S> std::pair<T, S> operator+(const std::pair<T,S>& a, const std::pair<T,S>& b)
template<class T, class S> std::pair<T, S> operator-(const std::pair<T,S>& a, const std::pair<T,S>& b)
template<class T, class S> std::pair<T, S> operator*(const std::pair<T,S>& a, const std::pair<T,S>& b)
template<class T, class S> std::pair<T, S> operator/(const std::pair<T,S>& a, const std::pair<T,S>& b)
```

`std::pair` に対して要素ごとの四則演算をオーバーロードしています。
これにより、`p3 = p1 + p2;` のような直感的な記述が可能になります。

**計算量**

- $O(1)$
