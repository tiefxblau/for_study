libmymath.a:add.o sub.o
	ar -rc $@ $^
%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -rf *.o libmymath.a mylib
	
.PHONY:output
output:
	mkdir mylib
	cp *.h ./mylib
	cp libmymath.a ./mylib
