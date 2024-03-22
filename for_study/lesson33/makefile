.PHONY:all
all:select_server client

select_server:select_server.cpp
	g++ -o $@ $^ -std=c++11

client:client.cpp
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f select_server client