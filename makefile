cc=gcc
all=client server
vpath %.c src
d_lib=./lib
d_include=./include
client:client.c 
	$(cc) -g  $^ -o $@ -L $(d_lib) -lio -I $(d_include)
server:server.c 
	$(cc) -g  $^ -o $@ -I $(d_include)
clean:
	rm client server

