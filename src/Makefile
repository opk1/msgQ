# 4981 Assignment 2

CC = gcc
CFLAGS = -g -l pthread

msgQ: msgQ.o qUtil.o client.o server.o
		$(CC) -o msgQ msgQ.o qUtil.o client.o server.o $(CFLAGS)

clean:
		rm -f *.o core.* msgQ

msgQ.o: msgQ.c msgQ.h
		$(CC) $(CFLAGS) -c msgQ.c
		
qUtil.o: qUtil.c
		$(CC) $(CFLAGS) -c qUtil.c
		
client.o: msgQ.c msgQ.h
		$(CC) $(CFLAGS) -c client.c
		
server.o: qUtil.c
		$(CC) $(CFLAGS) -c server.c


