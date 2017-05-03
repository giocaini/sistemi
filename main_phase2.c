#include <const.h>
#include <types11.h>
#include <const11.h>

//Variabili del nucleo(vettori interrupt e trap)
HIDDEN state_t *newAreaSysBp   = (state_t *) SYSBK_NEWAREA;
HIDDEN state_t *newAreaPgmTrap = (state_t *) PGMTRAP_NEWAREA;
HIDDEN state_t *newAreaTLBTrap = (state_t *) TLB_NEWAREA;
HIDDEN state_t *newAreaInts    = (state_t *) INT_NEWAREA;
	


int main(){

	tcb_t *currentThread = NULL;

	LIST_HEAD(readyQueue); /* la coda dei thread pronti per essere eseguiti */
	LIST_HEAD(waitQueue); /* la coda dei thread in attesa di essere sbloccati */
	LIST_HEAD(waitForPseudoClockQueue); /* la coda dei thread in attesa di pseudoclock*/
	
	
	//inzializzo strutture dati
	root=proc_init();// inizializza root
	thread_init();
	msgq_init();
	
	//Caricare i vettori di trap e interrupt
	
	
	
	
	//treathCounter=0
	currentThread = NULL;
	
	
	//chiamo lo scheduler
	scheduler();


}
