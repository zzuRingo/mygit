OBJS:=$(wildcard *.c)
main:${OBJS}
	gcc -g $^ -o $@ -I /usr/include/mysql/ -lmysqlclient -pthread -lcrypt
clean:
	rm main
