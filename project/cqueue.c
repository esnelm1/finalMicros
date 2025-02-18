/* Circular Queue */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "cqueue.h"

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static unsigned char buffer[QSIZE], buffer_2[QSIZE];   // storage for circular queue (static)
static unsigned int news,news_2;	    	  // How many "news" are remaining in Queue
static unsigned char *pin, *pout,*pin_2, *pout_2;     // input and output pointers

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
    pin_2  = buffer_2;
    pout_2 = buffer_2;
    news_2 = 0;
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


// AGREGADO -----------------------------------------------------------------------

/* AGREGADO: Funciones para transmisión (TX) */

unsigned char PushQueue_TX(unsigned char data) {
    if (news_2 > QSIZE - 1) {       // Verifica overflow del buffer TX
        return QFULL;             // Se alcanzó la capacidad máxima
    }
    *pin_2++ = data;              // Almacena el dato en el buffer TX
    news_2++;                     // Incrementa el contador del buffer TX (corregido)
    if (pin_2 == buffer_2 + QSIZE) // Si se excede el final del buffer, se reinicia el puntero
        pin_2 = buffer_2;
    return QOK;                   // Operación exitosa
}

unsigned char PullQueue_TX(unsigned char *pdata) {
    if (news_2 == 0)              // Si no hay datos en el buffer TX
        return QEMPTY;            // Retorna que el buffer está vacío

    *pdata = *pout_2++;           // Extrae el dato y lo asigna al puntero
    news_2--;                     // Decrementa el contador de datos en el buffer TX
    if (pout_2 == buffer_2 + QSIZE) // Si se excede el final del buffer, se reinicia el puntero
        pout_2 = buffer_2;
    return QOK;                   // Operación exitosa
}

unsigned int QueueStatus_TX(void) {
    return news_2;                // Retorna la cantidad de bytes almacenados en el buffer TX
}
