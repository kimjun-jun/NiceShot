# NiceShot(3D作品)
この度は[NiceShot]をお手に取っていただき誠にありがとうございます。
この作品に対するコメントを明記します。

フォルダ構成：

|-cpp(ソースファイル)

|-h(ヘッダーファイル)

|-project(ソリューションファイル)

|-date-

      |-BGM
      
      |-MODEL

      |-SE

      |-TEXTURE

      |-地面メッシュ分割(4分木分割確認データ)
      
実行環境：
Windows10

制作環境：
C/C++ Windows10 VisualStudio2017 DirextX9

ゲーム説明：


操作説明：


制作者：
木村純

制作人数：
一人

制作期間：

約1ヵ月 ver1.0 2020/1～2020/2　ゲームの形になり始めたころ。地形の自動生成やモーフィング、レイキャストなど実装。

約2ヵ月 ver1.1 2020/2～2020/4　操作性や高速化など実装。

約4ヵ月 ver2.0 2020/4～2020/8　通信機能やモーションブレンドなど実験成功。ライブラリの自作や弾道予測ラインの実装。

約1ヵ月 ver2.1 2020/8～2020/9　姿勢制御の球面線形補間実装。若干変なとこがあるがおおよそ問題なし。

予定    ver3.0 2020/9～現在はここでござる　　－－－＞システムやゲームっぽさの向上。特徴がバラバラのモデル、戦車の動き、ゲームシステムを再設計して実装予定。

担当範囲：
フォルダ内のすべてのプログラムファイル

制作した理由：
初めての3D作品制作で、変化というワードをもとに企画しました。
地形の自動生成やモーフィング処理によるプレイヤーモデルの変形を実装する事ができました。
HitBallTargetの時にテストプレイをしてもらい肌で感じたのは、楽しんでいる気分を他の人とも共感しながら盛り上がれるような作品を作りたいと考え4人対戦できるようにしました。

作品動画：
3月末 ver1.1
https://www.youtube.com/watch?v=Rg5bdDfClKc&t=0s

7月末 ver2.0
https://www.youtube.com/watch?v=oA6iKpiPQfM&t=0s

今後の改善点拡張点：

1 通信機能によるオンライン対戦化(winsockを利用した通信プログラムを勉強中)

2 当たり判定の高速化(4分木による地面との当たり判定高速化を実装済み/2020/3/20)

3 ホーミングを調整し弾道予測ラインを入れる(弾道予測ライン実装済み/2020/07/26)ホーミングは無くしていいかも。
 
4 頂点間移動時に球面補間等を用いてカクカクを減らすーー＞＞(球面線形補間実装済み/2020/09/07)プレイヤーと3角ポリゴン3辺の内、複数にまたがっ超えるときの処理が甘い。

テストプレイ報告(バグやシステム面での話)/2020/07/27 追加

1　全体的におじゃまアイテムの時間が長い(300frameから150frameに変更)

2　残りの人数で出現するアイテムの確率を変えたほうがいい

3　時間が長引くことがあるため、残り1VS1になったら制限時間を付けるなどの工夫が欲しい

4　大砲の一撃が弱い。戦車なら動きが遅いが攻撃力は高いなどのバランスがあるといいーー＞＞戦車の種類を増やしてスピードタイプ、パワータイプなど好きなタイプを選べてプレイできると面白いかも

5　戦車のスピードを遅くしたほうがいい。早いからジグザグに動くだけで簡単に避けれる。ーー＞＞4と同じ対応したらいいかも

6　バレット被弾成功時のリワードをわかりやすくしたほうがいい。ライフのビルボードが割れて飛んでいくみたいな

実感として、ゲーム性を高めるための仕様を考えたほうがいいかもーー＞＞戦車の体力やスピード、移動方法、移動の仕方をしっかり練ったほうがいいかも。

ひとこと：目的であった友達複数人でプレイしたときに盛り上がれるようなゲームを制作することができたと個人的に思っています。
ですが、まだまだ改善の余地がありもっと面白くできそう。
今後もスキルに磨きをかけながら良い作品を作っていきたい。
テストプレイに関わってくれた方々ありがとうございます。