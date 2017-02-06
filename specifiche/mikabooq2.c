#include "mikabooq.h"
#include <stdint.h>

struct list_head pcbFree;

/************************************** PROC MGMT ************************/

/* initialize the data structure */
/* the return value is the address of the root process */
struct pcb_t *proc_init(void) {
	static pcb_t pcbs[MAXPROC];
	pcb_t * root = &pcbs[0];
	INIT_LIST_HEAD(root->p_children));
	INIT_LIST_HEAD(root->p_siblings));	
	INIT_LIST_HEAD(root->p_threads));
	root->p_parent = NULL;	
	int i;
	for (i=1; i<MAXPROC; i++) {
		list_add(&(pcbs[i]),&pcbFree);
			
	}
	return root;	
}

/* alloc a new empty pcb (as a child of p_parent) */
/* p_parent cannot be NULL */
struct pcb_t *proc_alloc(struct pcb_t *p_parent){
	if (p_parent==NULL) return NULL;
	if (list_empty(&(pcbFree))) return NULL;
	pcb_t* allocPcb = container_of(pcbFree.next, pcb_t, ????);
	list_del(pcbFree.next);
	INIT_LIST_HEAD(&(allocPcb->p_threads));
	INIT_LIST_HEAD(&(allocPcb->p_children));
	allocPpcb->p_siblings = p_parent->p_children;
	list_add(&allocPcb,&(p_parent->p_children));
	allocPpcb->p_parent = p_parent;
	return allocPpcb;
}

/* delete a process (properly updating the process tree links) */
/* this function must fail if the process has threads or children. */
/* return value: 0 in case of success, -1 otherwise */
int proc_delete(struct pcb_t *oldproc);

/* return the pointer to the first child (NULL if the process has no children) */
struct pcb_t *proc_firstchild(struct pcb_t *proc);

/* return the pointer to the first thread (NULL if the process has no threads) */
struct tcb_t *proc_firstthread(struct pcb_t *proc);

/****************************************** THREAD ALLOCATION ****************/

/* initialize the data structure */
void thread_init(void);

/* alloc a new tcb (as a thread of process) */
/* return -1 if process == NULL or mo more available tcb-s.
	 return 0 (success) otherwise */
struct tcb_t *thread_alloc(struct pcb_t *process);

/* Deallocate a tcb (unregistering it from the list of threads of
	 its process) */
/* it fails if the message queue is not empty (returning -1) */
int thread_free(struct tcb_t *oldthread);

/*************************** THREAD QUEUE ************************/

/* add a tcb to the scheduling queue */
void thread_enqueue(struct tcb_t *new, struct list_head *queue);

/* return the head element of a scheduling queue.
	 (this function does not dequeues the element)
	 return NULL if the list is empty */
struct tcb_t *thread_qhead(struct list_head *queue);

/* get the first element of a scheduling queue.
	 return NULL if the list is empty */
struct tcb_t *thread_dequeue(struct list_head *queue);

static inline void thread_outqueue(struct tcb_t *this) {
	list_del(&this->t_sched);
}

#define for_each_thread_in_q(pos, queue) \
	list_for_each_entry(pos, queue, t_sched)

/*************************** MSG QUEUE ************************/

/* initialize the data structure */
/* the return value is the address of the root process */
void msgq_init(void);

/* add a message to a message queue. */
/* msgq_add fails (returning -1) if no more msgq elements are available */
int msgq_add(struct tcb_t *sender, struct tcb_t *destination, uintptr_t value);

/* retrieve a message from a message queue */
/* -> if sender == NULL: get a message from any sender
	 -> if sender != NULL && *sender == NULL: get a message from any sender and store
	 the address of the sending tcb in *sender
	 -> if sender != NULL && *sender != NULL: get a message sent by *sender */
/* return -1 if there are no messages in the queue matching the request.
	 return 0 and store the message payload in *value otherwise. */
int msgq_get(struct tcb_t **sender, struct tcb_t *destination, uintptr_t *value);

