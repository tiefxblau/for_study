gdb_test:gdb_test.cpp
	g++ -o $@ $^ -g -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f gdb_test

.PHONY:modify
modify:
	vim gdb_test
