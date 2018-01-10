server-client: server.c client.c

	gcc client.c -o client
	gcc -pthread server.c -o server
