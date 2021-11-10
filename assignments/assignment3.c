#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//ハッシュテーブル(オープンアドレス法)

#define HASH_SIZE 1000000
#define DATA_SIZE 850000

int hash_array[HASH_SIZE];
int data_array[DATA_SIZE];

//hash_arrayにaccessした回数のカウント
int accesscount = 0;

//data_arrayを作成する関数
void generate_dataarray(void){
    int i;

    srand((unsigned int)time(NULL));

    //乱数の生成、arrayに入れる
    for(int i=0; i < DATA_SIZE; ++i){
      data_array[i] = rand();
    }
}

//データをhashtableに挿入する関数
void insert_hashtable(int data){
  int i;
  int address = data % DATA_SIZE;

  for(i=0; i<HASH_SIZE; i++){
      if(hash_array[address] == DATA_SIZE){
        hash_array[address] = data;
        ++ accesscount;
        return;
  }
  else{
    while(hash_array[address] != DATA_SIZE && address < DATA_SIZE){
       address++;
       ++ accesscount;
    }
    if(hash_array[address] == DATA_SIZE){
      hash_array[address] = data;
      ++ accesscount;
    }
    else{
      printf("%dcannnot include in the hashtable!\n",data);
    }
  }
    return;
  }
}

//hashtableを使って探索する関数
void search(int key){
  int address = key % DATA_SIZE;

  if(hash_array[address]==key){
    printf("%dFound!\n",key);
    ++ accesscount;
    return;
  }
  else{
    while(hash_array[address]!=key && address < DATA_SIZE){
      ++ address;
      ++ accesscount;
    }
    if(hash_array[address]==key){
      printf("%dFound!\n",key);
      ++ accesscount;
      return;
    }
  }
}

int main(void){
  int i;

  //hashtableの中身をリセット
  for(i=0; i<HASH_SIZE; ++i){
    hash_array[i] = DATA_SIZE;
  }
  
  generate_dataarray();


  //data_arrayのデータをhash_arrayに入れる
  for(i=0; i<DATA_SIZE; i++){
    insert_hashtable(data_array[i]);
  }

  // hashtableの中身確認用
  /*for(i=0; i<DATA_SIZE; ++i){
    printf("%d\n",hash_array[i]);
  }*/

  for(i=0; i<DATA_SIZE; i++){
    search(data_array[i]);
  }

  printf("hash_arrrayにアクセスした回数は%d\n回です。",accesscount);
}