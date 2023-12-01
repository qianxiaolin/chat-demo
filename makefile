cc=gcc
client:client.c proto.h
	$(cc) -g  $^ -o $@ 
server:server.c proto.h
	$(cc) -g $^ -o $@ -g



