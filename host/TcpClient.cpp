#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "使用法: " << argv[0] << " <サーバIPアドレス> \n";
        return -1;
    }

    const char* server_ip = argv[1];

    // ソケット作成
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "ソケット作成に失敗しました\n";
        return -1;
    }

    // ソケットをバインド
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(0); // 自動で利用可能なポートを割り当てる
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(client_socket, (sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        std::cerr << "ローカルアドレス " << " のバインドに失敗しました\n";
        close(client_socket);
        return -1;
    }

    std::cout << "ローカルアドレスにバインドしました\n";

    // サーバアドレス設定
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // サーバのポート番号
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // サーバに接続
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "サーバ " << server_ip << " への接続に失敗しました\n";
        close(client_socket);
        return -1;
    }
    std::cout << "サーバ " << server_ip << " に接続しました\n";

    // 通常のメッセージ送信処理
    std::string message;
    while (true) {
        std::cout << "送信するメッセージを入力してください（終了するには'q'を入力）: ";
        std::getline(std::cin, message);
        if (message == "q") {
            break;
        }
        int bytes_sent = send(client_socket, message.c_str(), message.length(), 0);
        if (bytes_sent < 0) {
            std::cerr << "データ送信に失敗しました\n";
            break;
        }
    }

    // ソケットを閉じる
    close(client_socket);
    return 0;
}
