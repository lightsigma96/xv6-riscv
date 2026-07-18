#include "compact.h"

void *
kalloc(void)
{
  return calloc(1, 4096);
}

void
kfree(void *p)
{
  free(p);
}

void
panic(const char *msg)
{
  fprintf(stderr, "panic: %s\n", msg);
  abort();
}
