.PHONY:all
all:tcp_client tcp_server2

tcp_client:tcp_client.cpp
	g++ -o $@ $^ -std=c++11

tcp_server2:tcp_server2.cpp
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f tcp_client tcp_server2