# FunctionalGraph

各頂点から出る辺がちょうど1本の有向グラフ（functional graph）を扱う静的解析構造です。
`to` 配列を元に、どの頂点がどのサイクルに属するか、サイクルまでの距離などを $O(N)$ の前計算で構築します。

## 使用例

```cpp
vector<int> to = {1, 2, 0, 4, 5, 4};
FunctionalGraph fg(to);
bool on_cycle = fg.is_on_cycle(1); // true
int dist = fg.dist_to_cycle_of(3); // 1 (3 -> 4)
```

## コンストラクタ

```cpp
FunctionalGraph()
explicit FunctionalGraph(const std::vector<int>& to)
```

- 長さ $N$ の配列 `to` から functional graph を構築します。
- `to[v]` は頂点 $v$ から1歩進んだ先の頂点を表します。

**制約**

- $0 \leq to[v] < N$

**計算量**

- $O(N)$

## init

```cpp
void init(const std::vector<int>& to)
```

構築を行います。FunctionalGraph オブジェクトの再利用時に用います。

**計算量**

- $O(N)$

## メンバ変数

以下の配列および変数が構築後に利用可能です：

- `int n`: 頂点数
- `std::vector<int> to`: 入力された順方向の遷移先
- `std::vector<std::vector<int>> radj`: 逆方向の隣接リスト
- `std::vector<int> indeg`: 入次数
- `std::vector<int> cycle_id`: 頂点が最終的に到達するサイクルの ID
- `std::vector<int> cycle_pos`: 頂点がサイクル上のどのインデックスにいるか（木部分の頂点は `-1`）
- `std::vector<int> dist_to_cycle`: 頂点から最初に入るサイクルまでの距離（サイクル上の頂点は `0`）
- `std::vector<int> root_cycle_vertex`: 進み続けたとき最初に到達するサイクル頂点
- `std::vector<std::vector<int>> cycles`: 各サイクルの頂点列（`to` を辿る順序で格納）

## ヘルパー関数

```cpp
bool is_on_cycle(int v) const;
int dist_to_cycle_of(int v) const;
int cycle_length_of(int v) const;
int root_cycle_vertex_of(int v) const;

int cycle_count() const;
int cycle_size(int cid) const;
```

それぞれ自身のプロパティや、全体のサイクルの数、特定のサイクルの長さを定数時間で取得するためのラッパーです。

**計算量**

- $O(1)$
