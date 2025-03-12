#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define UDP_SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main() {
    // UDPソケット作成
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        std::cerr << "ソケット作成に失敗しました\n";
        return -1;
    }

    // サーバアドレス設定
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // すべてのアドレスで受け入れる
    server_addr.sin_port = htons(UDP_SERVER_PORT);

    // ソケットをバインド
    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "バインドに失敗しました\n";
        close(server_socket);
        return -1;
    }

    std::cout << "UDPサーバがポート " << UDP_SERVER_PORT << " で待機中...\n";

    char buffer[BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        // クライアントからのメッセージを受信
        int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0,
                                      (sockaddr*)&client_addr, &client_len);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';

            // クライアント情報を取得
            std::string client_ip = inet_ntoa(client_addr.sin_addr);
            int client_port = ntohs(client_addr.sin_port);

            std::cout << "クライアント " << client_ip << ":" << client_port
                      << " からのメッセージ: " << buffer << std::endl;

            // メモリの0を'.'に変換
            for (int i = 0; i < bytes_received; ++i) {
                if (buffer[i] == '\0') {
                    buffer[i] = '.';
                }
            }
        }
    }

    // ソケットを閉じる
    close(server_socket);
    return 0;
}
