#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.c>
#include <unistd.h>

#define square 4

int checkRow(int input[][square],  int index, int width){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < width; i++){
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

int checkCol(int input[][square], int index, int height){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < height; i++){
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

int checkSquare(int input[][square], int subX, int subY, int subWidth){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = subX; i < subX + subWidth; i++){
    for(int j = subY; j < subY + subWidth; j++){
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

// total number of independent processes here to finish is 3 * square
int validate(int input[][square], int width, int height){
  int isValid = 1;
  for(int i = 0; i < height; i++){
    isValid&&checkRow(input, i, width);
    if(isValid == 0) return isValid;
  }
  for(int i = 0; i < width; i++){
    isValid&&checkCol(input, i, height);
    if(isValid == 0) return isValid;
  }
  int squareUnit = floor(sqrt(square));
  int subX = 0;
  int subY = 0;
  for(int i = 0; i < squareUnit; i++){
    subY = 0;
    for(int j = 0; j < squareUnit; j++){
      isValid&&checkSquare(input, subX, subY, squareUnit);
      if(isValid == 0) return isValid;
      subY = subY + squareUnit;
    }
    subX = subX + squareUnit;
  }
  return isValid;
}

int main(void){

  int input0[][square] = {
    {1,-1,-1,-1},
    {2,-1,1,3},
    {0,3,-1,-1},
    {-1,-1,2,-1}
  };

  int width = sizeof(input0)/sizeof(input0[0]);
  int height = sizeof(input0[0])/sizeof(int);

  int k = validate(input0, square, square);
  printf("%d\n", k);
  return 0;
}
