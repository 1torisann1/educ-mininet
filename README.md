# educ-mininet
西研新b4教育用　（TCP・UDP）

### 各ディレクトリの説明
- host/
  仮想ネットワークのクライアント、サーバで実行
  - TcpClient.cpp
    - 実行時にサーバIP指定
  - TcpServer.cpp
  - UdpClient.cpp
    - 実行時にサーバIP指定
  - UdpServer.cpp
  
- mininet/
  仮想ネットワークの起動
  - NodeClass.py
    - ホスト、スイッチの設定が書いてある
    - IPv4に対応
    - おまじない
  - TreeTopo.py
    - ツリー構造のネットワーク
    - クライアントとクラウドが同じネットワークにいる
    - 遅延、帯域幅を設定可能
    - <img width="552" alt="Image" src="https://github.com/user-attachments/assets/3d42e166-bf6b-463d-bf90-1dda6bb34fd1" />
    ```TreeTopo.py
    DELAY = '10ms' # リンク遅延
    BW = 10 #  帯域幅
    ```
  - TreeTopoWithNat.py
    - ツリー構造にNATを追加
    - 外部ネットワークへの接続可能
    - <img width="465" alt="Image" src="https://github.com/user-attachments/assets/1ed6d8b2-4030-4bbb-bded-ef5c30da3082" />

### 動かし方
- hostディレクトリ内でコンパイル
  ```
  cd host
  make all
  ```
- TreeTopo.pyを実行
  ```
  cd mininet
  sudo python3 TreeTopo.py
  ```
- CLIにてxtermを起動
  ```
  xterm h0 h1 ...
  ```
- Client, Serverのコード実行
  ```
  # サーバ側
  ../host/TcpServer
  ../host/UdpServer
  ```
  ```
  # クライアント側
  ../host/TcpClient [サーバIPアドレス]
  ../host/UdpClient [サーバIPアドレス]
  ```
