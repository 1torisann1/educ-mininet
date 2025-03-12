#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8888
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "使用法: " << argv[0] << " <サーバIPアドレス> \n";
        return -1;
    }

    const char* server_ip = argv[1];

    // UDPソケット作成
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        std::cerr << "ソケット作成に失敗しました\n";
        return -1;
    }

    // サーバアドレス設定
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // メッセージ送信ループ
    std::string message;
    char buffer[BUFFER_SIZE];

    while (true) {
        std::cout << "送信するメッセージを入力してください（終了するには'q'を入力）: ";
        std::getline(std::cin, message);
        if (message == "q") {
            break;
        }

        // サーバにデータを送信
        int bytes_sent = sendto(client_socket, message.c_str(), message.length(), 0,
                                (sockaddr*)&server_addr, sizeof(server_addr));
        if (bytes_sent < 0) {
            std::cerr << "データ送信に失敗しました\n";
            break;
        }
    }

    // ソケットを閉じる
    close(client_socket);
    return 0;
}
