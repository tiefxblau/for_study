mysql:mysql_cpp.cpp
	g++ -o $@ $^ -std=c++11 -L../lib -lmysqlcppconn -D_GLIBCXX_USE_CXX11_ABI=0
.PHONY:clean
clean:
	rm -f mysql