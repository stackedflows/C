#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define colThreadNum 4

#define squareUnit 2
#define square 4
int** input;

int isValid = 1;

//
// threadpool for col calculations
//
typedef struct taskCol {
  int** input;
  int type;
  int index;
  int subX;
  int subY;
} taskCol;

taskCol colTaskQueue[3*square];
int colTaskCount = 0;

pthread_mutex_t colMutexQueue;
pthread_cond_t colCondQueue;

void checkCol(taskCol* task){
  int** input = task -> input;
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  int type = task -> type;
  if(type == 0){
    int index = task -> index;
    for(int i = 0; i < square; i++){
      if(input[i][index] == -1){
        continue;
      }
      else{
        available[input[i][index]]++;
        if(available[input[i][index]] > 1){
          isValid = 0;
          printf("col index: %d fails\n", index);
          return;
        }
      }
    }
    printf("col index: %d passes\n", index);
  }
  if(type == 1){
    int index = task -> index;
    for(int i = 0; i < square; i++){
      if(input[index][i] == -1){
        continue;
      }
      else{
        available[input[index][i]]++;
        if(available[input[index][i]] > 1){
          isValid = 0;
          printf("row index: %d fails\n", index);
          return;
        }
      }
    }
    printf("row index: %d passes\n", index);
  }
  if(type == 2){
    int subX = task -> subX;
    int subY = task -> subY;
    for(int i = subX; i < subX + squareUnit; i++){
      for(int j = subY; j < subY + squareUnit; j++){
        if(input[i][j] == -1){
          continue;
        }
        else{
          available[input[i][j]]++;
          if(available[input[i][j]] > 1){
            isValid = 0;
            printf("square: %d %d fails\n", subX, subY);
            return;
          }
        }
      }
    }
    printf("square: %d %d passes\n", subX, subY);
  }
  return;
}

void* colStartThread(void* args){
  while(1){
    taskCol task;
    pthread_mutex_lock(&colMutexQueue);
    // waits for task to become available
    while(colTaskCount == 0){
      pthread_cond_wait(&colCondQueue, &colMutexQueue);
      break;
    }
    // choose task
    task = colTaskQueue[0];
    int i;
    for(i = 0; i < colTaskCount; i++){
      colTaskQueue[i] = colTaskQueue[i + 1];
    }
    colTaskCount--;
    pthread_mutex_unlock(&colMutexQueue);
    // execute task
    checkCol(&task);
    if(colTaskCount == 0){
      break;
    }
  }
}

void submitColTask(taskCol task){
  pthread_mutex_lock(&colMutexQueue);
  colTaskQueue[colTaskCount] = task;
  colTaskCount++;
  pthread_mutex_unlock(&colMutexQueue);
  pthread_cond_signal(&colCondQueue);
}

int main(void){

  //
  // input example
  //
  int arrayIn[][square] = {
      {1,-1,-1,2},
      {2,-1,1,-1},
      {0,3,-1,-1},
      {1,-1,-1,0}
    };

  input = malloc(square * sizeof(int*));
  for(int i = 0; i < square; i++){
    input[i] = malloc(square * sizeof(int));
    for(int j = 0; j < square; j++){
      input[i][j] = arrayIn[i][j];
    }
  }

  //
  // col threadpool execute
  //
  pthread_t th_col[colThreadNum];
  pthread_mutex_init(&colMutexQueue, NULL);
  pthread_cond_init(&colCondQueue, NULL);
  for(int i = 0; i < colThreadNum; i++){
    if(pthread_create(&th_col[i], NULL, &colStartThread, NULL) != 0){
      printf("failed to create thread\n");
    }
  }

  for(int i = 0; i < square; i++){
    taskCol t = {
      .input = input,
      .type = 0,
      .index = i,
      .subX = -1,
      .subY = -1
    };
    submitColTask(t);
  }

  for(int i = 0; i < square; i++){
    taskCol t = {
      .input = input,
      .type = 1,
      .index = i,
      .subX = -1,
      .subY = -1
    };
    submitColTask(t);
  }

  int subX = 0;
  int subY = 0;
  for(int i = 0; i < squareUnit; i++){
    subY = 0;
    for(int j = 0; j < squareUnit; j++){
      taskCol t = {
        .input = input,
        .type = 2,
        .index = -1,
        .subX = subX,
        .subY = subY
      };
      submitColTask(t);
      subY = subY + squareUnit;
    }
    subX = subX + squareUnit;
  }

  for(int i = 0; i < colThreadNum; i++){
    if(pthread_join(th_col[i], NULL) != 0){
      printf("failed to join thread\n");
    }
  }
  pthread_mutex_destroy(&colMutexQueue);
  pthread_cond_destroy(&colCondQueue);

  switch(isValid){
    case 0:
      printf("does not pass\n");
      break;
    case 1:
      printf("passes\n");
      break;
  }

  free(input);
  return 1;
}
