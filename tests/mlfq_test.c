#include "../kernel/multi-queue.h"
#include "../kernel/types.h"
#include <stdio.h>

int
main(void)
{
  struct mlfq *mlfq_head = (struct mlfq *)kalloc();
  allocateMlfq(&mlfq_head);

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

  struct proc p = {0};

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

  struct proc p1, p2, p3, p4 = {0};

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

  /*----------------------------------------------------------*/
  /* Demotion Test with Time (One Time)                       */
  /*----------------------------------------------------------*/

  uint d_test_ticks = 15;
  struct proc *demote_proc;
  demote_proc->at_tick = 0;
  demote_proc->queue_no = 0;

  insertMlfq(mlfq_head, demote_proc);
  demote_proc = popMlfq(mlfq_head);

  if (demote_proc->queue_no < MAX_QUEUES - 1 &&
      d_test_ticks - demote_proc->at_tick >=
        allotment_ms_arr[demote_proc->queue_no])
    demote_proc->queue_no += 1;

  if (demote_proc->queue_no != 1) {
    printf("[FAIL] Demote One Time, queue no: %d\n", demote_proc->queue_no);
    return 1;
  }
  printf("[PASS] Demote One Time queue no: %d\n", demote_proc->queue_no);

  /*----------------------------------------------------------*/
  /* Promotion Test with Time ()                              */
  /*----------------------------------------------------------*/

  /* (The process is being removed from queue above and hence there is nothing in queue to promote) */

  uint p_test_ticks = 5;
  promotionMlfq(mlfq_head, &p_test_ticks);
  struct proc *demoted_proc = popMlfq(mlfq_head);

  if (!demoted_proc) {
    printf("[FAIL] Promote One Time, Returned Empty\n");
    return 1;
  }

  if (demoted_proc->queue_no != 0) {
    printf("[FAIL] Promote One Time, queue no: %d\n", demoted_proc->queue_no);
    return 1;
  }
  printf("[PASS] Promote One Time queue no: %d\n", demoted_proc->queue_no);

  freeMlfq(mlfq_head);

  return 0;
}
