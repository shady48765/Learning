CC = gcc -Wall
obj = ./server/server_process ./client/client_process
all:$(obj)

object_server = ./server/select_server.c    # don't create file.o
#object_server = ./server/select_server.o	# create file.o
object_client = ./client/select_client.c    # don't create file.o
#object_client = ./client/select_client.o   # create file.o

./server/server_process:$(object_server)
	$(CC) -o ./server/server_process $(object_server)
./client/client_process:$(object_client)
	$(CC) -o ./client/client_process $(object_client)



clean:
	rm -f ./server/*.o
	rm -f ./client/*.o
	rm -f ./server/*.sw?
	rm -f ./client/*.sw?
	rm -f $(obj)
	



