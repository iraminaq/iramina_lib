# MoRollback (Rollback Mo's Algorithm)

要素の追加（`Add`）は容易であるが、削除（`Remove`）に非常に時間がかかる、あるいは不可能なデータ構造（例：Union-Find など）を用いて Mo のアルゴリズムを適用するための実装です。

内部ではクエリをブロックごとに処理し、左側ポインタ $L$ の移動による変更のみを各クエリごとに `Snapshot()` と `Rollback()` で巻き戻すことで、「要素の削除処理」を一切行わずに区間クエリをオフラインで一括処理します。

## 使用例

```cpp
#include "iramina/Mo/MoRollback.hpp"

// 状態を管理する構造体（削除操作を持たない）
struct State {
    using answer_type = long long;
    vector<int> a;
    long long current_sum;
    vector<long long> history;

    State(const vector<int>& a) : a(a), current_sum(0) {}

    // 区間が左方向に広がる際呼ばれる
    void AddLeft(int index) {
        current_sum += a[index];
    }
    // 区間が右方向に広がる際呼ばれる
    void AddRight(int index) {
        current_sum += a[index];
    }
    
    // 現在の状態（主に左ポインタによって変更される前の状態）を保存する
    void Snapshot() {
        history.push_back(current_sum);
    }
    // Snapshot が呼ばれた時点の状態に復元する（左ポインタの変更分を取り消す）
    void Rollback() {
        current_sum = history.back();
        history.pop_back();
    }
    // ブロックが変化した際に、状態を完全に初期状態（空）に戻す
    void Reset() {
        current_sum = 0;
        history.clear();
    }

    // 現在の区間の答えを取得する
    answer_type GetAnswer() const {
        return current_sum;
    }
};

int main() {
    int n = 10;
    iramina::MoRollback<State> mo(n);

    mo.AddQuery(0, 5);
    mo.AddQuery(3, 8);

    vector<int> a = {1, 2, 1, 3, 2, 4, 1, 5, 2, 3};

    State state(a);
    vector<long long> ans(2);
    mo.Run(state, ans);
}
```

## コンストラクタ

```cpp
MoRollback(int n)
```

- 対象となる配列等の長さを $n$ とします。
- 制約：$n \geq 1$

**計算量**

- $O(1)$

## AddQuery

```cpp
void mo.AddQuery(int l, int r)
```

- 半開区間 $[l, r)$ に対するクエリを追加します。
- 制約：$0 \le l \le r \le n$

**計算量**

- $O(1)$

## Run

```cpp
void mo.Run(State& state, std::vector<answer_type>& ans) const
```

- 追加されたクエリをオフラインで適切な順序（ブロックごとの $R$ 昇順）にソートし、`state` に対して `AddLeft`, `AddRight`, `Snapshot`, `Rollback`, `Reset` を適宜呼び出しながら計算します。
- 制約上、右端のポインタ $R$ が単調増加である必要があるため、通常 Mo とは異なりスネーク順のソートは行われません。
- `ans.size()` はあらかじめ追加したクエリ数以上確保されている必要があります。

**`State` に要求される仕様**

テンプレート引数に渡す `State` 構造体（またはクラス）は、以下のメンバーとメソッドを持つ必要があります。
```cpp
struct State {
    using answer_type = 戻り値の型;
    void AddLeft(int index);
    void AddRight(int index);
    void Snapshot();
    void Rollback();
    void Reset();
    answer_type GetAnswer() const;
};
```

**計算量**

- 状態の更新メソッドの呼び出し回数は $O(N \sqrt{Q})$ 回になります。したがって、状態更新1回あたりの計算量を $O(T)$ とすると、全体の時間計算量は $O(Q \log Q + T \cdot N \sqrt{Q})$ となります。
