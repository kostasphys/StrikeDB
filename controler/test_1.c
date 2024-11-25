#include <stdio.h>

struct test_test
{
  int id;
  char name[255];
};

struct test_test __attribute__((__section__(".sdb.test"))) dbs = {.id=226, .name="Kostiras"};
struct test_test __attribute__((__section__(".sdb.test"))) dbss = {.id=528, .name="Kostiras"};