CXX = g++
CXXFLAGS = -std=c++11

all: TcpServer TcpClient UdpServer UdpClient

TcpServer: TcpServer.cpp
	$(CXX) $(CXXFLAGS) TcpServer.cpp -o TcpServer

TcpClient: TcpClient.cpp
	$(CXX) $(CXXFLAGS) TcpClient.cpp -o TcpClient

UdpClient: UdpClient.cpp
	$(CXX) $(CXXFLAGS) UdpClient.cpp -o UdpClient

UdpServer: UdpServer.cpp
	$(CXX) $(CXXFLAGS) UdpServer.cpp -o UdpServer

clean:
	rm -f TcpServer TcpClient UdpServer UdpClient
