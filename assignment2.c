#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//linked list
//高田陽一郎

// 構造体
typedef struct list {
  int data;
  struct list *next;
} list;

// insertion
list *insert(int data, list *start) {
  list *p;
  p = (list *)malloc(sizeof(list));
  if(p == NULL){
    exit(1);
  }

  if(start -> next == NULL){
    start -> next = p;
    p -> data = data;
    p -> next = NULL;
  }else{
    p -> data = data;
    p -> next = start -> next;
    start -> next = p;
  }
  return p;
}


int main(void){
    int i;
    list *start;
    start = (list *)malloc(sizeof(list));

    for(i = 1; i <= 100; i++){
        insert(i, start);
    }

    //linked listの表示
    while (start -> next != NULL) {
      start = start -> next;
      printf("%d ", start -> data);
    }
}