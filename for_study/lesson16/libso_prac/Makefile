libmymath.so:add.o sub.o
	gcc -shared -o $@ $^
%.o:%.c
	gcc -fPIC -c $<

.PHONY:clean
clean:
	rm -rf *.o libmymath.so output

.PHONY:output 
output:
	mkdir mylib
	cp *.h mylib 
	cp libmymath.so mylib
