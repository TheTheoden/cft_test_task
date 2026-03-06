#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int cmp_id(const void *a, const void *b) {
  long id1 = ((StatData *)a)->id;
  long id2 = ((StatData *)b)->id;
  if (id1 > id2)
    return 1;
  if (id1 < id2)
    return -1;
  return 0;
}

int cmp_cost(const void *a, const void *b) {
  float cost1 = ((StatData *)a)->cost;
  float cost2 = ((StatData *)b)->cost;
  if (cost1 > cost2)
    return 1;
  if (cost1 < cost2)
    return -1;
  return 0;
}

/* returns 0 on success, 1 otherwise */
int StoreDump(const StatData *data, int count, char *path) {
  FILE *f = fopen(path, "wb");

  if (!f) {
    fprintf(stderr, "Can't open %s\n", path);
    return 1;
  }

  fwrite(&count, sizeof(int), 1, f);
  fwrite(data, sizeof(StatData), count, f);

  fclose(f);
  return 0;
}

/* returns StatData array on success, NULL otherwise */
StatData *LoadDump(char *path, int *out_count) {
  int count;

  FILE *f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "Can't open %s\n", path);
    return NULL;
  }

  fread(&count, sizeof(int), 1, f);

  StatData *data = malloc(count * sizeof(StatData));

  if (!data)
    return NULL;

  fread(data, sizeof(StatData), count, f);

  *out_count = count;

  fclose(f);
  return data;
}

void SortDump(StatData *data, int n) {
  qsort(data, n, sizeof(StatData), cmp_cost);
}

/* StatData array on success, NULL otherwise */
StatData *JoinDump(StatData *left, int n, StatData *right, int m,
                   int *out_size) {
  int total = n + m;
  StatData *temp = malloc(sizeof(StatData) * total);
  if (!temp)
    return NULL;

  for (int i = 0; i < n; i++) {
    temp[i] = left[i];
  }
  for (int i = 0; i < m; i++) {
    temp[n + i] = right[i];
  }

  qsort(temp, total, sizeof(StatData), cmp_id);
  int write = 0;

  for (int read = 0; read < total; read++) {
    if (write == 0 || temp[read].id != temp[read - 1].id) {
      temp[write++] = temp[read];
    } else {
      temp[write - 1].count += temp[read].count;
      temp[write - 1].cost += temp[read].cost;
      temp[write - 1].primary &= temp[read].primary;

      if (temp[read].mode > temp[write - 1].mode) {
        temp[write - 1].mode = temp[read].mode;
      }
    }
  }

  *out_size = write;
  return temp;
}

void PrintBinaryMode(unsigned int x) {
  int started = 0;

  for (int i = 2; i >= 0; i--) { /* We know that mode uses 3 bits */
    int bit = (x >> i) & 1;

    if (bit)
      started = 1;

    if (started) {
      printf("%d", bit);
    }
  }

  if (!started)
    printf("0");
}

void PrintData(const StatData data) {
  printf("%lx | %d | %.3e | ", data.id, data.count, data.cost);
  printf(data.primary ? "y |" : "n | ");
  PrintBinaryMode(data.mode);
  printf("\n");
}

/* returns 1 if two instances have equal fields, 0 otherwise */
int CompareData(const StatData a, const StatData b) {
  if (a.cost != b.cost)
    return 0;
  if (a.count != b.count)
    return 0;
  if (a.id != b.id)
    return 0;
  if (a.mode != b.mode)
    return 0;
  if (a.primary != b.primary)
    return 0;
  return 1;
}