﻿# 紅桜エディタ

## はじめに

紅桜エディタは、RED2 の機能や操作性をサクラエディタ(Sakura Editor)で実現するために派生させたエディタです。  
RED2 の多くの機能は、サクラエディタの機能で実現可能ですので、キーバインドやメニューデフォルトの変更が主
ですが、一部、機能拡張も行なっています。  


## RED というエディタの系譜と紅桜の開発経緯

RED は 1980 年代後半 ～ 1990 年代前半にかけて、ライフボート社が MS-DOS 用に販売していたエディタで、MIFES や VZ Editor、
FINAL などと同じく、WordStar(WordMaster) 系の操作性を継承したエディタとして、一世を風靡しました。
RED の最終バージョンは 1994 年にリリースされた MS-DOS 用の RED2 Ver.2.15A であり、残念ながら Windows 版は発売されませんでした。  

 その後、iketeru 氏により、RED2 の機能を継承した Windows9x/NT 系 OS 用の WM32 Editor がフリーウェアとしてリリースされました。
WM32 は RED2 以上の機能を実装したすばらしいエディタですが、Unicode に対応していないため、UTF-8 がデフォルトとなった昨今の
Linux 向けのソフトウェア開発には利用しずらい状況でした。  

そこで、Unicode 対応の RED2 を目標に、サクラエディタをベースに変更を始めたのが紅桜の開発経緯です。
残念ながら、未だ 20 年以上前の RED2 の機能に追いついていない部分も多いのですが、徐々に実装、および、改善したいと思います。  


## 使用上の注意

* メインメニュー、キー割り当て、カラーリングの反映  
  バージョンアップ時に、これらのデフォルト値の変更を反映させるためには、以下の機能を使用してください。
  ただし、カスタマイズしていた場合には、元に戻ってしまいますので、ご注意ください。  
    - メインメニュー : 共通設定 -> メインメニュー タブにある「初期設定」ボタン  
    - キー割り当て : 共通設定 -> キー割り当て タブにある「プリセットに戻す」ドロップダウンリスト  
    - カラーリング : タイプ別設定 -> カラー タブにある「プリセット配色に戻す」ドロップダウンリスト  


* デフォルト値の変更の反映  
  バージョンアップ時に、共通設定やタイプ別設定のデフォルト値、カスタムメニューの変更を反映させるためには、BeniZakura.ini を削除してから
  紅桜を起動する必要があります。BeniZakura.ini には各種設定や履歴情報(※1)、カスタマイズしたメニューやカラー設定などが格納されているため、
  これらの情報を維持したい場合には、以前のファイルを保存しておき、新たに生成された ini ファイルと手動でマージする必要があります。  
  マージの手間を減らすためにも、BeniZakura.ini はバージョン管理ツールを用いて管理しておくことをお勧めします。  
  （バージョンアップ前に、ini ファイルをコミットしてから削除する。）  
  ※1 Ver.2015.8.1.28 以降、「最近使ったファイル／フォルダ」に関する情報は別ファイル (BeniZakura.mru.ini) に分離しました。


---
## ソースコード

### ライセンス

サクラエディタの最近の変更部分については「zlib/libpng ライセンス」を適用しているようですので、紅桜の変更部分も
これに倣い、「 zlib / libpng ライセンス」を適用します。

### ソースコード リポジトリ

本家サクラエディタの Subversion リポジトリ内の trunk2 ディレクトリを HgSubversion にて Mercurial リポジトリに
変換したものをベースにしています。
ソースコード リポジトリは bitbucket にて公開しています。

https://bitbucket.org/suzzsegv/benizakura/


## ビルド方法

Visual Studio Community 2013 を用いてビルドしています。sakura/BeniZakura\_vs2013.sln をダブルクリックして
IDE を起動後、ターゲットとして Release\_Unicode を選択、ビルドメニューからビルドを行ってください。

---
## 変更点

### 2016/04/02 - Ver.2016.4.2.30

 * 「タブをまとめてグループ化」時に Aero Snap によるウィンドウサイズ変更に追従するように変更  
   Aero Snap でウィンドウサイズの変更を行ったあとに、他のタブに切り替えた場合などに、ウィンドウの位置とサイズが
   Aero Snap 前の状態に戻ってしまう不具合を修正。  
   制限事項:  
     - Windows10 上で動作、かつ、「起動時のウィンドウの大きさを継承」の設定の場合に、
       前回終了時のウィンドウサイズより若干小さいサイズのウィンドウで起動してしまう。


 * 編集時のデフォルトの文字コードを UTF-8 に、デフォルトの改行コードを LF に変更  
   タイプ別設定のデフォルトの文字コードを SJIS から UTF-8 に、デフォルトの改行コードを CR+LF から LF に変更。


 * セッションの復元時に、既に存在しないファイルは開かないように変更  


 * 「最近使ったファイル／フォルダ」用 ini ファイル名を変更  
   BeniZakura.mru.ini  から BeniZakura.RecentFiles.ini に変更。


 * 「メインメニュー」、「カスタムメニュー」の設定を別々の ini ファイルに保存するように変更。  
   メインメニュー: BeniZakura.MainMenu.ini  
   カスタムメニュー: BeniZakura.CustomMenu.ini  


 * 「新規作成」で新しいエディタプロセスを生成する時に、プロセスの起動完了を待つように変更  
   キーにより新規ウィンドウを作成した場合に、フォーカスが失われてしまう場合があったのでウェイトを追加したのだが、
   効果なさそう・・・。


### 2016/03/01 - Ver.2016.3.1.29

 * セッションの復元機能を追加  
   前回終了時に開いていたファイルを復元する機能。終了時に、開いていたファイル一覧を BeniZakura.PreviouslyOpenedFiles.txt に保存し、次回起動時に復元を行う。  
   制限事項:  
     - 複数グループのエディタを閉じた場合に、正しく動作しない。  
     - シャットダウン中のファイル保存確認ダイアログに対して「キャンセル」を選択すると保存確認ダイアログが
       もう一度表示されてしまう。  


 * タグファイル作成ダイアログ: 「UTF-8 指定を追加する」チェックボックスを追加  
   チェックボックスを ON にすると、文字コードに UTF-8 を指定してタグファイルを生成する。  
   (オプションに "--jcode=utf8" を追加して ctags の実行を行う)


 * Command: ファイルの排他制御モードを切り替えた場合に、ファイルを開き直すように変更  
   ファイルの排他制御を「排他ロック」または「共有ロック」に切り替えた場合に、オープン中のファイルを開き直すように変更。
   共有ファイルサーバ上のファイルを開いて、排他モードを切り替えるまでの間に他の人が同じファイルを更新してしまうと、
   古いデータで上書きしてしまう問題が発生したため。これを回避するために、排他モードを切り替えてから、同一ファイルの
   データを再度読み直すように変更した。


 * Menu: メインメニュー -> ツール に「大文字」、「小文字」、「マクロ実行」の項目を追加  


 * Menu: 「名前を指定してマクロ実行...」の名称を「マクロ実行...」に変更  


 * Menu: カスタムメニュー４に「キーマクロの保存」、「キーマクロの読み込み」を追加  


### 2015/08/01 - Ver.2015.8.1.28

 * 幅ゼロの矩形選択を行った場合に、直線を描画するように変更


 * Grep の出力結果から文字コードの表示を削除  


 * 「シンボル検索」にて、検索結果がプロトタイプだった場合には (); を付加するように変更  


 * ウィンドウおよびタブキャプションの変更ファイルに関する表示を「(更新)」から「\*」に変更  
    他のアプリで一般的なファイル名の後に「\*」を付加する形に変更した。  
    （ウィンドウリストの変更ファイルは「\*」が付加される表示だったため、これに統一。）  


 * 「最近使ったファイル／フォルダ」に関する情報を BeniZakura.mru.ini に保存するように変更  
    BeniZakura.ini に保存されていた「最近使ったファイル／フォルダ」に関する情報を、BeniZakura.mru.ini という
    別ファイルに分離して保存するように変更。  
    バージョンアップ後の初回起動時には「最近使ったファイル／フォルダ」の情報は削除されます。
    （BeniZakura.ini のバックアップファイル(BeniZakura.ini.bak)から該当セクションを
    コピーすることで移行は可能です。）


 * メインメニュー -> ファイル -> 最近使ったファイル／フォルダ、および、メインメニュー -> 履歴の表示方法を変更
    - ファイル名 - フォルダ名 (アクセスキー) の順に表示するように変更  
    - パス区切り文字を \ から / に変更  

 * 「現在位置の単語で検索」の実行時に、検索開始位置に登録するように変更  
    「検索ダイアログ」で検索した時と同様、「検索開始位置に戻る」で戻れるように。  


 * 紅桜プリセット配色を変更  
    - 選択範囲: 少し明るい黄色に  
    - 検索文字列: 文字色を黄色から水色に  
    - コメント: 暗い緑に  


 * 「設定(カスタムメニュー５)」の「ビューモード」のアクセスキー定義が誤っていた不具合を修正  
    (T) -> (V) に修正しました。


### 2015/07/01 - Ver.2015.7.1.27

 * カスタムメニューの定義を変更
    - RED2 の「設定」機能の代わりとして、カスタムメニュー５に画面表示関連の機能を定義して F5 キーに割り当て  
    - 「FINAL Ctrl+Q メニュー」に「次のブックマークへ」、「前のブックマークへ」を追加
       (「FINAL Ctrl+Q メニュー」はカスタムメニュー５から６に移動)  
    - カスタムメニュー１～４の名称から "RED2" という文言を削除  


 * 「キーワードを指定してタグジャンプ」機能の名称を「シンボル検索」に変更  


 * 「シンボル検索」ダイアログにて、検索結果の表示列の順序を変更  
    参照優先度が高い順になるように、列の表示を「シンボル」、「ファイル名」、「行番号」、「種類」、「備考」の順に変更した。
    また、ディレクトリ階層を示す「階層」の列は削除した。  


 * 「シンボル検索」ダイアログにて、検索結果のキーワードに (), :: を付加するように  
    検索した結果が関数だった場合には () を、構造体 or クラスだった場合には :: をキーワードの後ろに付加するように変更。
    また、検索結果のキーワードの最大文字数を 64 文字までに変更。  


 * 「シンボル検索」ダイアログにて、長い名前のシンボルを検索できるように変更  


 * 「シンボル検索」ダイアログにて、検索結果が表示される前にリターンキーを押してもダイアログが閉じないように  
    ジャンプ先のシンボルが選択されていない状態でリターンキーを押した場合に、これを無視するように変更。


 * デバッグビルド版で「シンボル検索」ダイアログを開くと assert で停止する不具合を修正  


 * 紅桜キーバインドのデフォルトを変更  
    - Ctrl + O : シンボル検索（キーワードを指定してタグジャンプ）  
    - Ctrl + Shift + O : シンボル検索（キーワードを指定してタグジャンプ）  


 * Command: 電卓を起動する機能を追加  
    Windows 標準アクセサリの「電卓(Calc.exe)」を起動する機能を追加し、紅桜キーバインド デフォルトの Shift + F7 に設定。
    (RED2 のバインドと同様)  


 * Command: コマンドプロンプトを起動する機能を追加  
    編集中のファイルが格納されたディレクトリを指定して cmd.exe を起動する機能を追加。  
    この機能を、紅桜キーバインドの Ctrl + Shift + X に設定し、メインメーニューの「ツール」にも項目を追加した。  


 * 幅ゼロの矩形選択をした状態でインデント機能を実行すると、選択範囲が最大桁となる動作を削除  
    分かりにくい挙動なので通常のインデント動作になるように変更した。  


 * 検索/置換/Grep ダイアログの「英大文字と小文字を区別する」のデフォルトを有効に変更  


 * ツールバー＆メニュー用アイコンを変更  
    218: Explorer 起動(ファイルツリー表示): VS2012 標準アイコンをベースに作成  
    266: 共通設定: プロパティ -> ギア アイコンに変更  
    269: 履歴の管理: フラッグ -> ライブラリ アイコンに変更  
    248: タグファイルの作成: VS2012 標準アイコンをベースに変更  
    249: ダイレクトタグジャンプ: VS2012 標準アイコンをベースに変更  
    281: 外部コマンド実行: VS2012 標準アイコンをベースに変更  
    449: TortoiseHg annotate: Mercurial のアイコンを新規追加  
    450: コマンドプロンプトを起動: VS2012 標準アイコンを追加  


---
### 2015/06/01 - Ver.2015.6.1.26

 * 「キーワードを指定してタグジャンプ」にて、複数の条件で自動検索する機能を追加  
  細かい条件を指定しなくても検索してくれる機能に変更しました。
  以下の順に検索を行います。
    1. キーワードに完全一致
       完全に一致するシンボルが見つかった場合には、以下条件での検索は行いません。
    2. キーワードで始まる  
    3. キーワードを含む  
    4. 大文字小文字を区別せず、キーワードで始まる  
    5. 大文字小文字を区別せず、キーワードを含む  
    6. キーワードで指定した文字が順に含まれる  
    7. 大文字小文字を区別せず、キーワードで指定した文字が順に含まれる  
  機能的に Command Pallet や Fuzzy Finder には及びませんが、代わりに使えるように・・・
  というのが変更の主旨です。  
  （Ver.2013.11.01.12 にて、タグ作成時にファイル名を含めるように変更していますので、
  この機能でファイル名も検索できます。）


 * 「キーワードを指定してタグジャンプ」のキーワード初期値を空欄に変更  
  通常、新たなキーワードで検索したいはずなので、初期値は空欄にしました。


 * カーソル UP2, DOWN2 の移動行数を２行から「スクロール行数の２倍」に変更  
  「カーソル上移動(２行ごと)」、「カーソル下移動(２行ごと)」の名称を  
  「カーソル上移動(複数行)」　、「カーソル下移動(複数行)」　に変更し、
  共通設定 -> 全般 -> スクロール -> 行数 で指定された値の２倍を移動行数として使用するように変更。  
  「キーリピートによるカーソル移動よりも遅い」のでは役に立たないので、移動量を増やしました。  


 * タグジャンプで新たにファイルを開いた場合に、カーソル位置が画面中央に表示されるように変更  


 * Grep 実行時、アウトラインのドッキングウィンドウを表示しないように変更  


 * タブメニューがカーソル位置に表示されてしまう不具合を修正  
  タブ上で右クリックした時に表示される「タブメニュー」がマウスカーソル位置ではなく、カーソル位置に表示されていた。  
  Ver.2015.4.1.25 の「カスタムメニューの表示位置をマウスカーソル位置からカーソル位置に変更」によるデグレード。


 * 「タブメニュー」のデフォルト項目を整理  
  使用頻度の低い項目を削除し、秀丸に合わせて「Explorer起動」を追加。  


 * 「右クリックメニュー」から「ファイルの先頭に移動」、「ファイルの最後に移動」の項目を削除  
  元々、これらの機能をキーに割り当てていなかった頃の名残なので、削除した。  
  (現在は、Ctrl + Shift + R/C キーと「Ctrl + Q メニュー」にも割り当てられている。)  


---
### 2015/04/01 - Ver.2015.4.1.25

* プリセット配色(タイプ別設定->カラータブ->「プリセット配色に戻す」)を追加
  以下の３種類を追加  
    - MIFES Bule  
    - 秀丸エディタ Black  
    - Monokai  
  また、プリセット配色変更時に、カーソルや TAB 記号、改行記号の表示／非表示も設定するように変更しました。  


* 「カスタムメニュー５」のデフォルトを FINAL の Ctrl+Q メニューの定義に変更し、紅桜と秀丸の Ctrl+Q キーに割り当て  

* カスタムメニューの表示位置を「マウスカーソル位置」から「カーソル位置」に変更  
  「秀丸」と同様の動作に変更しました。


* キーバインドのデフォルトを変更  
    - Shift + Esc : Undo  
    - Ctrl + W : 閉じる  


* 拡張子 C の場合でも C++ 形式のアウトラインを使用するように  

---
### 2015/03/01 - Ver.2015.3.1.24
 * 共通設定 -> キー割り当て タブに「プリセットに戻す」機能を追加  
  キー割り当てを一括でデフォルト状態に戻すためのドロップダウンリストを追加。  
  以下の 3 種類が選択できます。
    - 紅桜  
    - サクラエディタ  
    - 秀丸エディタ  
   「秀丸エディタ」のキーバインドに関して、機能が不足しているものは「未割付」の状態です。

 * 共通設定->キー割り当て タブのコンポーネントの配置を変更  
   「プリセットに戻す」を配置する都合上配置を見直しました。  
    - ダイアログ左側にキーの選択、右側に機能の選択を配置  
    - キーに割り当て / 割り当て解除 ボタンを右下に配置  
    - インポート / エクスポート ボタンを左下に配置  
    - 「ウィンドウ毎にアクセラレータテーブルを... 」-> 「Wine上で実行」に変更  

 * 画面フォントサイズの「拡大・縮小」、「リセット」、「最小化」の機能を追加  
   以下のキーで編集画面のフォントサイズを変更できます。  
    - Ctrl + : 拡大  
    - Ctrl - : 縮小  
    - Ctrl 0 : リセット  
    - Ctrl 1 : 最小化  
   また、メニューの「表示」->「ズーム」でも拡大・縮小・リセットを変更できます。  
   フォントサイズのリセットは 12 ポイントに、最小化は 4 ポイントに変更します。  
   この機能は、共通設定(相当)のフォントサイズが変更されるため、全てのエディタ画面が変更されます。  
   ※ 本家サクラエディタ 2.1.0.0 以降では、マウスのホイールでフォントサイズの変更が可能ですが・・・追従できていません。  

 * 「TAB記号の表示(トグル)」、「改行記号の表示(トグル)」機能を追加  
   以下のキーでTAB記号、改行記号の表示を切り替えることができます。  
    - Ctrl + O : 改行記号の表示(トグル)  
    - Ctrl + Shift + O : TAB記号の表示(トグル)  
   この機能は、タイプ別設定 -> カラー -> 色指定 が変更されるため、全ての同一タイプのエディタ画面が変更されます。  

 * タブ記号の幅を切り替える「TAB 2」,「TAB 4」,「TAB 8」機能を追加  
   以下のキーで編集画面のタブ幅を変更できます。  
    - Ctrl + 2 : TAB 2  
    - Ctrl + 4 : TAB 4  
    - Ctrl + 8 : TAB 8  
   また、メニューの「表示」->「TAB記号」でも変更できます。  
   この機能は、タイプ別設定 -> スクリーン -> レイアウト にある、TAB 幅が変更されるため、全ての同一タイプのエディタ画面が
   変更されます。  

 * 「現在位置の単語で検索」の初回実行時に「単語単位で探す」オプションを有効に変更  
   「現在位置の単語で検索」は、デフォルトでダブルクリックと Ctrl + L にバインドしている機能。  
   Ctrl + L を連続実行した場合には、従来通りに通常の検索になります。  

 * 検索/置換/Grep ダイアログの検索条件を、それぞれ別々に保持できるように変更  
   既存の実装では、検索/置換/Grep ダイアログの検索条件（単語単位/大小区別/正規表現）を共有していたが、ダイアログ毎に別々に保持する
   ように変更した。  
   ini ファイルへの保存は行っていないため、エディタを再起動すると検索条件はデフォルトの無効に戻ります。  
   Rev.0e791a55343e で「現在位置の単語で検索」時に「単語単位で探す」を有効にしたが、この変更により検索ダイアログを開くたびに
   「単語単位で探す」が有効になってしまい、使いづらくなってしまった。
   （ダイアログを開いて検索を行う場合は、キーワードを手入力してあいまい検索を行いたい場合なので、「単語単位で探す」を無効にする
   事の方が多い。）  

 * C/C++ データタイプの強調キーワードに intN_t, uintN_t, INTn, UINTn を追加  

 * キーバインドのデフォルトを変更
    - Shift + F11 : アウトライン解析(トグル)
    - Alt + Enter : ウィンドウの最大化
    - PgUp : 半ページアップ
    - PgDn : 半ページダウン
    - Shift + PgUp : １ページアップ
    - Shift + PgDn : １ページダウン
    - Ctrl + ↑ : カーソル上移動(２行ごと)
    - Ctrl + ↓ : カーソル下移動(２行ごと)
    - Ctrl + ← : 単語の左端に移動
    - Ctrl + → : 単語の右端に移動

 * 「英大文字小文字を区別しない」条件での Grep が実行できなかった不具合を修正  
   Rev.ab9ba5dc7b93 : 検索や GREP 時の「英大文字と小文字を区別する」のデフォルトを有効に変更 の変更によるデグレード。  
   今まで、大小区別した Grep しか使用していなかったので、全く気が付かなかった・・・のだが、上記のダイアログ毎にデフォルトを
   持つように変更＋デフォルトが無効に変わったことによって気が付いた。。。

 * 置換ダイアログの「見つからないときにメッセージを表示」チェックボックスが残っていた不具合を修正  
   Rev.14a14b062a7a にてダイアログ表示を削除していたため、このチェックボックスは機能していない状態だった。
   検索に関しては、Rev.b4c0765baac9 にて削除していたが、置換は削除漏れしていた。  


---
### 2015/02/01 - Ver.2015.2.1.23

 * メインメニューの項目を整理して大幅に削減  
   あまり使用しないメニュー項目を削除し、主要アプリケーションに近いメニューに変更しました。  

 * 編集中ファイルの排他制御モードを切り替える機能を追加  
   メインメニュー -> ファイル -> 排他制御 を使用して、編集中ファイルに対する排他制御（ロック状態）を
   切り替える機能を追加しました。
   この機能は、別のウィンドウやタブで編集中のファイルの状態は変化しません。
   また、共通設定 -> ファイルタブ -> 排他制御 の設定を変更した場合に、即時反映を行わないように変更しました。
   （以前の動作では、全ての編集中ファイルの排他制御状態が共通設定の変更に追従して即時反映されていました）  
    - 排他ロック  
      編集中のファイルを排他ロック（読み書き禁止）状態に変更  

    - 共有ロック  
      編集中のファイルを共有ロック（上書き禁止）状態に変更  

    - ロック解除  
      編集中のファイルのロックを解除  

 * 「アウトライン解析」をドッキング表示中に「アウトライン解析」を実行(F11 キー押下)した場合に、
   再解析を行うように変更  

 * 共通設定: 「ファイル履歴」と「フォルダ履歴」表示数のデフォルトを 15 -> 36(最大値)に変更  

 * 共通設定: アウトライン解析ウィンドウ内の▼メニューにある「ドッキング配置を同期」のデフォルトを無効に変更  

 * Menu: ファイル履歴: ファイルの文字コードが SJIS の場合でも文字コードを表示するように変更  

 * C/C++ プリプロセッサコメントのカラーリングにて、大文字も有効になっていた不具合を修正  
   \#IF 0 ～ \#ENDIF のように、大文字で記述されていた場合でもカラーリングが有効になっていた不具合を修正。

 * タグファイル生成後のダイアログ表示を削除  
   ダイアログの代わりに、ステータスバーに「タグファイルの作成が終了しました。」と表示するように変更。

 * 次/前のブックマークが見つからなかった場合のダイアログ表示を削除  

 * 半角数値のカラーリングにて、C 言語の f/F, l/L 接尾語を含めないように変更  
   16進数のダンプ出力などを表示中に f だけ半角数値扱いでカラーリングされるという微妙な動作だったため、
   接尾語を含めないように変更した。
   （実数の接尾語のみの対応で、整数の接尾語には対応していないという中途半端な実装状態だったことも理由。）  

 * 半角数値のカラーリングにて、指数表現の対応を削除  

---
### 2015/01/01 - Ver.2015.1.1.22
* ツールバーアイコンを「サクラエディタWiki」の「平坦な」ベースのアイコンに変更  
	「平坦な」アイコンに対して、サーチアイコンを上下矢印に変更。
	また、インクリメンタルサーチのカーソルも、同じ上下矢印に変更。  

	関連:  
		http://sakura.qp.land.to/?Customize%2FIcons  


* タイプ別設定->カラータブを開いた時に、行コメント開始桁入力ボックスの Disable が機能していない不具合を修正  
	「SVN Rev.2680: Chg:カラータブ「背景画像」「行番号の表示」「行番号区切り」->ウィンドウタブ」変更時のデグレード。  

* タイプ別設定->カラータブに「プリセット配色に戻す」機能を追加  
	カラーリングの設定を一括でプリセット配色に戻すためのドロップダウンリストを追加。  
	以下の３種類のプリセットから選択できます。
	- 紅桜 ... 紅桜のデフォルト配色
	- サクラエディタ ... サクラエディタ風の配色
	- VS2012 Dark ... Visual Studio 2012 の Drak テーマ風の配色

* 選択範囲を描画する際のα値を変更  
	選択範囲の「文字色」と「背景色」に同じ色を指定した場合、アルファブレンディングが行われるが、
	この際のα値を以下のように変更した。  
	- 文字を描画する際のα値を 30% から 0% に変更(アルファブレンディングを行わない）
	- 背景を描画する際のα値を 30% から 50% に変更

* 紅桜プリセット配色を変更  
	- カーソル(IME ON)、半角数値、対括弧の強調表示、シングルクォーテーション文字列、
	- ダブルクォーテーション文字列、強調キーワード4: ff3030 -> ff4040 (少し明るい赤に)
	- コメント: 00c000 -> 309850 (少し明るい緑に)
	- 強調キーワード1: ff8000 -> ff9840 (少し明るいオレンジに)
	- 強調キーワード5: ff0080 -> ff4098 (少し明るいピンクに)
	- 強調キーワード6: ff8000 -> 98ff98 (オレンジから palegreen)
	- 正規表現キーワード1: 98fb98 -> 9898ff (palegreen から濃いラベンダーに)

* 「SVN Rev.3081: New: NEL,LS,PS限定的サポート(3f9ab6058cae)」の変更を元に戻した  
	「NEL, LS, PS 改行コード対応」にはデグレードが多く、あまり使用しない機能でもあるため、元に戻すことで対応した。  

	関連:  
		SVN Rev.3748: Fix: Pythonの改行コードチェックが無効になっていたのを修正(b2ea92a1420e)  
		SVN Rev.3765: Fix: UnicodeファイルでNELが含まれているとNELが改行コードにならず描画で無限ループする(e8a2120245c2)  


* 「SVN Rev.3757: Fix: 検索/置換/grepダイアログの検索履歴の初回ドロップダウン幅不正(7b58ad50d010)」の変更を適用  

	http://sourceforge.net/p/sakura-editor/patchunicode/852/  
	http://sakura.qp.land.to/?BugReport%2F170  
	検索/置換/grepダイアログの不具合です。検索履歴のドロップダウンリスト表示で、コンボボックス幅に入りきらない
	長い文字列がリストに含まれている場合に、表示幅が拡張されなくなっています。
	一度リストを閉じて再表示すると正常に一番長い文字列の幅に拡張されます。


* 「SVN Rev.3770: Fix: CKeyBind.cppでDebug_x64のときCStrictIntegerのエラー(aa24889dc6eb)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/862/  

* 「SVN Rev.3772: Fix: ファイルを開いた後落ちるとブックマークが消える(ecacbf6f64c8)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/859/  
	ファイルを開いた後のMRUの登録でブックマークを保存していないため、編集中にエディタが落ちると元のブックマークが消えるので、
	それを修正します。  

* 「SVN Rev.3815: Fix: 分割直後のビューで背景が透過されない(92e73a8ac036)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/866/  
	カーソル行背景色が有効の状態で画面分割を行った場合に、新たに表示されたペインのカーソル行背景が正しく表示されない。  

* 「SVN Rev.3817: Fix: アウトラインが右・下にあるときにビューにサイズボックスが表示されたままになる(d9331f5f2fd2)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/874/  
	ステータスバーとファンクションキーを表示していない状態でアウトラインウィンドウを右または下にドッキングした場合に、
	ビューにサイズボックスが表示されていた不具合を修正。  

* 「SVN Rev.3820: Fix: 保存確認メッセージが非フォーカス状態で表示される(f7a994dd7dfa)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/858/  

* 「SVN Rev.3822: Fix: メニュー周りの不具合の修正(33f6e31e9e5b)」の変更を適用  
	「長いコマンドをメインメニューに追加した後にメインメニュー設定を開くとバリデーションエラーが起きるのを修正」の修正を適用。  

---
### 2014/12/01 - Ver.2014.12.1.21
* 「SVN Rev.3737: Fix: 1023文字を超える長さの補完をするとバッファオーバーランする(f394c03f6f60)」に相当する修正  
	未反映のチャンジセットである「SVN Rev.3161(8860d8feab21)」に依存していたため、旧ソース用に変更して適用。  

	http://sourceforge.net/p/sakura-editor/patchunicode/831/  
	長すぎる候補は、補完ファイル・補完プラグインから設定できます。ファイル内容から補完する場合は長さ制限が適用されています。  

* 「SVN Rev.3738: Fix: タイプ別設定のインポートで約122文字以上のファイル名を持つ色設定があるとバッファオーバーラン」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/832/  

* 「SVN Rev.3739: Fix: タイプ別設定の名前拡張子タイプ数が多いとファイルダイアログで異常終了する」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/834/  

* 「タイプ別設定」ダイアログを閉じると「色の設定」の「作成した色」が失われてしまう不具合を修正  
	「SVN Rev.3742(c078de6a149d): Fix: 作成した色の保持」に相当する変更。  
	未反映のチャンジセットである「SVN Rev.3450(1b5867d4a772): Keep: CPropCommonクラスとCPropTypesクラスの動的生成」に
	依存していたため、必要な変更を追加して適用。

---
### 2014/11/01 - Ver.2014.11.1.20
* 「SVN Rev.3664: Fix: GetDocumentationの修正」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/782/  

* 「SVN Rev.3672: Keep: 構造体のコピーでmemcpy_rawを使わない」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/797/  

* 「SVN Rev.3697: Fix: gccでお気に入りダイアログでフリーズする」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/791/  

* 「SVN Rev.3698: Fix: mingwでのコンパイル警告・バグの修正」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/792/  
	mingw環境での警告-Wallでの問題などを修正します。  
	・ふぁんくらぶ part16 >>476 windressで "\'" がunrecognized escape sequence  
	・未使用変数の削除  
	・アウトライン解析ルールファイルの改行を取り除く処理の修正  
	・プラグインフォルダ検索時の条件式修正  
	・タイプ別フォント選択時の等幅チェックの修正  

* 「SVN Rev.3700: Fix: 折り返し位置への挿入での再描画不足(r3069-)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/802/  

* 「SVN Rev.3714: Fix: 行頭禁則と行末禁則の初期値」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/807/  
	- 行頭禁則  
	重複している文字を変更しました(U+FFE0 -> U+00A2)。  
	- 行末禁則  
	重複している文字を変更しました(U+FFE1 -> U+00A3）。  
	円記号を追加しました(U+00A5)。  

* 「SVN Rev.3717: Fix: アクティブ化時にキャプション更新されないことがある(r2929-)」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/815/  
	[r2929]の変更を元に戻します。  

* 「SVN Rev.3724」の変更を適用  
	Fix: 強調キーワード長の設定・コピーがおかしいのを修正  
	Fix: 強調キーワードの文字列コピーのバッファオーバーランの可能性の修正  

* 「SVN Rev.3727: Fix: 対括弧強調の状態で選択すると対括弧が強調されたままになる」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/814/  

* 「SVN Rev.3728: Fix: マクロで引数無しのSearchNext()/SearchPrev()でエラー」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/818/  

	紅桜では未適用であった「SVN Rev.3160: New: ダイアログ表示/結果適用のマクロを追加」に依存していたため、
	必要な変更を追加して適用。  

* 「SVN Rev.3730: Fix: 文字幅に合わせてスペースを詰めるのヘルプID修正」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/820/  
	IDが「ラインモード貼り付けを可能にする」と重複していたため変更します。  

* 「SVN Rev.3731: Fix: ウィンドウの位置と大きさの位置に-(マイナス記号)を直接入力できない」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/822/  
	マイナスを直接入力できず、UpDownコントロールで一度マイナスにする必要があったのを修正します  

* 「SVN Rev.3732: Fix: トレイメニューのホットキーをなしにするとなんでも反応する」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/823/  
	>サクラエディタが「非アクティブ」のとき、F1キーを押すとサクラエディタのメニューが出てきます。  
	F1キーに関しては再現しませんでしたが、私の環境だと「なし」にすると、
	マウスパッドの拡大縮小をするとトレイメニューが反応します。  
	「なし」のときは登録しないようにし、これを反応しないようにします。  

* 「SVN Rev.3733: Fix: マクロパスのブロック範囲外でのアクセスの修正」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/824/  

* 「SVN Rev.3735: Fix: タグジャンプの!_TAG_S_SEARCH_NEXTが正常に処理されない」の変更を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/827/  
	tagsファイルに次のタグファイルの場所を指定する "!_TAG_S_SEARCH_NEXT" の処理がおかしくて
	次のタグが検索されないことがあるバグを修正します。  

---
### 2014/10/01 - Ver.2014.10.1.19
* 「ダイレクトタグジャンプリスト」、「アウトライン解析」の表示に編集用フォントを使用するように変更  
	高 DPI 環境において、文字が小さくて読みにくいため、編集画面で使用しているフォントとフォントサイズを、
	以下に適用するように変更。
	- 「ダイレクトタグジャンプリスト」のリストビュー
	- 「アウトライン解析」のリストビュー
	- 「アウトライン解析」のツリービュー

* タイプ別設定: C/C++: デフォルト値に「関数を強調表示するための正規表現キーワード」を追加。  
	正規表現は SakuraEditorWiki の以下のページのものを使用。  
	http://sakura.qp.land.to/?%C0%B5%B5%AC%C9%BD%B8%BD%2F%C5%EA%B9%C6#w02a741e

* デフォルトの表示色を変更  
	コメント: 00d000 -> 00c000 (１段暗い緑に)  
	シングルクォーテーション文字列: 48d0c0 -> ff3030 (ダブルクォーテーション文字列と同じ赤に)  
	正規表現キーワード1: デフォルト有効に変更し、40ffff -> 98fb98 (シアン系の色からpalegreenに)  

* 「SVN Rev.3626」の修正を適用  
	Keep: CFileLoadの変換でEOLも含めるように  
	Fix: ファイル読み込み進捗で改行コード長が抜けていた  

	http://sourceforge.net/p/sakura-editor/patchunicode/736/  
	pUnicodeBuffer->AppendString(EOL)でメモリが再確保されることがあるので、cLineBufferの段階でEOLを含めるようにします。  
	M_nReadLengthに改行コード長が抜けていたバグが修正されてました。(ファイル読み込みなどで100%にならない)  
	・余分な代入を削除  

* 「SVN Rev.3638: Fix: CTipWndクラスのメンバ変数の初期化」の修正を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/779/  
	m_hFontをコンストラクタで初期化するようにしました。Create() 呼び出し時に、Debug ビルドのときは
	m_hFont = 0xcdcdcdcd なので、DeleteObject() を呼び出しますが、Release ビルド時は m_hFont = 0 になるので
	大丈夫みたいです。

* 「SVN Rev.3640: Fix: 現在位置の単語選択」不具合修正を適用  
	http://sourceforge.net/p/sakura-editor/patchunicode/781/  
	[r3417]以降、現在位置の単語選択が再実行できない不具合を修正しました。

---
### 2014/08/01 - Ver.2014.8.1.18
* 共通設定: ウィンドウ -> タイトルバー のデフォルト値に「排他制御の状態」を表示するパラメータを追加  
	共通設定 -> ファイル -> ファイルの排他制御 にて、ファイルの排他制御を有効にした場合、タイトルバーに
	"(排他: 上書き禁止)" もしくは "(排他: 読み書き禁止)" と表示する。

* タイプ別設定: Markdown を追加  
	ファイルの拡張子は ".md"。初期状態で Markdown 用の正規表現キーワードがいくつか登録されています。

* 「カーソル行背景色」有効時に、マウスで矩形選択を行うと、カーソル行背景色が残ってしまう 不具合を修正  
	「SVN Rev.3618: Fix: マウスによる矩形選択でカーソル行背景色の描画不正」に相当する修正。

* 「SVN Rev.3623: Fix: 印刷プレビューで設定変更するとメモリーリーク」不具合修正を適用  

* 「SVN Rev. 3624: Fix: 整数オーバーフローの修正」に相当する不具合修正  
	未反映のチャンジセットである「SVN Rev.3177: New: 文字幅取得マクロの追加」に依存していたため、この変更を一部マージして適用。
	以下のページに簡単な説明はあるが、不具合の再現方法が不明であるため、修正確認はできていない。  

	http://sourceforge.net/p/sakura-editor/patchunicode/755/  
	大きいファイルでフォントサイズを大きくしている場合などに、落ちたり、描画がおかしかったりするのを修正します。

---
### 2014/06/01 - Ver.2014.6.1.17

* ExpandParameter の条件に「ファイルの排他制御状態」を評価する 'X' を追加  
	排他制御のモードを切り替えてファイルをオープンする機能を実現するための予備実装。
	以下の指定で使用可能だが、共有メモリの内容で判定しているため、このままではあまり役に立つ機能ではない。  
	${X?$:(排他: 上書き禁止)$:(排他: 読み書き禁止)$}

* 「Explorer 起動」時にカレントディレクトリの explorer を実行してしまう 不具合を修正  
	「Explorer 起動」コマンドの実行時に、パスを指定せずに explorer.exe を起動していたため、同じ名前の実行ファイルが
	カレントパス（編集中のファイルパス）などに存在していた場合には、こちらが起動していた。
	この場合でも Windows 標準の explorer が起動するように、Windows ディレクトリのパスを指定して explorer.exe を起動するように変更した。

* 正規表現検索に \ のみ指定すると、エラーダイアログが２度表示されてしまう 不具合を修正  
	「SVN Rev.3609, 3610: Fix: 不正な正規表現検索で不要なメッセージが出る」に相当する修正。
	検索ダイアログを開き、「正規表現」にチェックを入れた状態で「条件」に「\」と入力し、検索を実行すると、「unmatched separator」、
	「検索条件を指定してください。」と２度、エラーダイアログが表示されていたため、「unmatched separator」のダイアログのみ表示されるように
	変更されている。
	また、置換ダイアログも同じ動作だったため、同様に修正されている。

* 検索、置換、Grepダイアログ、および、メインメニュー再設定時のハンドルリーク不具合を修正  
	「SVN Rev.3602: Fix: フォント、メニューのハンドルリークの修正」に相当する修正。以下のハンドルリークが修正されている。  
	- 検索、置換、Grepダイアログを開いて閉じた際のフォントハンドルリーク
	- メインメニュー再設定時のハンドルリーク

	また、upatchid:764 には「設定のフォントラベルのハンドルリーク修正」も行ったとの記述があり、以下の操作に関する処理が
	修正されているが、修正の前後で差異は見受けられなかった。  
	- 共通設定 -> 支援 タブ -> キーワードヘルプ -> フォント変更
	- 共通設定 -> タブバー タブ -> タブの概観 -> フォント変更

---
### 2014/04/01 - Ver.2014.4.1.16

* Command: 「Explorer 起動」機能を追加  
	キー割り当ての「その他」種別に、「Explorer 起動」を追加。
	この機能は、編集中ファイルの格納されたフォルダウィンドウを起動する。
	デフォルトで Ctrl + Shift + F キーにバインド。

* 行番号の最少幅を 2 桁から 4 桁に変更  
	ファイルの総行数によって、横方向の表示位置が変わらないように。
	また、1000 万行で表示がロールオーバーするように変更。
	（このあたりが動作確認できる上限であるため。）

* 「指定行へジャンプ」ダイアログの機能改善  
	- テキストボックスの幅を広げて、入力可能桁数を増加させた
	- テキストボックスに数値のみ入力できるように変更
	- スピンコントロールの上限値を 1000 万行に
	- 0 行目を指定した場合は、不正値が入力された扱いに変更
	- 不正値が入力された場合に、先頭行にジャンプしてしまう不具合を修正

* Java 強調キーワードを別セットで定義するように変更  
	import と package、基本型とラッパークラスを別セットで登録。

* C/C++ のソースコードを編集中にアウトライン解析を行うと異常終了する場合があった不具合を修正  
	サクラエディタ向けの以下の修正パッチを適用。  
	http://sourceforge.net/p/sakura-editor/patchunicode/786/  
	上記パッチにより、以下の不具合が修正されている。  
	- wcsncat() 使用方法の誤りによるバッファーオーバーラン
	- 文字列長の更新漏れによるバッファーオーバーラン

* C/C++ の不完全なソースコードを編集中にアウトライン解析を行うと異常終了する不具合を修正  
	以下のような記述途中の不完全なソースコードを解析すると、不正番地アクセスにより異常終了してしまう。

        template <> unsigned long testFunc<int (int arg)
        {
            printf("testFunc() : %d\n", arg);
        }

	関数名の解析が正常に終了していない場合には、関数名の登録処理を迂回するように修正した。

* コントロールプロセス、および、新しいエディタプロセスが起動するまでのタイムアウトを 10 秒から 60 秒に変更  
	PC の負荷が高い状態（ウィルスチェック中など）でタグジャンプを行って新しいウィンドウを開くと
	「プロセスの起動に失敗しました。」というダイアログが表示される場合がある。
	対策として、プロセス起動完了までのタイムアウトを延ばして暫定対策とする。
	また、初回起動時に生成されるコントロールプロセスの起動待ちタイムアウトも 10 秒から 60 秒に変更した。

	関連:  
		Request/455: コントロールプロセスの起動に失敗した旨のメッセージが表示された
		場合、［OK］ボタンを押下しなくてもサクラエディタを起動して欲しい。
		(http://sakura.qp.land.to/?Request%2F455)

* デバッグビルド版の実行ファイルが起動できない不具合を修正  
	サクラエディタの以下のチェンジセットを反映。  
	- SVN Rev.3133: Fix: シングルトンのアサートに引っかかる

* 実行ファイルのプロパティ「製品バージョン」の表記を、カンマ区切りからピリオドに変更  
	「SVN Rev.3608: Fix: 一部のバージョン表記がカンマ区切りで表示される」に相当する変更。

---
### 2014/03/01 - Ver.2014.03.01.15

* C/C++ プリプロセッサコメント (#if 1 ～ #else ～ #endif) のカラーリングに対応  
	\#if 1 ～ #else ～ #endif 中の #else ～ #endif 部分をプリプロセッサコメントと判定してカラーリングを行うように変更。
	ただし、プリプロセッサのネストは未対応です。

* カラーリング処理の速度改善  
	「Rev.ba0bb4663620 C/C++ プリプロセッサコメント（#if 0）のコメントネスト対応」以降、暫定実装であったカラーリングの
	終了判定を見なおして、編集時の動作速度を改善。

* 「各種モードの取り消し」機能の実行時に「検索マークの切り替え（クリア）」も行うように変更  
	ESC キー押下時に、検索結果のハイライト表示をクリアするため。

* キーバインドのデフォルトを変更  
	* Ctrl + Shift + A : TortoiseHg annotate 起動

* About ダイアログ  
	- サクラエディタのバージョン表記、コピーライトの年を修正
	- ビルドに使用した Visual Sutdio のバージョン表記を追加

* Build: Windows XP 向けのバイナリ生成用に VC2008 のプロジェクトファイルを更新  

---
### 2014/02/01 - Ver.2014.02.01.14

* TortoiseHg の annotate ウィンドウを起動する機能を追加  
	キー割り当ての「その他」種別に、「TortoiseHg annotate 起動」機能を追加。
	この機能は、外部コマンド実行で "thg.exe annotate \$F --line \$y" を指定した場合と同じ動作を行います。
	TortoiseHg Ver.2.10 以降のバージョンを使用してください。(Ver.2.10.2 で動作を確認しています。）

* サクラエディタ Ver.2.1.1.0 - SVN Rev.3593 までの不具合修正を一部適用  
	容易にマージ可能だったものに関して、いくつか適用しています。影響がありそうな修正は、以下の通りです。  
	- SVN Rev.3283: タブバーの「閉じるボタン」が ON の場合に、タブバーのフォント指定が効かない
	- SVN Rev.3528: カーソル形状が MS-DOS 風の場合に、挿入/上書きを切り替えるとカーソル表示がおかしくなる
	- SVN Rev.3545: タブを閉じた後、直前にアクティブだったタブを表示するように変更

---
### 2014/01/01 - Ver.2014.01.01.13

* ビルド環境を「Visual C++ 2008 Express」から 「Visual Studio Express 2013 for Windows Desktop」に変更  
	プラットフォーム ツールセットは v120 を使用しているため、このバージョン以降の紅桜は Windows Vista
	以降の OS でのみ動作します。（Windows XP では動作しません。）

* アプリケーションにマニフェストを埋め込むように変更  
	マニフェストファイルがない状態でも、Windows XP 以降の UI スタイルになるように。

* ビルドオプションを「サイズ優先」から「速度優先」に変更

---
### 2013/11/01 - Ver.2013.11.01.12

* ダイレクトタグジャンプ: ファイル名によるタグジャンプに対応  
	タグファイル生成時に「ファイル先頭行のエントリ」を出力するように変更。
	(ctags のオプションに "--extra=+f" を追加)
	「ダイレクトタグジャンプ」ダイアログの「種類」に file を追加。

* 共通設定: タグファイル作成時の「サブフォルダも対象にする」チェックボックスのデフォルトを有効に変更  

* 共通設定: タブバー「閉じるボタン」のデフォルトを有効に変更  

---
### 2013/08/01 - Ver.2013.08.01.11

* キーバインドのデフォルトを変更  
	- F12 : ブックマークの一覧
	- Ctrl + PgDn : 次のウィンドウ
	- Ctrl + PgUp : 前のウィンドウ
	- Alt + N : 次のウィンドウ
	- Alt + P : 前のウィンドウ
	- Ctrl + Shift + C : ファイルの最後に移動
	- Ctrl + Shift + R : ファイルの先頭に移動
	- Ctrl + Shift + T : 行末まで削除(改行単位)
	- Ctrl + Shift + V : 行頭まで削除(改行単位)  
	RED2 の Ctrl キーのうち、割り当てできていなかった機能を Ctrl + Shift に割り当てました。

* 表示色のデフォルトを変更  
	URL: Blue -> Deep Periwinkle に変更。  
	(黒地の青は見づらかったので、薄い青に変更しました。)

* アプリケーションのアイコンを変更  
	アプリケーションのアイコンを Windows Vista Generic Document ベースのアイコンに変更しました。  
	（Windows8 では関連付けしたファイルがすべてアプリケーションのアイコンになってしまうため、
	紅桜の情けないアイコンが大量に表示されていました。）  
	ついでに Grep ウィンドウのアイコンも Vista のアイコンに変更しました。

* サクラエディタ SVN Rev.3115 までの変更を反映  
	サクラエディタ Ver.2.1.0.0 - SVN Rev.3241 までのチェンジセットのうち、容易にマージ可能だったものに関しては、
	いくつか適用しています。
	未反映のチャンジセットに関しては 0.0.0.9 と同様です。

---
### 2013/07/01 - Ver.2013.07.01.10

* タグファイル作成 ダイアログ: タグ作成ファルダの初期値に VCS リポジトリルートを設定するように変更  

* タグファイル作成 ダイアログ: "C/C++ prototype, external and forward declarations を追加" チェックボックスを追加  
	チェックボックスを ON にすると、C 言語のプロトタイプ宣言、外部宣言、前置宣言もタグに追加されます。  
	(ctags のオプションに "--c-kinds=+px" を追加)

* サクラエディタ Ver.2.0.8.1 - SVN Rev.3019 までの変更を反映  
	未反映のチャンジセットに関しては 0.0.0.9 と同様です。

---
### 2013/06/01 - Ver 0.0.0.9

* Grep 時にバイナリファイルを除外するように変更  
	ファイルの先頭から 1KB に 0x00 (1byte) が含まれていた場合には、バイナリファイルと判断して Grep の対象から除外する。  
	制限事項:  
	この判定方法では UTF16 形式のファイルがバイナリファイルとして判断され、除外されてしまう。

* C/C++ 強調キーワードを追加  
	* 不足していたキーワードや、よく使うキーワードを追加
	* 定数キーワードを別グループで登録  
	サクラエディタ SVN Rev.2999 の代わりに、個人的に使用するキーワードを追加しました。

* タイプ別設定: アセンブラ: GNU Assembler 対応  
	* Assembler ファイルの拡張子に ".s" を追加
	* GNU Assembler、C/C++互換プリプロセッサのキーワードを追加して、カラーリングを有効に

* リポジトリ clone 直後の状態でビルドできない不具合を修正  
	svnrev.h が存在しない場合にビルドに失敗していたため、参照しないよう修正しました。

* サクラエディタ SVN Rev.3009 までの変更を反映  
	未反映チャンジセットの増分は以下の通りです。  
	- SVN Rev.2868: 「TAB矢印表示の仕様変更」と「TAB矢印の太字が反映されない」不具合の修正
	- SVN Rev.2999: Imp: C/C++キーワード更新

---
### 2013/05/01 - Ver 0.0.0.8

* Grep 時に ".bzr", ".git", ".hg", ".svn" フォルダを除外するように変更  

* Grep 時に VCS リポジトリルートを検索対象ファルダの初期値に設定する機能を追加  
	Grep ダイアログの「フォルダの初期値をカレントフォルダにする」がチェックされている場合に、
	編集中のファイルが格納されたディレクトリを起点に、親ディレクトリ方向に ".bzr", ".git", ".hg", ".svn"
	ディレクトリを検索して、これらのディレクトリが発見されたフォルダを、検索対象の初期値として設定する機能を追加。
	VCS リポジトリが発見できなかった場合には、今までどおり、編集中のファイルが格納されたディレクトリを設定する。

* C/C++ プリプロセッサコメント（#if 0）の判定において、同一行内で完結しているコメントアウト部分は無視するように変更  
	以下のように、コメント内に #if 0 が記述されている場合には無視できるように対応。

        \#if 0
            \#if 0
                \#if 0
                \#endif /* #if 0 */
            \#endif // #if 0
        \#endif

* \#1: 「キー割り当て」のインポートを行うと暴走してしまう 不具合を修正  
	サクラエディタの以下の変更によるデグレード。  
	- Rev.4d6bb991fa10 Keep: CShareData\_IOクラス変更(CommonSetting\_KeiBind関連)

	サクラエディタ Ver.2.0.7.1 の方では修正されているようなので、紅桜で取り込めていない
	以下の変更で修正されていたものと思われる。  
	- SVN Rev.2505: Fix: 古いキー割り当て設定をImportすると誤動作する
	- SVN Rev.2511: Imp: キーコードからコマンドを検索する時のループを除去
	- SVN Rev.2530: Fix: Importしたキーの名前が内部のバッファサイズより長いとメモリ破壊する

	紅桜のキー定義ファイルはサクラエディタと互換性がない（キー定義数が異なる）ため、
	キー定義数の異なるファイルはインポートできないように変更した。
	付随して、旧形式（？）のキー定義ファイルのインポート処理も削除した。

* 共通設定: GREP フォルダの初期値をカレントフォルダにする: 有効に変更  

* 共通設定: カーソル位置の文字列をデフォルトの検索文字列にする: 無効に変更  

* タイプ別設定: 「入力補完 強調キーワード」のデフォルトを有効に変更  

* 「タグジャンプ」ダイアログの縦幅を拡張  
	ダイアログのリサイズ対応（サクラエディタ Rev.2754）により、ウィンドウのスタイルが変更になった影響で
	「サイズ変更グリップ」と「キャンセルボタン」が重なって描画されていたため。

* About ダイアログ: バージョン表示に「Mercurial タグ + 相対リビジョン」を使用するように変更  

* サクラエディタ Ver.2.0.7.1 - SVN Rev.2836 までの変更を反映  
	未反映のチャンジセットに関しては 0.0.0.7 と同様です。

---
### 2013/04/01 - Ver 0.0.0.7

* サクラエディタ Ver.2.0.6.0 - SVN Rev.2743 までの変更を反映  
	未反映チャンジセットの増分は以下の通りです。  
	- SVN Rev.2643: Keep: Windows2000以降サポート  
	紅桜は WindowsXP 以降をサポートのため。

---
### 2013/03/01 - Ver 0.0.0.6

* サクラエディタ SVN Rev.2614 までの変更を反映  
	未反映のチャンジセットに関しては 0.0.0.4 と同様です。

---
### 2013/02/15 - Ver 0.0.0.5

* サクラエディタ SVN Rev.2563 までの変更を反映  
	SVN Rev.2544: 「強調キーワードによる補完機能」を使用したかったため更新しました。  
	未反映のチャンジセットに関しては 0.0.0.4 と同様です。

---
### 2013/02/01 - Ver 0.0.0.4

* C/C++ プリプロセッサによるコメントのネスト対応にて #ifndef の判定が漏れていた不具合を修正  

* サクラエディタ SVN Rev.2535 までの変更を反映  
	ただし、以下の変更に関しては、変更が衝突していたため未反映です。
	* SVN Rev.2505: Fix: 古いキー割り当て設定をImportすると誤動作する
	* SVN Rev.2511: Imp: キーコードからコマンドを検索する時のループを除去
	* SVN Rev.2530: Fix: Importしたキーの名前が内部のバッファサイズより長いとメモリ破壊する

---
### 2013/01/01 - Ver 0.0.0.3

* アプリケーションのアイコンを変更  

* C/C++ プリプロセッサによるコメントのネスト対応  
	\#if 0 ～ #endif がネストした状態でも正しくカラーリングできるよう改良。
	ただし、暫定実装の部分が残っているため、カラーリング処理の速度は低下しているはずです。

* タイプ別設定: 「行の間隔」のデフォルトを 1 -> 0 に変更  

* １行の最大桁数を 10240 -> 1024 に変更  

* サクラエディタ SVN Rev.2513 までの変更を反映  
	ただし、以下の変更に関しては、変更が衝突していたため未反映です。
	* SVN Rev.2505: Fix: 古いキー割り当て設定をImportすると誤動作する
	* SVN Rev.2511: Imp: キーコードからコマンドを検索する時のループを除去

---
### 2012/12/01 - Ver 0.0.0.2

* ルーラーを 8 文字単位で表示するように変更  

* 検索 ダイアログ:「見つからないときにメッセージを表示」チェックボックスを削除  

* ジャンプ ダイアログ: PL/SQL 関連の機能を削除  

* バージョン情報 ダイアログ: コントロールの配置を調整  

* 検索や GREP 時の「英大文字と小文字を区別する」のデフォルトを「有効」に変更  

* 共通設定: 「ファイルの排他制御」のデフォルト値を「上書きを禁止する」から「しない」に変更  

* 共通設定: GREP 結果のリアルタイム表示を「有効」に変更  

* 共通設定: ウィンドウ非アクティブ時のタイトルバー表示を、ウィンドウアクティブ時と同一に  

* タイプ別設定: C/C++, Java のスマートインデントのデフォルトを「無効」に変更  

* デフォルトの表示色を RED2 ライクな配色に変更  
	* ルーラーの背景色をグリーンに
	* TAB を暗いブルーに
	* 改行記号をイエローに
	* 行番号をグレーに

* サクラエディタ SVN Rev.2498 までの変更を反映  

---
### 2012/11/01 - Ver 0.0.0.1 (サクラエディタ Ver.2.0.5.0 からの変更点)

* 背景色のデフォルトを「黒地」ベースに変更  

* キーバインドのデフォルトを RED2 ライクなバインドに変更  
	ただし、Windows 標準機能に関する CTRL + Z, X, C, V など、サクラエディタのキーバインドを継承している部分も多いです。
	参考までに、RED2 とキーバインドが異なる CTRL キーのバインドは以下のとおりです。

	|キー       |RED2                      |紅桜             |
	|-----------|--------------------------|-----------------|
	|CTRL + C   |ファイルの最後にジャンプ  |コピー           |
	|CTRL + F   |１語先（右）に進む        |検索ダイアログ   |
	|CTRL + G   |カーソル上の文字を削除    |Grep ダイアログ  |
	|CTRL + H   |カーソル直前の文字を削除  |未割り当て       |
	|CTRL + J   |大文字小文字変換          |指定行へジャンプ |
	|CTRL + M   |改行入力                  |ブックマーク     |
	|CTRL + O   |改行・タブ表示切替        |アウトライン表示 |
	|CTRL + R   |ファイル先頭にジャンプ    |置換ダイアログ   |
	|CTRL + S   |カーソル左                |上書き保存       |
	|CTRL + V   |カーソルから左の文字を削除|貼り付け         |
	|CTRL + X   |カーソル下                |切り取り         |
	|CTRL + Z   |拡張コントロールキー      |元に戻す         |
	|CTRL + E   |カーソル上                |ウィンドウ一覧   |
	|CTRL + D   |カーソル右                |単語削除         |

	※ CTRL + I は未割り当て状態ですが、 TAB が入力されます  

	RED2 の F1 ～ F4 メニューに相当する機能は、カスタムメニューで実現しています。コントロールキーのバインドされて
	いないファイル先頭へのジャンプは F2 - T, ファイル末尾へのジャンプは F2 - B で代用してください。

* 「行頭に移動(折り返し単位)」のデフォルト動作を、空白を無視して移動するように変更  
	サクラエディタは「行頭に移動(折り返し単位)」でカーソルを移動する場合に、行頭のホワイトスペース位置に移動して
	いましたが、RED2 と同様、行頭に移動するように変更しました。（紅桜では SHIFT + ← にバインド）

* 「改行」機能を追加  
	RED2 では CTRL + M にバインドされていた機能ですが、紅桜の CTRL + M はサクラエディタと同様「マーク」機能のままです。
	RED2 と同じキーバインドを実現したい場合に使用してください。

* 「単語取り込み」機能を追加  
	実装はしたものの、デフォルトキーにはバインドされていません。

* 「現在位置～単語末尾の文字で検索」機能を追加  
	RED2 で CTRL + L にバインドされていた「カーソル位置文字列を検索バッファに取り込む」機能。RED2 から機能を拡張して
	おり、検索バッファに取り込むと同時に検索を行います。RED2 と同様、連続実行することで、後続の単語を追加して
	再検索を行います。デフォルトでは ALT + L にバインド。

* 「現在位置の単語で検索」機能を追加  
	上記の「現在位置～単語末尾の文字で検索」と同様の機能ですが、サクラエディタの単語取り込みと同様、カーソルより
	前方向に単語区切りを探して単語を取り込みます。連続実行することで、後続の単語を追加して再検索を行います。
	デフォルトでは CTRL + L にバインド。

* ダブルクリックに「現在位置の単語で検索」機能をバインド  
	上記の機能をダブルクリックにもバインドしていますので、マウスのみで単語検索を行うことができます。

* 単語取り込み時の文字種別に「括弧」を追加  
	括弧は ( ) { } [ ] の 6 文字で、これらの括弧は常に単語の区切りとして認識されます。

* C/C++ ソース編集時に、プリプロセッサを用いたコメントアウトが正しくカラーリングできない問題に対応  
	サクラエディタでは、ブロックコメントのデフォルトに #if 0 と #endif を定義して、プリプロセッサ コメントの
	カラーリングを実現していたため、途中に #else や #elif などが記述されていても、 #endif までコメントとして
	カラーリングされていました。
	対策として、 #if 0 をコメント開始、#endif、#else、#elif をコメント終了として判定を行う処理を追加して、
	カラーリングを行うように変更しています。

* 色指定に「C/C++ プリプロセッサコメント」項目を追加  
	コメントとは別の色を設定可能です。デフォルト色はグレー。

* 挿入モード時の DOS タイプカーソルの高さを、半分から ÷ 2.6 に変更（少し低くした）  
	半分の高さだと "-" の上にカーソルがある場合に文字があるのか無いのか、わからなくなってしまうため。

* 「検索／置換」の対象文字列が見つからなかった場合のダイアログ表示を削除  
	いちいちダイアログが開くのは煩わしかったので削除しました。

* キーボードの「Pause」キーを使用可能に変更  
	ノート PC など「Home」キーが存在しない機種で、代替キーとして使用できるように。デフォルトのキーバインドは「Home」
	と同様、ウィンドウの切り替えに設定されています。

* ウィンドウの「最大化」機能を追加  
	WMed32 と同様、デフォルトでは F12 にバインド。

* C/C++ 強調キーワードのデフォルト定義を３種類に分割  
	プリプロセッサ、データタイプを別々に定義。

* タイプ別設定のうち、個人的に使用頻度の低い設定を削除  
	HTML, SQL, COBOL, AWK, DOS Batch, Pascal, TEX, Perl, VB, Rich Text を削除

* その他、各種デフォルト値の変更  
