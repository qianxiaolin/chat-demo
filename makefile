cc=gcc
client:client.c proto.h
	$(cc) $^ -o $@
server:server.c proto.h
	$(cc) $^ -o $@



