#include "common.h"
#include <stddef.h>
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

/* 0 on success, 1 otherwise */
int StoreDump(const StatData *data, size_t count, char *path) {
  FILE *f = fopen(path, "wb");
  size_t ret;

  if (!f) {
    fprintf(stderr, "Can't open %s\n", path);
    return 1;
  }

  ret = fwrite(&count, sizeof(size_t), 1, f);
  if (ret != 1) {
    fprintf(stderr, "fwrite() failed: %zu\n", ret);
    fclose(f);
    return 1;
  }
  ret = fwrite(data, sizeof(StatData), count, f);
  if (ret != count) {
    fprintf(stderr, "fwrite() failed: %zu\n", ret);
    fclose(f);
    return 1;
  }

  fclose(f);
  return 0;
}

/* StatData array on success, NULL otherwise */
StatData *LoadDump(char *path, size_t *out_count) {
  size_t count;
  size_t ret;

  FILE *f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "Can't open %s\n", path);
    return NULL;
  }

  ret = fread(&count, sizeof(size_t), 1, f);
  if (ret != 1) {
    fprintf(stderr, "fread() failed: %zu\n", ret);
    fclose(f);
    return NULL;
  }

  StatData *data = malloc(count * sizeof(StatData));

  if (!data) {
    fclose(f);
    return NULL;
  }

  ret = fread(data, sizeof(StatData), count, f);
  if (ret != count) {
    fprintf(stderr, "fread() failed: %zu\n", ret);
    fclose(f);
    free(data);
    return NULL;
  }

  *out_count = count;

  fclose(f);
  return data;
}

void SortDump(StatData *data, size_t n) {
  qsort(data, n, sizeof(StatData), cmp_cost);
}

/* StatData array on success, NULL otherwise */
StatData *JoinDump(StatData *left, size_t n, StatData *right, size_t m,
                   size_t *out_size) {
  size_t total = n + m;
  StatData *temp = malloc(sizeof(StatData) * total);
  if (!temp)
    return NULL;

  for (size_t i = 0; i < n; i++) {
    temp[i] = left[i];
  }
  for (size_t i = 0; i < m; i++) {
    temp[n + i] = right[i];
  }

  qsort(temp, total, sizeof(StatData), cmp_id);
  size_t write = 0;

  for (size_t read = 0; read < total; read++) {
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
  for (int i = 2; i >= 0; i--) {
    printf("%d", (x >> i) & 1);
  }
}

void PrintData(const StatData data) {
  printf("%lx | %d | %.3e | ", data.id, data.count, data.cost);
  printf(data.primary ? "y | " : "n | ");
  PrintBinaryMode(data.mode);
  printf("\n");
}

/* 0 if equal, enum of the first unequal field otherwise */
int CompareData(const StatData a, const StatData b) {
  if (a.cost != b.cost)
    return COST;
  if (a.count != b.count)
    return COUNT;
  if (a.id != b.id)
    return ID;
  if (a.mode != b.mode)
    return MODE;
  if (a.primary != b.primary)
    return PRIMARY;
  return EQ;
}