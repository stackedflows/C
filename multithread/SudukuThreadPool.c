#include <stdio.h>
#include <stdlib.h>

int checkRow(int input[][3],  int index, int width){
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

int checkCol(int input[][3], int index, int width){
  int available[] = {0,0,0,0,0,0,0,0,0,0};
  for(int i = 0; i < width; i++){
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

int checkSquare(int input[][3],  int subX, int subY, int subWidth){
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

int main(void){

  int input0[][3] = {
    {1,-1,-1},
    {2,-1,1},
    {0,3,-1}
  };

  int input1[][3] = {
    {1,1,-1},
    {2,-1,1},
    {2,3,-1}
  };

  int input2[][3] = {
    {1,0,-1},
    {2,0,1},
    {2,3,-1}
  };

  int width = sizeof(input0)/sizeof(input0[0]);
  int height = sizeof(input0[0])/sizeof(int);

  int k = checkSquare(input0, 0, 1, 2);
  printf("%d\n", k);

  return 0;
}
