#include "../kernel/multi-queue.h"
#include "../kernel/types.h"
#include <stdio.h>

int
main(void)
{
  uint8 allotment_ms_arr[MAX_QUEUES] = {1, 2, 4, 8, 16};
  struct mlfq *mlfq_head = (struct mlfq *)kalloc();
  allocateMlfq(allotment_ms_arr, &mlfq_head);

  /*----------------------------------------------------------*/
  /* Empty Queue Test                                         */
  /*----------------------------------------------------------*/

  if (popMlfq(mlfq_head) != 0)
    printf("[FAIL] EMPTY POP\n");
  else
    printf("[PASS] EMPTY POP\n");

  /*----------------------------------------------------------*/
  /* Single Insert / Pop Test                                 */
  /*----------------------------------------------------------*/

  struct proc p;

  insertMlfq(mlfq_head, &p);

  if (popMlfq(mlfq_head) != &p)
    printf("[FAIL] SINGLE POP\n");
  else if (mlfq_head->queue[0]->head != 0 || mlfq_head->queue[0]->tail != 0)
    printf("[FAIL] QUEUE NOT EMPTY\n");
  else
    printf("[PASS] SINGLE POP\n");

  /*----------------------------------------------------------*/
  /* Priority Test                                            */
  /*----------------------------------------------------------*/

  struct proc p1, p2, p3, p4;

  struct Node *n1 = (struct Node *)kalloc();
  struct Node *n2 = (struct Node *)kalloc();
  struct Node *n3 = (struct Node *)kalloc();
  struct Node *n4 = (struct Node *)kalloc();

  n1->data = &p1;
  n1->back = n2;

  n2->data = &p2;
  n2->back = 0;

  n3->data = &p3;
  n3->back = 0;

  n4->data = &p4;
  n4->back = 0;

  /* Queue 0 : P1 -> P2 */
  mlfq_head->queue[0]->head = n1;
  mlfq_head->queue[0]->tail = n2;

  /* Queue 1 : P3 */
  mlfq_head->queue[1]->head = n3;
  mlfq_head->queue[1]->tail = n3;

  /* Queue 2 : P4 */
  mlfq_head->queue[2]->head = n4;
  mlfq_head->queue[2]->tail = n4;

  if (popMlfq(mlfq_head) != &p1) {
    printf("[FAIL] PRIORITY 1\n");
  } else if (popMlfq(mlfq_head) != &p2) {
    printf("[FAIL] PRIORITY 2\n");
  } else if (popMlfq(mlfq_head) != &p3) {
    printf("[FAIL] PRIORITY 3\n");
  } else if (popMlfq(mlfq_head) != &p4) {
    printf("[FAIL] PRIORITY 4\n");
  } else if (popMlfq(mlfq_head) != 0) {
    printf("[FAIL] PRIORITY EMPTY\n");
  }

  printf("[PASS] PRIORITY TEST\n");

  freeMlfq(mlfq_head);

  return 0;
}
