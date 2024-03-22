.PHONY:all
all:exec_test myexec

myexec:myexec.c
	gcc -o $@ $^
exec_test:exec_test.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f exec_test myexec
