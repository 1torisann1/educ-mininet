#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>

#define TCP_SERVER_PORT 8080
#define BUFFER_SIZE 1024

int master_fd;
std::vector<int> client_sockets; // クライアントソケットのリスト
std::mutex client_mutex;         // 排他制御用

int init_socket() {
  struct sockaddr_in address;
  int opt = 1;

  // Creating socket file descriptor
  if ((master_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      std::cerr << "Socket failed" << std::endl;
      return -1;
  }

  if (setsockopt(master_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
      std::cerr << "setsockopt" << std::endl;
      return -1;
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  address.sin_port = htons(TCP_SERVER_PORT);

  // Forcefully attaching socket to the port 8080
  if (bind(master_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
      std::cerr << "bind failed" << std::endl;
      return -1;
  }

  if (listen(master_fd, 3) < 0) {
      std::cerr << "listen" << std::endl;
      return -1;
  }

  return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    struct sockaddr_in peer_address;
    socklen_t peer_address_len = sizeof(peer_address);

    if (getpeername(client_socket, (struct sockaddr *)&peer_address, &peer_address_len) == -1) {
        std::cerr << "Failed to get client info for socket: " << client_socket << std::endl;
        return;
    }

    std::string client_ip = inet_ntoa(peer_address.sin_addr);
    int client_port = ntohs(peer_address.sin_port);

    while (true) {
        // メッセージを受信
        ssize_t bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // 文字列の終端

            // メモリの0を'.'に変換
            for (ssize_t i = 0; i < bytes_read; ++i) {
                if (buffer[i] == '\0') {
                    buffer[i] = '.';
                }
            }

            std::cout << "Message from " << client_ip << ":" << client_port
                      << " - " << buffer << std::endl;
        } else if (bytes_read == 0) {
            std::cout << "Client disconnected: " << client_ip << ":" << client_port << std::endl;
            close(client_socket);

            // クライアントリストから削除
            {
                std::lock_guard<std::mutex> lock(client_mutex);
                auto it = std::remove(client_sockets.begin(), client_sockets.end(), client_socket);
                client_sockets.erase(it, client_sockets.end());
            }
            break;
        } else {
            // エラーが発生
            std::cerr << "recv failed for client: " << client_socket << std::endl;
            close(client_socket);
            break;
        }
    }
}

void run_server() {
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    std::cout << "Server is listening on port " << TCP_SERVER_PORT << std::endl;

    while (true) {
        // 新しいクライアント接続を受け入れ
        int client_socket = accept(master_fd, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            std::cerr << "accept failed" << std::endl;
            continue;
        }

        // クライアント情報を出力
        std::cout  << "New connection from "
                  << inet_ntoa(client_address.sin_addr) << ":"
                  << ntohs(client_address.sin_port) << std::endl;

        {
            std::lock_guard<std::mutex> lock(client_mutex);
            // クライアントソケットをリストに追加
            client_sockets.push_back(client_socket);
        }

        // スレッドでクライアントを処理
        std::thread client_thread(handle_client, client_socket);
        client_thread.detach(); // スレッドをデタッチして独立実行
    }
}

int main() {
    if (init_socket() < 0) {
        std::cerr << "Failed to initialize server" << std::endl;
        return 1;
    }

    run_server(); // サーバーを実行

    // 終了時にソケットをクローズ
    close(master_fd);
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        for (int client_socket : client_sockets) {
            close(client_socket);
        }
    }

    return 0;
}
