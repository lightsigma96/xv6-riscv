#include "multi-queue.h"
#include "defs.h"

uint8 allotment_ms_arr[MAX_QUEUES] = {10, 20, 30, 40};
#define PROMOTION_TIME_MS allotment_ms_arr[MAX_QUEUES - 1] + 10

void
allocateMlfq(struct mlfq **__restrict head)
{
  if ((*head = (struct mlfq *)kalloc()) == 0) {
    panic("MLFQ Allocation");
  }

  for (uint8 h = 0; h < MAX_QUEUES; h++) {
    (*head)->queue[h] = (struct queue *)kalloc();
    (*head)->allotment_ms[h] = allotment_ms_arr[h];

    // during creation of mlfq no processes exists hence tail & head point to NULL
    (*head)->queue[h]->tail = (void *)0;
    (*head)->queue[h]->head = NULL;
  }
}

struct proc *
popMlfq(struct mlfq *head)
{
  for (uint8 q = 0; q < MAX_QUEUES; q++) {
    if (head->queue[q]->tail == NULL && head->queue[q]->head == NULL) {
      continue;
    }

    if (head->queue[q]->tail == head->queue[q]->head) {
      struct Node *ret_node = head->queue[q]->head;
      struct proc *ret_proc = ret_node->data;

      head->queue[q]->head = NULL;
      head->queue[q]->tail = NULL;

      kfree(ret_node);

      return ret_proc;
    }

    struct Node *ret_node = head->queue[q]->head;
    struct proc *ret_proc = head->queue[q]->head->data;
    head->queue[q]->head = head->queue[q]->head->back;
    kfree(ret_node);

    return ret_proc;
  }
  return NULL;
}

void
insertMlfq(struct mlfq *head, struct proc *enter_proc)
{
  if (enter_proc->queue_no > MAX_QUEUES) {
    panic("queue number greater than expected");
  }

  if (!head->queue[enter_proc->queue_no]->tail) {
    struct Node *first_node = (struct Node *)
      kalloc(); // kalloc returns 4096 bytes of memory, not that much is needed for 1 node, take raw mem needed for node.

    first_node->data = enter_proc;
    first_node->back = NULL;
    head->queue[enter_proc->queue_no]->tail = first_node;
    head->queue[enter_proc->queue_no]->head = first_node;

    return; // some status
  }
  struct Node *node = (struct Node *)
    kalloc(); // kalloc returns 4096 bytes of memory, not that much is needed for 1 node, take raw mem needed for node.

  node->data = enter_proc;
  node->back = NULL;

  head->queue[enter_proc->queue_no]->tail->back = node;
  head->queue[enter_proc->queue_no]->tail = node;
  head->queue[enter_proc->queue_no]->tail->back = NULL;
}

void
promotionMlfq(struct mlfq *head, uint *ticks)
{
#ifdef TEST_V
#include <stdio.h>
  printf("\nPromotion Time: %d\n", PROMOTION_TIME_MS);
#endif

  if (*ticks < PROMOTION_TIME_MS) {
    return;
  }

  for (int q = MAX_QUEUES - 1; q >= 1; q--) {
    while (head->queue[q]->head != NULL) {
      struct Node *promote_node = head->queue[q]->head;

      if (head->queue[q]->head == head->queue[q]->tail) {
        head->queue[q]->head = NULL;
        head->queue[q]->tail = NULL;
      } else {
        head->queue[q]->head = head->queue[q]->head->back;
      }

      promote_node->data->queue_no = 0;
      insertMlfq(head, promote_node->data);
      kfree(promote_node);
    }
  }
  *ticks = 0;
};

void
freeMlfq(struct mlfq *__restrict head)
{

  for (int i = 0; i < MAX_QUEUES; i++) {
    struct Node *n = head->queue[i]->head;
    while (n != NULL) {
      head->queue[i]->head = n->back;
      kfree(n);
      n = head->queue[i]->head;
    }
    kfree(head->queue[i]);
  }
  kfree(head);
}
