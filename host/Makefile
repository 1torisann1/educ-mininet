CXX = g++
CXXFLAGS = -std=c++11

all: TcpServer TcpClient

TcpServer: TcpServer.cpp
	$(CXX) $(CXXFLAGS) TcpServer.cpp -o TcpServer

TcpClient: TcpClient.cpp
	$(CXX) $(CXXFLAGS) TcpClient.cpp -o TcpClient

server_multi: server_multi.cpp
	$(CXX) $(CXXFLAGS) server_multi.cpp -o server_multi

clean:
	rm -f TcpServer TcpClient
