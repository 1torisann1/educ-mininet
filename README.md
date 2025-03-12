# educ-mininet
西研新b4教育用　（TCP・UDP）

#### 各ディレクトリの説明
- host
  仮想ネットワークのクライアント、サーバで実行
  - TcpClient.cpp
  
- mininet
  仮想ネットワークの起動
  - NodeClass.py
    - ホスト、スイッチの設定が書いてある
    - IPv4に対応
    - おまじない
  - TreeTopo.py
    - ツリー構造のネットワーク
    - クライアントとクラウドが同じネットワークにいる
    - 遅延、帯域幅を設定可能
    ```TreeTopo.py
    DELAY = '10ms' # リンク遅延
    BW = 10 #  帯域幅
    ```
  - TreeTopoWithNat.py
    - ツリー構造にNATを追加
    - 外部ネットワークへの接続可能

#### 動かし方