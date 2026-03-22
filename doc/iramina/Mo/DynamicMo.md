# DynamicMo (Mo's algorithm with Updates)

通常の区間に対するクエリ（区間クエリ）に加えて、時間 $T$ に依存する要素の変更（点更新クエリ）をオフラインで処理できるように 3 次元に拡張された Mo のアルゴリズムの実装です。

ブロックサイズを $N$ だけではなくクエリ回数 $Q$ や更新の回数 $U$ も考慮した最適値 $B \approx (N^2 U / Q)^{1/3}$ で自動的に判定し、ソートを行います。

## 使用例

```cpp
#include "iramina/Mo/DynamicMo.hpp"

// 時間 T による配列の更新を管理するための情報
struct UpdateInfo {
    int idx;
    int old_val;
    int new_val;
};

// 状態を管理する構造体
struct State {
    using answer_type = long long;
    vector<int> a; // 更新によって書き換わる可能性のある状態のコピー
    const vector<UpdateInfo>* updates;
    long long current_sum;

    State(const vector<int>& a, const vector<UpdateInfo>& u) 
        : a(a), updates(&u), current_sum(0) {}

    // L や R のポインタによる通常クエリの操作
    void AddLeft(int index) { current_sum += a[index]; }
    void AddRight(int index) { current_sum += a[index]; }
    void RemoveLeft(int index) { current_sum -= a[index]; }
    void RemoveRight(int index) { current_sum -= a[index]; }

    // 現在の計算区間が [nl, nr) の状態で、時刻 t 番目の更新を「適用」する
    void ApplyUpdate(int t, int nl, int nr) {
        auto& u = (*updates)[t];
        // 更新場所がアクティブ区間内なら、状態を操作する
        if (nl <= u.idx && u.idx < nr) {
            current_sum -= a[u.idx];
            current_sum += u.new_val;
        }
        // 実データの更新
        a[u.idx] = u.new_val;
    }

    // 現在の計算区間が [nl, nr) の状態で、時刻 t 番目の更新を「元に戻す（Undo）」する
    void UndoUpdate(int t, int nl, int nr) {
        auto& u = (*updates)[t];
        if (nl <= u.idx && u.idx < nr) {
            current_sum -= a[u.idx];
            current_sum += u.old_val;
        }
        // 実データを元の姿に戻す
        a[u.idx] = u.old_val;
    }

    // 現在の区間の答えを取得する
    answer_type GetAnswer() const {
        return current_sum;
    }
};

int main() {
    int n = 10;
    iramina::DynamicMo<State> mo(n);

    // 点更新と区間クエリの順序の例（更新→クエリ→更新 のイメージ）
    mo.AddUpdate();       // 時刻 t が 1 進む（更新クエリを発行した合図）
    mo.AddQuery(0, 5);    // 現時点での時刻 t = 1, 左端 0, 右端 5 のクエリを追加
    mo.AddUpdate();       // 時刻 t が 2 に進む

    // 実際の更新データをユーザー側で用意する
    vector<UpdateInfo> updates = {
        {3, 10, 50}, // index 3 を 10 から 50 に変更（1つ目の更新）
        {5, 20, 10}  // index 5 を 20 から 10 に変更（2つ目の更新）
    };
    
    vector<int> a = {1, 2, 1, 10, 2, 20, 1, 5, 2, 3};

    State state(a, updates);
    vector<long long> ans(1); // AddQuery の回数分のサイズを確保
    mo.Run(state, ans);
}
```

## コンストラクタ

```cpp
DynamicMo(int n)
```

- 対象となる配列等の初期長さを $n$ とします。
- 制約：$n \geq 1$

**計算量**

- $O(1)$

## AddUpdate / AddQuery

```cpp
void mo.AddUpdate()
// ----
void mo.AddQuery(int l, int r)
```

- `AddUpdate()`: 更新クエリが発行された事実を記録します。内部で時刻 $T$ が $1$ 増加します（実際の配列データの書き換えは `State` に任せます）。
- `AddQuery(int l, int r)`: 現在の時刻 $T$ に結びつけられた形で、半開区間 $[l, r)$ に対するクエリを追加します。
- 制約：$0 \le l \le r \le n$

**計算量**

- どちらも $O(1)$

## Run

```cpp
void mo.Run(State& state, std::vector<answer_type>& ans) const
```

- 追加されたクエリをオフラインで時間 $T$ の軸も入れた 3D（Z-Order スネーク配列）でソートし、効率的にポインタを移動させて状態を回答します。

**`State` に要求される仕様**

テンプレート引数に渡す `State` 構造体（またはクラス）は、以下のメンバーとメソッドを持つ必要があります。
```cpp
struct State {
    using answer_type = 戻り値の型;
    void AddLeft(int index);
    void AddRight(int index);
    void RemoveLeft(int index);
    void RemoveRight(int index);
    void ApplyUpdate(int t, int nl, int nr);
    void UndoUpdate(int t, int nl, int nr);
    answer_type GetAnswer() const;
};
```
- `ApplyUpdate(int t, int nl, int nr)`: 時刻 $T$ に発行された `t` 番目の更新操作を適用してください。更新対象のインデックスがアクティブ区間 `[nl, nr)` 内にある場合は、答え（`GetAnswer()` で返される状態）自体にも更新を加えるようになど、状況によった対応が必要です。
- `UndoUpdate` はその逆操作です。

**計算量**

- $N$ を配列長、$Q$ をクエリ回数、$U$ を更新回数とし、最適なブロックサイズ $B \approx N^{2/3}$ によって、状態の更新メソッドの平均呼び出し回数は $O(N^{5/3})$ 弱（およそ $O(N^{5/3} \cdot U^{1/3})$ 等のオーダー）となります。
