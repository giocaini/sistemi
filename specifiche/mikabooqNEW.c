#include <stdint.h>
#include "mikabooq.h"

/* teste delle liste libere */
struct list_head pcbFree;
struct list_head tcbFree;
struct list_head msgFree;

/************************************** PROC MGMT ************************/

/* initialize the data structure */
/* the return value is the address of the root process */
struct pcb_t *proc_init(void) {
	static pcb_t pcb_table[MAXPROC]; //Array statico contenente MAXPROC pcb_t 
	int i;
	INIT_LIST_HEAD(&(pcbFree)); //Inizializza la testa dell lista libera pcbFree
	//pcbFree=&(pcb_table[0]); PERCHE'?????	
	for(i=1; i<MAXPROC; i++){ //Aggiunge tutti i pcb_t (a parte il root) nella lista libera
		pcb_t* newPcb = &pcb_table[i];
        	list_add(&(newPcb->p_siblings), &(pcbFree)); //p_siblings usata per gestire la lista libera
	}
	pcb_t* root=&(pcb_table[0]); //root non è stato messo nella pcbFree
	INIT_LIST_HEAD(&(root->p_children)); //Inizializza la lista libera p_children di root
	INIT_LIST_HEAD(&(root->p_threads)); //Inizializza la lista libera p_threads di root
	/////INIT_LIST_HEAD(&(root->p_siblings)); //Inizializza la lista libera p_siblings di root
	root->p_parent=NULL; //root non ha "genitori"
return(root);
}

/* alloc a new empty pcb (as a child of p_parent) */
/* p_parent cannot be NULL */
struct pcb_t *proc_alloc(struct pcb_t *p_parent){
		if (list_empty(&(pcbFree))) return NULL; //non ci sono pcb_t liberi
		if(p_parent==NULL) return NULL; //p_parent è nullo, non può avere figli

		//Lista libera non è vuota e p_parent non è nullo!		
		//allocpcb: puntatore al primo pcb_t libero della pcbFree			
		struct pcb_t* allocpcb = container_of(pcbFree.next, pcb_t, p_siblings);
		//Stacco l'elemento dalla lista libera
		list_del(pcbFree.next);
		//inizializzo i campi del pcb_t che ho allocato
		INIT_LIST_HEAD(&(allocpcb->p_children));
		INIT_LIST_HEAD(&(allocpcb->p_threads));

		/////fratelli di allocpcb = figli del padre (senza allocpcb)		
		/////allocpcb->p_siblings=p_parent->p_children;
		
		//aggiungo l'elemento allocato ai figli del padre
		list_add(&(allocpcb->p_siblings),&(p_parent->p_children));
		allocpcb->p_parent=p_parent;
		
		////aggiungo allocpcb ai suoi fratelli
		////struct pbc_t* item; //item: elemento corrente nel ciclo
		////list_for_each_entry(item, &(allocpcb->p_siblings), p_siblings){
			////list_add(&(allocpcb->p_siblings),&(item->p_siblings));
		////}
		
		return(allocpcb);
}

/* delete a process (properly updating the process tree links) */
/* this function must fail if the process has threads or children. */
/* return value: 0 in case of success, -1 otherwise */
int proc_delete(struct pcb_t *oldproc){
	if(!(list_empty(oldproc->p_children))) return -1; //oldproc ha figli attivi
	if(!(list_empty(oldproc->p_threads))) return -1; //oldproc ha processi attivi
	
	/////list_del(&(allopcb->p_sibling), &(oldproc->p_sibling)); WHY???????

	//Tolgo oldproc dai figli del padre	
	list_del(&(oldproc->p_siblings);	
	
	////struct pbc_t* item; //item elemento corrente nel ciclo
	////list_for_each_entry(item, &(oldproc->p_siblings), p_siblings){
		////list_add(&(allocpcb->p_siblings),&(item->p_siblings)) WHY???????
		////list_del(&(oldproc->p_siblings));
	////}

	list_add(&(oldproc->p_siblings),&(pcbFree)); //rimette oldproc nella lista libera freePcb
	//NB Non vengono ripuliti i campi di oldproc, in quanto verranno reinizializzati alla prima alloc.
	return(0);
}

/* return the pointer to the first child (NULL if the process has no children) */
struct pcb_t *proc_firstchild(struct pcb_t *proc){
	if((list_empty(proc->p_children))) return NULL; //non ha figli
	return container_of((proc->p_children).next, pcb_t, p_sibling); //primo figlio
	
}

/* return the pointer to the first thread (NULL if the process has no threads) */
struct tcb_t *proc_firstthread(struct pcb_t *proc){
	if((list_empty(proc->p_threads))) return NULL; //non ha threads
	return container_of((proc->p_threads).next, pcb_t, p_sibling); //primo thread
}

/****************************************** THREAD ALLOCATION ****************/

/* initialize the data structure */
void thread_init(void) {
	static tcb_t tcb_table[MAXTHREAD]; //Array statico contenente MAXTHREAD tcb_t 
	int i;
	INIT_LIST_HEAD(&(tcbFree)); //Inizializza la testa della lista libera tcbFree
	for(i=0; i<MAXPROC; i++){ //Aggiunge tutti i tcb_t della tcb_table nella lista libera
		tcb_t* newTcb = &tcb_table[i];
        	list_add(&(newTcb->t_next), &(tcbFree)); //NB: t_next usata per gestire la lista libera
	}
}

/* alloc a new tcb (as a thread of process) */
/* return -1 if process == NULL or mo more available tcb-s.
	 return 0 (success) otherwise */
struct tcb_t *thread_alloc(struct pcb_t *process) {
	if (list_empty(&(tcbFree))) return NULL; //non ci sono tcb_t liberi
	if(process==NULL) return NULL; //process è nullo, non può avere threads

	//Lista libera non è vuota e process non è nullo!		
	//allocTcb: puntatore al primo tcb_t libero della tcbFree			
	struct tcb_t* allocTcb = container_of(tcbFree.next, tcb_t, t_next);
	//Stacco l'elemento dalla lista libera
	list_del(tcbFree.next);
	//NB: inizializzo tutti i campi, tranne	t_s
	//inizializzo i campi del tcb_t che ho allocato
	INIT_LIST_HEAD(&(allocTcb->t_sched));
	INIT_LIST_HEAD(&(allocTcb->t_msgq));
	//imposto il processo "padre" in p_tcb
	allocTcb->t_pcb=process;
	//inizializzo lo stato del thread
	allocTcb->t_status=T_STATUS_NONE; //allocato, ma non ancora eseguito
	//inizializzo t_wait4sender
	allocTcb->t_wait4sender=NULL;
	//aggiungo l'elemento allocato ai thread del processo "padre"
	list_add(&(allocTcb->t_next),&(process->p_threads));
	return(allocTcb);
}

/* Deallocate a tcb (unregistering it from the list of threads of
	 its process) */
/* it fails if the message queue is not empty (returning -1) */
int thread_free(struct tcb_t *oldthread){
	if (!(list_empty(oldthread->t_msgq)) return -1;	
	//Tolgo oldthread dalla lista dei thread del processo	
	list_del(&(oldthread->next));
	//Aggiungo oldthread alla lista libera dei tcb	
	list_add(&(oldthread->next), &(tcbFree));
	//NB Non vengono ripuliti i campi di oldthread, in quanto verranno reinizializzati alla prima alloc.
	return 0;
}

/*************************** THREAD QUEUE ************************/

/* add a tcb to the scheduling queue */
void thread_enqueue(struct tcb_t *new, struct list_head *queue){
	list_add(&(new->t_sched),&(queue));
}

/* return the head element of a scheduling queue.
	 (this function does not dequeues the element)
	 return NULL if the list is empty */
struct tcb_t *thread_qhead(struct list_head *queue){
	if (list_empty(queue)) return NULL;
	return (conrainer_of(queue.next, tcb_t, t_sched));
}

/* get the first element of a scheduling queue.
	 return NULL if the list is empty */
struct tcb_t *thread_dequeue(struct list_head *queue);
	if (list_empty(queue)) return NULL;
	struct list_head *temp_t = queue.next;
	list_del(queue.next);
	return (conrainer_of(temp_t, tcb_t, t_sched));
}

/*************************** MSG QUEUE ************************/

/* initialize the data structure */
/* the return value is the address of the root process */
void msgq_init(void){
	static msg_t msg_table[MAXMSG]; //Array statico contenente MAXMSG msg_t 
	int i;
	INIT_LIST_HEAD(&(msgFree)); //Inizializza la testa della lista libera msgFree
	for(i=0; i<MAXMSG; i++){ //Aggiunge tutti i msg_t della msg_table nella lista libera
		msg_t* newMsg = &msg_table[i];
        	list_add(&(newMsg->m_next), &(msgFree)); //NB: m_next usata per gestire la lista libera
	}
}

/* add a message to a message queue. */
/* msgq_add fails (returning -1) if no more msgq elements are available */
int msgq_add(struct tcb_t *sender, struct tcb_t *destination, uintptr_t value){
	if (sender==NULL) return -1; //Sender NULL
	if (destination==NULL) return -1; //Destination NULL
	if (list_empty(&(msgFree)) return -1; //Lista libera dei messaggi vuota
	
	//Lista libera non è vuota e sender e destination non sono nulli!		
	//allocMsg: puntatore al primo msg_t libero della msgFree			
	struct msg_t* allocMsg = container_of(msgFree.next, msg_t, m_next);
	//Stacco l'elemento dalla lista libera
	list_del(msgFree.next);
	//Imposto il mittente del messaggio
	allocMsg->m_sender=sender;
	//Imposto il payload del messaggio	
	allocMsg->m_value=value;
	//Aggiungo il messaggio alla lista dei messaggi del destinatario
	list_add(&(allocMsg->m_next), &(destination->t_msgq));
	return 0;
}

/* retrieve a message from a message queue */
/* -> if sender == NULL: get a message from any sender
	 -> if sender != NULL && *sender == NULL: get a message from any sender and store
	 the address of the sending tcb in *sender
	 -> if sender != NULL && *sender != NULL: get a message sent by *sender */
/* return -1 if there are no messages in the queue matching the request.
	 return 0 and store the message payload in *value otherwise. */
int msgq_get(struct tcb_t **sender, struct tcb_t *destination, uintptr_t *value){
	if (sender==NULL){ //CASO 1: Sender NULL - No filtro mittente
		if (list_empty(destination->t_msgq)) return -1; //Destinatario senza messaggi
		//temp_m: puntatore al primo msg_t libero della t_msgq del destinatario			
		struct msg_t *temp_m = container_of((destination->t_msgq).next, msg_t, m_next);
		//Elimino temp_m dalla lista t_msgq del destinatario		
		list_del(temp_m);
		//Aggiungo temp_m alla lista libera msgFree
		list_add(&(temp_m->m_next),&(msgFree));
		//Imposto il campo value		
		*value=temp_m->m_value;
		return 0;
	}
	if ((sender!=NULL)&&(*sender==NULL)){ //CASO 2: No filtro mittente, ma memorizzato
		if (list_empty(destination->t_msgq)) return -1; //Destinatario senza messaggi
		//temp_m: puntatore al primo msg_t libero della t_msgq del destinatario			
		struct msg_t *temp_m = container_of((destination->t_msgq).next, msg_t, m_next);
		//Elimino temp_m dalla lista t_msgq del destinatario		
		list_del(temp_m);
		//Aggiungo temp_m alla lista libera msgFree
		list_add(&(temp_m->m_next),&(msgFree));
		//Imposto il campo value		
		*value=temp_m->m_value;
		//Imposto il campo sender
		*sender = temp_m->m_sender;		
		return 0;
	}
	if ((sender!=NULL)&&(*sender!=NULL)){ //CASO 3: Filtro mittente
		struct msg_t *temp_m;		
		struct msg_t *item;
		bool found=0;
		list_for_each_entry(item,&(destination->t_msgq),t_msgq) {
			if ((!found)&&(item->m_sender==*sender)){
				temp_m=item;
				found=1;
			}
		}
		if !found return -1;
		//Elimino temp_m dalla lista t_msgq del destinatario		
		list_del(temp_m);
		//Aggiungo temp_m alla lista libera msgFree
		list_add(&(temp_m->m_next),&(msgFree));
		//Imposto il campo value		
		*value=temp_m->m_value;
		return 0;
	}
}
