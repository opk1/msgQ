#ifndef _MSG_Q_H_
#define _MSG_Q_H_

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#define MAXMESSAGEDATA (4096-16)
#define REQUEST         1L

#define MSGKEY   (key_t)0x010

typedef struct{
    long mtype;            // Message type
    int priority;
    int status;            // flag if there is more data
                           // 0 no more data to send
			   // 1 more data to be sent
    char buffer[MAXMESSAGEDATA];
    pid_t pid;             // client process id
}msg;

void server();
void client(char*, int);

void * client_thread(void *);

FILE * open_file(char *);

int open_queue (key_t);
int remove_queue(int);
int send_message(int, msg *);
int read_message(int, long, msg *);

#endif
