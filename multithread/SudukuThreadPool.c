#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <pthread.c>
//#include <unistd.h>

#define square 4
#define squareUnit floor(sqrt(square))

//
// tasks for threads to complete
//
typedef struct TaskRow {
  int index;
  int input[][square];
} TaskRow;
TaskRow rowTaskQueue[256];
int rowTaskCount = 0;

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

//
// executionable tasks
//
int checkRow(int input[][square], int index){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < square; i++){
    if(input[index][i] == -1){
      continue;
    }
    else{
      available[input[index][i]]++;
      if(available[input[index][i]] > 1){
        return 0;
      }
    }
  }
  return 1;
}

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

//
// total number of independent processes here to finish is 3 * square
//
int validate(int input[][square]){
  int isValid = 1;
  for(int i = 0; i < square; i++){
    isValid&&checkRow(input, i);
    if(isValid == 0) return isValid;
  }
  for(int i = 0; i < square; i++){
    isValid&&checkCol(input, i);
    if(isValid == 0) return isValid;
  }

  int subX = 0;
  int subY = 0;
  for(int i = 0; i < squareUnit; i++){
    subY = 0;
    for(int j = 0; j < squareUnit; j++){
      isValid&&checkSquare(input, subX, subY);
      if(isValid == 0) return isValid;
      subY = subY + squareUnit;
    }
    subX = subX + squareUnit;
  }
  return isValid;
}

//
// approach: leave 2 threads in 3 thread pools, and make the task queue of length 3 * square
//
int main(void){

  int input0[][square] = {
    {1,-1,-1,-1},
    {2,-1,1,3},
    {0,3,-1,-1},
    {-1,-1,2,-1}
  };

  int width = sizeof(input0)/sizeof(input0[0]);
  int height = sizeof(input0[0])/sizeof(int);

  int k = validate(input0);
  printf("%d\n", k);
  return 0;
}
