CC = gcc -o
SRC_SERVER = ./src/server.c 
SRC_CLIENT = ./src/client.c
TARGET_SERVER = ./app/server
TARGET_CLIENT = ./app/client

all:
	$(CC) $(TARGET_SERVER) $(SRC_SERVER) -lpthread
	$(CC) $(TARGET_CLIENT) $(SRC_CLIENT)  

clean:
	rm -rf ./src/server.o
	rm -rf ./app/server
	rm -rf ./src/client.o
	rm -rf ./app/client
	
