#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define MAX_BUFFER_SIZE 100000
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef struct StatData {
  long id;
  int count;
  float cost;
  unsigned int primary : 1;
  unsigned int mode : 3;
} StatData;

enum Field { EQ, ID, COUNT, COST, PRIMARY, MODE };

int StoreDump(const StatData *data, size_t count, char *path);

StatData *LoadDump(char *path, size_t *out_count);

void SortDump(StatData *data, size_t n);

StatData *JoinDump(StatData *left, size_t n, StatData *right, size_t m,
                   size_t *out_size);

void PrintData(const StatData data);

int CompareData(const StatData a, const StatData b);

#endif