#ifndef CQUEUE_H
#define CQUEUE_H

#define QSIZE  20	// Queue size


void QueueInit(void);
unsigned char PushQueue(unsigned char );
unsigned char PullQueue(unsigned char *);
unsigned int QueueStatus(void);

#define QEMPTY 2		// Queue is empty (no more data can be pulled)
#define QFULL  1		// Queue is full (no more data can be pushed)
#define QOK    0		// Queue Push was done succesfully 

#endif
