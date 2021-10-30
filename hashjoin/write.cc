#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include "debug.h"

typedef struct _TUPLE {
  int key;
  int val;
} TUPLE;

#define FILE "DATA"

void
writeToStorage(const int max)
{
  int fd;
  TUPLE t;
  int key = 0;

  fd = open(FILE, O_WRONLY|O_TRUNC|O_CREAT, 0644);
  if (fd == -1) ERR;
  for (int i = 0; i < max; i++) {
    t.key = key++;
    t.val = rand() % 100;
    write(fd, &t, sizeof(t));
  }
  close(fd);
}

//追加で書いた部分
void 
readFromstorage(void){
  int fd;
  TUPLE t;
  int ret;

  fd = open(FILE, O_RDONLY);
  if (fd == -1) ERR;
  while(1){
    ret = read(fd, &t, sizeof(t));
    if(ret == 0) break;
    else if (ret == -1) ERR;
    read(fd, &t, sizeof(t));
    printf("%d %d\n", t.key, t.val);
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int max;

  if (argc != 2) max = 10;
  else max = atoi(argv[1]);

  writeToStorage(max);
  readFromstorage();

  return 0;
}
