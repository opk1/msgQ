#include "msgQ.h"

/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   server
--
-- DATE:       February 17, 2014
--
-- REVISIONS:  February 18, 2014 - spawn each client as new child process
--                               - opens and sends requested file
--             February 19, 2014 - *clients in shared memory for priority usage
--                               - opens and sends requested file
--
-- DESIGNER:   Damien Sathanielle
--
-- PROGRAMMER: Damien Sathanielle
--
-- INTERFACE:  void server()
--
-- RETURNS:    void.
--
-- NOTES:      The server constantly listens for clients requesting a file.
--             This is done by waiting for messages with mtype REQUEST (1).
--             Once the server gets a request from the client it spawns a
--             process for each  client and sends messages with an mtype of
--             the client pid. Clients also specify a priority which is set
--             after the server creates a process for said client.
--------------------------------------------------------------------------------------------------*/
void server()
{
    int msq_id;
    int qid;
    int shmid;
    FILE *fp;
    
    /* shared memory to keep track of how many clients are connected */
    int *clients = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    msg *message = (msg *)malloc(sizeof(msg));
	
    if((qid = open_queue(MSGKEY)) == -1)
    {
	perror("opening queue");
	exit(1);
    }
    
    while(1)
    {
	if(read_message(qid, REQUEST, message) >= 0)
	{
	    /* create process for each client */
	    printf("Client %d connected with priority %d\n", (int)message->pid, message->priority);
	    switch(fork())
	    {
	    case -1:
		perror("fork call");
		exit(1);
	    case 0:
	        /* increment number of clients */
	        *clients =  *clients + 1;
		printf("%d clients connected\n", *clients);
	    
		fp = open_file(message->buffer);
		
		message->mtype = message->pid;

		/* set status to notify more data to send */
		message->status = 1;
		while(fread(message->buffer, sizeof(char), MAXMESSAGEDATA, fp))
		{
		    send_message(qid, message);
		    memset(message->buffer, 0, sizeof(message->buffer));
		    /* sleep depending on number of clients connected and priority */
		    usleep(message->priority * *clients * 100);
		}
		/* set status for no more data to send */
		message->status = 0;
		send_message(qid, message);
		printf("Client %d compete with priority %d\n", message->pid, message->priority);
		/* decrement number of clients */
		*clients = *clients - 1;
		fclose(fp);
		exit(1);
	    default:
		    break;
	    }
	}
    }	
}

/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   open_file
--
-- DATE:       February 18, 2014
--
-- REVISIONS:  none
--
-- DESIGNER:   Damien Sathanielle
--
-- PROGRAMMER: Damien Sathanielle
--
-- INTERFACE:  FILE * open_file(char *filename)
--
-- RETURNS:    pointer to opened file
--
-- NOTES:      opens a file for reading
--------------------------------------------------------------------------------------------------*/
FILE * open_file(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
	perror("error opening file\n");
	exit(1);
    }
    return fp;
}
