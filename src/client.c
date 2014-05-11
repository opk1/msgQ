#include "msgQ.h"

/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   client
--
-- DATE:       February 17, 2014
--
-- REVISIONS:  February 18, 2014 - takes in file to request
--                               - receives ok
--             February 19, 2014 - treads from message queue in thread
--                               - closes upon completion
--
-- DESIGNER:   Damien Sathanielle
--
-- PROGRAMMER: Damien Sathanielle
--
-- INTERFACE:  void client(char *filename, int priority)
--
-- RETURNS:    void.
--
-- NOTES:      A client requests a file from the server with a specified 
--             priority (-20  to 20). The client then runs a thread to get
--             the data from the message queue. Once the entire file has
--             been sent, the client receives a notification from the server
--             saying that it has finished sending data; the client exits.
--------------------------------------------------------------------------------------------------*/
void client(char *filename, int priority)
{
    int qid;
    pid_t pid;
    pthread_t rcvmsg;
    msg *message;
    
    message = (msg *)malloc(sizeof(msg));
    
    if((qid = open_queue(MSGKEY)) == -1)
    {
    	perror("opening queue");
    	exit(1);
    }
    /* set up request message */
    message->mtype = 1;
    message->priority = priority;
    message->pid = getpid();
    strcpy(message->buffer, filename);

    send_message(qid, message);
    /* read server response to request */
    pthread_create(&rcvmsg, NULL, client_thread, (void*)&qid);
    pthread_join(rcvmsg, 0);
    free(message);
}

/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   client_thread
--
-- DATE:       February 19, 2014
--
-- REVISIONS:  none
--
-- DESIGNER:   Damien Sathanielle
--
-- PROGRAMMER: Damien Sathanielle
--
-- INTERFACE:  void * client_thread(void *param)
--
-- RETURNS:    void.
--
-- NOTES:      This thread gets data from the message queue until the
--             server signals completion by setting the message status
--             to 0.
--------------------------------------------------------------------------------------------------*/
void * client_thread(void *param)
{
    int pid;
    int qid;
    msg *message;
    
    pid = getpid();
    qid = *(int*)param;
    message = (msg *)malloc(sizeof(msg));
    
    while(1)
    {
	   if(read_message(qid, pid, message) >= 0)
	{
	    /* check status if there is more data to read */
	    if(message->status)
	    {
		  printf("%s", message->buffer);
	    }else
		  break;
	}
    }
    free(message);
}
