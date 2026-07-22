#ifndef MLFQ
#define MLFQ

/* This architecture can be improved by using single block of memory which improves cache affinity
and reduces free complexity. */

#include "proc.h"
#include "defs.h"
#include "types.h"

#define MAX_QUEUES 5

extern uint8 allotment_ms_arr[MAX_QUEUES];

struct Node {
  struct proc *data;
  struct Node *back; // as we are picking from front
};

struct queue {
  struct Node *head;
  struct Node *tail;
};

/*Multi-Level Feedback queue*/
struct mlfq {
  struct queue *queue[MAX_QUEUES];
  uint8 allotment_ms[MAX_QUEUES];
};

/* Allocates Mlfq with allotment_ms per queue*/
void allocateMlfq(struct mlfq **__restrict head);

/* Insert into mlfq in top most queue */
void insertMlfq(struct mlfq *head, struct proc *enter_proc);

/* Pops and return a process, starting from first queue to last
 * Returns NULL if none found*/
struct proc *popMlfq(struct mlfq *head);

void promotionMlfq(struct mlfq *head, uint *ticks);

/* Free multi-queue */
void freeMlfq(struct mlfq *__restrict head);

#endif
