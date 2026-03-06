#ifndef COMMON_H
#define COMMON_H

typedef struct StatData {
  long id;
  int count;
  float cost;
  unsigned int primary : 1;
  unsigned int mode : 3;
} StatData;

enum Field { EQ, ID, COUNT, COST, PRIMARY, MODE };

int StoreDump(const StatData *data, int count, char *path);

StatData *LoadDump(char *path, int *out_count);

void SortDump(StatData *data, int n);

StatData *JoinDump(StatData *left, int n, StatData *right, int m,
                   int *out_size);

void PrintData(const StatData data);

int CompareData(const StatData a, const StatData b);

#endif