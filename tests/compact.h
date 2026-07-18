#ifndef COMPAT_H
#define COMPAT_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

struct proc {
  int pid;
};

void *kalloc(void);
void kfree(void *);
void panic(const char *);

#define printk printf

#endif
