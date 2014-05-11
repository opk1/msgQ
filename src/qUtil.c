#include "msgQ.h"
/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   open_queue
--
-- INTERFACE:  int open_queue(key_t)
--
-- RETURNS:    message queue identifier
--
-- NOTES:      Wrapper function for opening message queue
--------------------------------------------------------------------------------------------------*/
int open_queue(key_t keyval)
{
    int qid;
    
    if((qid = msgget (keyval, IPC_CREAT | 0660)) == -1)
    {
	   return (-1);
    }
	
    return (qid);
}
/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   remove_queue
--
-- INTERFACE:  int remove_queue(int)
--
-- RETURNS:    0 on success
--
-- NOTES:      Wrapper function for removing message queue
--------------------------------------------------------------------------------------------------*/
int remove_queue(int qid)
{
    if(msgctl(qid, IPC_RMID, 0) == -1)
    {
	   return (-1);
    }
	
    return (0);
}
/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   send_message
--
-- INTERFACE:  int send_message(int, msg *)
--
-- RETURNS:    0 on success
--
-- NOTES:      Wrapper function for sending a message
--------------------------------------------------------------------------------------------------*/
int send_message(int msg_qid, msg *qbuf)
{
    int result;
    int length;
    /* The length is essentially the size of the structure minus sizeof(mtype) */
    length = sizeof (msg) - sizeof(long);
    
    if((result = msgsnd(msg_qid, qbuf, length, 0)) == -1)
    {
	   return (-1);
    }

    return (result);
}
/*-----------------------------------------------------------------------------------------------
-- FUNCTION:   read_message
--
-- INTERFACE:  int read_message(int, long, msg *)
--
-- RETURNS:    number of bytes read
--
-- NOTES:      Wrapper function for reading a message
--------------------------------------------------------------------------------------------------*/
int read_message(int qid, long type, msg *qbuf)
{
    int result;
    int length;
    /* The length is essentially the size of the structure minus sizeof(mtype) */
    length = sizeof(msg) - sizeof(long);
    
    if ((result = msgrcv(qid, qbuf, length, type, IPC_NOWAIT)) == -1)
    {
	   return(-1);
    }
	
    return (result);
}




