#ifndef MLFQ
#define MLFQ

/* This architecture can be improved by using single block of memory which improves cache affinity
and reduces free complexity. */

// remove these as include in main file
#include "proc.h"
#include "defs.h"
#include "types.h"

#define MAX_QUEUES 5

struct Node {
  struct proc *data;
  struct Node *back; // as we are picking from front
};

struct queue {
  struct Node *head;
  struct Node *tail;
  uint8 node_count; // not needed for now but maybe later
};

/*Multi-Level Feedback queue*/
struct mlfq {
  struct queue *queue[MAX_QUEUES];
  uint8 allotment_ms[MAX_QUEUES];
};

/* Allocates Mlfq with allotment_ms per queue*/
void allocateMlfq(const uint8 *__restrict allotment_ms_arr,
                  struct mlfq **__restrict head);

void insertMlfq(struct mlfq *head, struct proc *enter_proc);

/* Pops and return a process, starting from first queue to last*/
struct proc *popMlfq(struct mlfq *head);

/* Free multi-queue */
void freeMlfq(struct mlfq *__restrict head);

#endif
