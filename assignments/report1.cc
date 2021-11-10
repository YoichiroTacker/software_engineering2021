#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <iostream>

#define HASH_SIZE 14000
#define DATA_SIZE 10000

int hash_array[HASH_SIZE];
int data_array[DATA_SIZE];

int accesscount;

//data_arrayの生成関数
void generate_dataarray(void){
    int i,j,tmp;

    // 現在時刻の情報で初期化
    srand((unsigned int)time(NULL));

    data_array[0] = rand();
    for(int i = 1; i < DATA_SIZE; ++i){
      data_array[i] = data_array[i-1] + rand()%5 + 1 ;
    }

    //配列の中身確認用
    for(i=0; i<DATA_SIZE; ++i){
      printf("%d ",data_array[i] );
    }
}

//binarysearchの関数
void binsearch(const int key, int left, int right){
  while (left < right){
    int mid = left + (right - left)/2;
    int num = data_array[mid];
    ++ accesscount;
    if(num == key){
      //printf("%dFound!\n",key);
      return;
    }else if(num < key){
      left = mid + 1;
    }else if(num > key){
      right = mid - 1;
    }
  }
  ++ accesscount;
  if(data_array[left] == key){
    //printf("%dFound!\n", key);
    return;
  }else{
    printf("%d not Found!\n", key);
  }
}

//data_arrayのデータをhashtableに挿入する関数
void insert_hashtable(int data){
  int i;
  int address = data % HASH_SIZE;
  bool b1 = true;

  for(i=0; i< HASH_SIZE; i++){
      if(hash_array[address] == DATA_SIZE){
        hash_array[address] = data;
        return;
  }
  else{
    while(hash_array[address] != DATA_SIZE){
       if(address<HASH_SIZE && b1 == true){
         address ++;
       }else if(address == HASH_SIZE && b1 ==true){
         address = 0;
         bool b1 = false;
       }else{
         printf("%dcannnot include in the hashtable!\n",data);
         exit(0);
       }
    }
    hash_array[address] = data;
  }
    // hashtableの中身確認用
    /*for(i=0; i<DATA_SIZE; ++i){
    printf("%d\n",hash_array[i]);
    }*/
    return;
  }
}

//hashtableを使って探索する関数
void hashsearch(int key){
  int address = key % HASH_SIZE;
  bool b1 = false;

  if(hash_array[address] == key){
    //printf("%dFound!\n",key);
    ++ accesscount;
    return;
  }
  else{
    while(hash_array[address] != key){
      ++ accesscount;
      if(address < HASH_SIZE){
        ++address;
      }else if(address == HASH_SIZE && b1 == false){
        address = 0;
        bool b1 = true;
      }else if(address == HASH_SIZE && b1 ==true){
        printf("%d not Found!\n",key);
      }
    }
    return;
  }
}

int main(void){
  int i;
  int key;

  generate_dataarray();

  printf("\nsearch key?\n");
  scanf("%d", &key);

  assert(key > 0);

  accesscount = 0;

  //binarysearchを実行する
  binsearch(key, 0, DATA_SIZE);

  printf("[binarysearch] %d回\n",accesscount);

  //hashtableの中身をリセット
  for(i=0; i< HASH_SIZE; ++i){
    hash_array[i] = DATA_SIZE;
  }

  //data_arrayのデータをhash_arrayに入れる
  for(i = 0; i< DATA_SIZE; i++){
    insert_hashtable(data_array[i]);
  }

  accesscount = 0;

  //hashsearchを実行する
  hashsearch(key);

  printf("[hashsearch] %d回\n",accesscount);
}