#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define rowThreadNum 2
#define colThreadNum 2
#define squareThreadNum 2

#define squareUnit 2
#define square 4
int** input;

int isValid = 1;

/*
typedef struct TaskCol {
  int index;
  int input[][square];
} TaskCol;
TaskCol colTaskQueue[256];
int colTaskCount = 0;

typedef struct TaskSquare {
  int subX;
  int subY;
  int input[][square];
} TaskSquare;
TaskSquare squareTaskQueue[256];
int squareTaskCount = 0;
*/

/*
int checkCol(int input[][square], int index){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < square; i++){
    if(input[i][index] == -1){
      continue;
    }
    else{
      available[input[i][index]]++;
      if(available[input[i][index]] > 1){
        return 0;
      }
    }
  }
  return 1;
}

int checkSquare(int input[][square], int subX, int subY){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = subX; i < subX + squareUnit; i++){
    for(int j = subY; j < subY + squareUnit; j++){
      if(input[i][j] == -1){
        continue;
      }
      else{
        available[input[i][j]]++;
        if(available[input[i][j]] > 1){
          return 0;
        }
      }
    }
  }
  return 1;
}
*/

// threadpool for row calculations
typedef struct taskCol {
  int** input;
  int index;
} taskCol;
taskCol colTaskQueue[square];
int colTaskCount = 0;

void checkCol(taskCol* task){
  int** input = task -> input;
  int index = task -> index;
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < square; i++){

    if(input[i][index] == -1){
      continue;
    }
    else{
      available[input[i][index]]++;
      if(available[input[i][index]] > 1){
        isValid = 0;
        printf("index: %d fails\n", index);
        return;
      }
    }
    for(int j = 0; j < 9; j++){
      printf("%d ", available[j]);
    }printf("\n");
  }
  printf("index: %d passes\n", index);
  return;
}

pthread_mutex_t colMutexQueue;
pthread_cond_t colCondQueue;

void* colStartThread(void* args){
  while(1){
    taskCol task;
    pthread_mutex_lock(&colMutexQueue);
    // waits for task to become available
    while(colTaskCount == 0){
      pthread_cond_wait(&colCondQueue, &colMutexQueue);
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

/*
//
// threadpool for row calculations
//
typedef struct taskRow {
  int** input;
  int index;
} taskRow;
taskRow rowTaskQueue[square];
int rowTaskCount = 0;

void checkRow(taskRow* task){
  int** input = task -> input;
  int index = task -> index;
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < square; i++){
    if(input[index][i] == -1){
      continue;
    }
    else{
      available[input[index][i]]++;
      if(available[input[index][i]] > 1){
        isValid = 0;
        printf("index: %d fails\n", index);
        return;
      }
    }
  }
  printf("index: %d passes\n", index);
  return;
}

pthread_mutex_t rowMutexQueue;
pthread_cond_t rowCondQueue;

void* rowStartThread(void* args){
  while(1){
    taskRow task;
    pthread_mutex_lock(&rowMutexQueue);
    // waits for task to become available
    while(rowTaskCount == 0){
      pthread_cond_wait(&rowCondQueue, &rowMutexQueue);
    }
    // choose task
    task = rowTaskQueue[0];
    int i;
    for(i = 0; i < rowTaskCount; i++){
      rowTaskQueue[i] = rowTaskQueue[i + 1];
    }
    rowTaskCount--;
    pthread_mutex_unlock(&rowMutexQueue);
    // execute task
    checkRow(&task);
    if(rowTaskCount == 0){
      break;
    }
  }
}

void submitRowTask(taskRow task){
  pthread_mutex_lock(&rowMutexQueue);
  rowTaskQueue[rowTaskCount] = task;
  rowTaskCount++;
  pthread_mutex_unlock(&rowMutexQueue);
  pthread_cond_signal(&rowCondQueue);
}
*/

int main(void){

  //
  // input example
  //
  int arrayIn[][square] = {
      {1,-1,-1,2},
      {2,-1,1,-1},
      {0,3,-1,-1},
      {-1,-1,-1,2}
    };

  input = malloc(square * sizeof(int*));
  for(int i = 0; i < square; i++){
    input[i] = malloc(square * sizeof(int));
    for(int j = 0; j < square; j++){
      input[i][j] = arrayIn[i][j];
    }
  }

  /*
  //
  // row threadpool execute
  //
  pthread_t th_row[rowThreadNum];
  pthread_mutex_init(&rowMutexQueue, NULL);
  pthread_cond_init(&rowCondQueue, NULL);
  for(int i = 0; i < rowThreadNum; i++){
    if(pthread_create(&th_row[i], NULL, &rowStartThread, NULL) != 0){
      printf("failed to create thread\n");
    }
  }
  for(int i = 0; i < square; i++){
    taskRow t = {
      .input = input,
      .index = i
    };
    submitRowTask(t);
  }
  for(int i = 0; i < rowThreadNum; i++){
    if(pthread_join(th_row[i], NULL) != 0){
      printf("failed to join thread\n");
    }
  }
  pthread_mutex_destroy(&rowMutexQueue);
  pthread_cond_destroy(&rowCondQueue);
  */

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
      .index = i
    };
    submitColTask(t);
  }
  for(int i = 0; i < colThreadNum; i++){
    if(pthread_join(th_col[i], NULL) != 0){
      printf("failed to join thread\n");
    }
  }
  pthread_mutex_destroy(&colMutexQueue);
  pthread_cond_destroy(&colCondQueue);

  /*
  int subX = 0;
  int subY = 0;
  for(int i = 0; i < squareUnit; i++){
    subY = 0;
    for(int j = 0; j < squareUnit; j++){
      isValid = isValid&&checkSquare(input, subX, subY);
      if(isValid == 0){
        printf("%d\n", isValid);
        return isValid;
      }
      subY = subY + squareUnit;
    }
    subX = subX + squareUnit;
  }
  */
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
