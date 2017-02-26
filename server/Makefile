OBJS:=$(wildcard /mnt/hgfs/test/v3.0/server/src/*.c)
server:${OBJS}
	gcc -g $^ -o $@ -I /usr/include/mysql/ -I/mnt/hgfs/test/v3.0/server/include -lmysqlclient -pthread -lcrypt
	mkdir ./bin/server -p
	mv server ./bin/server/
clean:
	rm ./bin/server/server

