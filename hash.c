#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//ハッシュテーブル(オープンアドレス法)

#define SIZE 100
#define div 85

int array[SIZE];

//データをhashtableに挿入する関数
void generate_hashtable(int data){
  int i;
  int address = data % div;

  if(array[address] == div){
    array[address] = data;
    return;
  }
  else{
    while(array[address] != div && address < div){
       address++;
    }
    if(array[address] == div){
      array[address] = data;
    }
    else{
      printf("%dcannnot include in the hashtable!\n",data);
    }
    
    return;
  }
}

//hashtableを使って探索する関数
void function(int key){
  int address = key % div;
  int count_flag=0;

  if(array[address]==key){
    printf("%dFound!\n",key);
    return;
  }
  else{
    while(array[address]!=key && address < div){
      ++address;
    }
    if(array[address]==key){
      printf("%dFound!\n",key);
      return;
    }
  }
}

int main(void){
  int i, j, k;
  int counter =0;
  srand((unsigned int)time(NULL));

  //配列の中身をリセット
  for(i=0; i<SIZE; ++i){
    array[i] = div;
  }

//70つのデータをhashtableに挿入
  while(counter<80){
    j = rand();
    generate_hashtable(j);
    counter =counter +1;
  }

  // hashtableの中身確認用
  /*for(i=0; i<SIZE; ++i){
    printf("%d\n",array[i]);
  }*/

  for(k=0; k<SIZE; k++){
    if(array[k]!=div){
      function(array[k]);
    }
  }
}