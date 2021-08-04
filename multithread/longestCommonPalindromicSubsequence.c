#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// time complexity: O(n^2)
// space complexity: O(1)
void *odd(char* seq, int len){
  int currMaxOdd = 0;
  for(int i = 1; i < len - 1; i++){
    int low = i - 1;
    int high = i + 1;
    int tempMaxOdd = 1;
    while(low >= 0 && high < len && seq[low] == seq[high]){
      low--;
      high++;
      tempMaxOdd = tempMaxOdd + 2;
    }
    if(tempMaxOdd > currMaxOdd){
      currMaxOdd = tempMaxOdd;
    }
  }
  int* res = malloc(sizeof(int));
  *res = currMaxOdd;
  printf("%d\n", currMaxOdd);
  return (void*)&res;
}

// time complexity: O(n^2)
// space complexity: O(1)
void *even(char* seq, int len){
  int currMaxEven = 0;
  for(int i = 0; i < len - 1; i++){
    int low = i;
    int high = i + 1;
    int tempMaxEven = 0;
    while(low >= 0 && high < len && seq[low] == seq[high]){
      low--;
      high++;
      tempMaxEven = tempMaxEven + 2;
    }
    if(tempMaxEven > currMaxEven){
      currMaxEven = tempMaxEven;
    }
  }
  int* res = malloc(sizeof(int));
  *res = currMaxEven;
  printf("%d\n", currMaxEven);
  return (void*)&res;
}

// execute both "odd" and "even" in parallel on spereate threads, then compare their results
// total time complexity: O(n^2)
int main(void){
  char seq0[] = "asdhjbajshsj";
  char seq1[] = "asdhjbajshhsjsf";
  int len0 = sizeof(seq0)/sizeof(seq0[0]) - 1;
  int len1 = sizeof(seq1)/sizeof(seq1[0]) - 1;

  odd((char*)seq0, len0);
  even((char*)seq1, len1);

  /* must wrap res and len in a truct and pass to thread for this to work */

  /*

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

  */
  return 0;
}
