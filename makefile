cc=gcc
client=client.c proto.h
	$(cc) $^ -c $@
server=server.c proto.h
	$(cc) $^ -c $@



