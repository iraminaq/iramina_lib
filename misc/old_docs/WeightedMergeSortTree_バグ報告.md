# MergeSortTree テスト実行報告およびバグ・エラー報告

現在、対象となるテストプログラム ( `MergeSortTree_test.cpp` ) を作成しコンパイルを実行しましたが、以下の理由により実行に失敗しました。

## エラー原因

* 読み込んでいるヘッダファイル `../../include/iramina/MargeSortTree/MargeSortTree.hpp` の中身が現在空になっており、実装が存在しません。
* そのため、テスト内で使用している `Elem` 構造体や `MergeSortTree` クラスが未定義であるというコンパイルエラーが発生しています（例: `error: 'Elem' was not declared in this scope` 等）。

## 今後の対応についてのお願い

* `include/iramina/MargeSortTree/MargeSortTree.hpp` （または該当のソースコード）へ、`Elem` 構造体と `MergeSortTree` クラス等の実装の追記をお願いいたします。
* もし、`MergeSortTree` の実装時に、仕様（テンプレートにする、名前空間 `iramina` に入れるなど）を変更した場合は教えて頂ければ、それに合わせてテストのテストコード `MergeSortTree_test.cpp` 側も修正いたします。

実装が完了しましたら、再度お知らせいただければテスト（ `g++ -std=c++20 MergeSortTree_test.cpp -o MergeSortTree_test.exe` および実行）を実施して動作確認を行います。
