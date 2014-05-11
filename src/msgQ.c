/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: msgQ.c - A client/server application demonstrating use of
--                       message queues
--
-- PROGRAM:     msgQ
--
-- FUNCTIONS:   void server();
--              void client(char*, int);
--              void * client_thread(void *);
--              FILE * open_file(char *);
--              int open_queue (key_t);
--              int remove_queue(int);
--              int send_message(int, msg *);
--              int read_message(int, long, msg *);
--
-- DATE:        February 17, 2014
--
-- REVISIONS:   (Date and Description)
--
-- DESIGNER:    Damien Sathanielle
--
-- PROGRAMMER:  Damien Sathanielle
--
-- NOTES: 
----------------------------------------------------------------------------------------------------------------------*/


#include "msgQ.h"

int main(int argc, char* argv[])
{
	int priority;
	char *filename;
	
	if(argc < 2)
	{
	    perror("Select to run server or client\n");
	    exit(1);
	}
	
	if(!strcmp(argv[1], "server"))
	{
	    server();
	}else if(!strcmp(argv[1], "client"))
	{
	    if((argc < 4) || (((atoi(argv[3])) > 20) || ((atoi(argv[3])) < -20)))
	    {
			perror("Usage: [filename] [priority(-20 to 20)]");
			exit(1);
	    }
	    client(argv[2], atoi(argv[3]));
	}
	
	return 0;
}
