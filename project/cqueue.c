/* Circular Queue */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "cqueue.h"

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static unsigned char buffer[QSIZE];   // storage for circular queue (static)
static unsigned int news;	    	  // How many "news" are remaining in Queue
static unsigned char *pin, *pout;     // input and output pointers 	

/*******************************************************************************
 *******************************************************************************
 GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

//init function
void QueueInit(void) {
    pin  = buffer;	//set up pin, pout and "news" counter 	
    pout = buffer;
    news = 0;
}

//push data on queue
unsigned char PushQueue(unsigned char data) {
    if (news > QSIZE - 1)		//test for Queue overflow
    {
        return (QFULL);		// Queue capacity has reached
    }
    *pin++ = data;			// pull data
    news++;					// update "news" counter

    if (pin == buffer + QSIZE)	// if queue size is exceded reset pointer
        pin = buffer;

    return (QOK);			// inform Queue state
}

/*
 * PullQueue()
 * 
 Retrieve data from queue 
 Dont forget to call QueueStatus()
 before call this function
 *
 * 
 * Sample code
 *  
 * unsigned char answer; 
 * 
 *  while (QueueStatus())		// some news?
 {
 status = PullQueue(&answer); 	// if yes then get data from queue and store it at caller address
 
 }

 * 
 * 
 * 
 * 
 */

unsigned char PullQueue(unsigned char *pdata) {
    if (news == 0)		        // Just in case 
        return (QEMPTY);		// Error queue is empty 

    *pdata = *pout++;		    // pull data and store at caller address
    news--;				        // update "news" counter

    if (pout == buffer + QSIZE)	// Check for Queue boundaries
        pout = buffer;			// if queue size is exceded reset pointer
    return (QOK);				// return operation status 
}

// queue status
unsigned int QueueStatus(void) {
    return (news);			// Retrieve "news" counter, Queue is empty when null is returned		
}

/******************************************************************************/
