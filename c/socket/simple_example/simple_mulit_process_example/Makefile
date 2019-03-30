CC = gcc -Wall -o
exe_server = ./simple_server/server
exe_client = ./simple_client/client

all: $(exe_server) $(exe_client)

obj_temp = ./simple_client/client.o ./simple_server/server.o

obj_client = ./simple_client/client.c
obj_server = ./simple_server/server.c


$(exe_server):$(obj_server)
	$(CC) $(exe_server) $(obj_server)

$(exe_client):$(obj_client)
	$(CC) $(exe_client) $(obj_client)


clean:
	rm -f $(obj_remove)
	rm -f $(exe_server)
	rm -f $(exe_client)
