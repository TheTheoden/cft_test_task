#include "common.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "The functions takes 3 arguments.\n");
    return 1;
  }

  char *path_in1 = argv[1];
  char *path_in2 = argv[2];
  char *path_out = argv[3];

  int a_cnt;
  StatData *a = LoadDump(path_in1, &a_cnt);

  if (!a) {
    fprintf(stderr, "Can't load %s\n", path_in1);
    return 1;
  }

  int b_cnt;
  StatData *b = LoadDump(path_in2, &b_cnt);

  if (!b) {
    free(a);
    fprintf(stderr, "Can't load %s\n", path_in2);
    return 1;
  }

  int c_cnt;
  StatData *c = JoinDump(a, a_cnt, b, b_cnt, &c_cnt);

  if (!c) {
    free(a);
    free(b);
    fprintf(stderr, "Error while doing JoinDump\n");
    return 1;
  }

  SortDump(c, c_cnt); /* Not necessary */

  for (int i = 0; i < 10 && i < c_cnt; i++) {
    PrintData(c[i]);
  }

  StoreDump(c, c_cnt, path_out);

  free(a);
  free(b);
  free(c);
  return 0;
}