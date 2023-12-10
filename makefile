cc=gcc
all=client server
vpath %.c src
d_lib=./lib
d_include=./include
vpath %.h src
vpath %.o src
client:client.c 
	$(cc) -g  $^ -o $@ ./src/socket/socket.o  rio.o  -I $(d_include) 
server:server.c 
	$(cc) -g  $^ -o $@ ./src/socket/socket.o -I $(d_include)
clean:
	rm client server

