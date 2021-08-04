#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *f0(){
  int val = 101;
  int* res = malloc(sizeof(int));
  *res = val;
  return (void*)res;
}
void *f1(){
  int val = 202;
  int* res = malloc(sizeof(int));
  *res = val;
  return (void*)res;
}

int main(void){


  int* res0;
  int* res1;

  pthread_t t0;
  pthread_t t1;
  if (pthread_create(&t0, NULL, &f0, NULL)!=0){
    return 1;
  }
  if (pthread_create(&t1, NULL, &f1, NULL)!=0){
    return 11;
  }

  if(pthread_join(t0, (void**)&res0)!=0){
    return 2;
  }
  if(pthread_join(t1, (void**)&res1)!=0){
    return 22;
  }

  if (*res0 < *res1){
    printf("%d\n", *res1);
  }else{
    printf("%d\n", *res1);
  }
  return 0;
}
