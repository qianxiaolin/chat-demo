cc=gcc
all=client server
client:client.c proto.h
	$(cc) -g  $^ -o $@ 
server:server.c proto.h
	$(cc) -g  $^ -o $@ 

clean:
	rm client server

