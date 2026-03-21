# Mo (Mo's algorithm)

区間クエリをオフラインで一括処理し、$O(N \sqrt{Q})$ で状態の集約を行う Mo's Algorithm の実装です。
ソート順には定数倍が優秀なスネーク順（右端の昇順・降順をブロックの偶奇で反転する手法）を採用しています。

## 使用例

```cpp
#include "iramina/Mo/Mo.hpp"

// 状態を管理する構造体
struct State {
    using answer_type = int;
    vector<int> a;
    vector<int> cnt;
    int current_distinct;

    State(const vector<int>& a) : a(a), cnt(100005, 0), current_distinct(0) {}

    // 区間が左方向に広がる際呼ばれる
    void AddLeft(int index) {
        if (cnt[a[index]] == 0) current_distinct++;
        cnt[a[index]]++;
    }
    // 区間が右方向に広がる際呼ばれる
    void AddRight(int index) {
        if (cnt[a[index]] == 0) current_distinct++;
        cnt[a[index]]++;
    }
    // 区間が左方向から狭まる際呼ばれる
    void RemoveLeft(int index) {
        cnt[a[index]]--;
        if (cnt[a[index]] == 0) current_distinct--;
    }
    // 区間が右方向から狭まる際呼ばれる
    void RemoveRight(int index) {
        cnt[a[index]]--;
        if (cnt[a[index]] == 0) current_distinct--;
    }

    // 現在の区間の答えを取得する
    answer_type GetAnswer() const {
        return current_distinct;
    }
};

int main() {
    int n = 10;
    iramina::Mo<State> mo(n);

    // クエリの追加 [l, r)
    mo.AddQuery(0, 5);
    mo.AddQuery(3, 8);

    // 要素の配列
    vector<int> a = {1, 2, 1, 3, 2, 4, 1, 5, 2, 3};

    // Stateのインスタンスを作成して実行
    State state(a);
    vector<int> ans(2); // 追加したクエリ数以上のサイズを確保
    mo.Run(state, ans);

    // ans[0] に [0, 5) の種類数、ans[1] に [3, 8) の種類数が入る
}
```

## コンストラクタ

```cpp
Mo(int n)
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

- 追加されたクエリをオフラインで適切な順序（スネーク順）にソートし、`state` に対して `AddLeft`, `AddRight`, `RemoveLeft`, `RemoveRight` を呼び出しながら状態を更新します。
- `ans.size()` はあらかじめ追加したクエリ数以上確保されている必要があります。`ans[i]` には $i$ 番目に追加したクエリの答えが書き込まれます。

**`State` に要求される仕様**

テンプレート引数に渡す `State` 構造体（またはクラス）は、以下のメンバーとメソッドを持つ必要があります。
```cpp
struct State {
    using answer_type = 戻り値の型;
    void AddLeft(int index);
    void AddRight(int index);
    void RemoveLeft(int index);
    void RemoveRight(int index);
    answer_type GetAnswer() const;
};
```

**計算量**

- 状態の更新メソッドの呼び出し回数は $O(N \sqrt{Q})$ 回になります。したがって、状態更新1回あたりの計算量を $O(T)$ とすると、全体の時間計算量は $O(Q \log Q + T \cdot N \sqrt{Q})$ となります。
