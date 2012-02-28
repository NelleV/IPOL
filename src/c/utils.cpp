#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
using namespace std;
#include "utils.h"


double quicksort(double * num, unsigned int first, unsigned int last){
  if(first < last){
    unsigned int pivot = rand() % (last - first) + first;
    pivot = partition(num, first, last, pivot);
    quicksort(num, first, pivot - 1);
    quicksort(num, pivot + 1, last);
  }
  return num[last / 2];
}

unsigned int partition(double * array, unsigned int first, unsigned int last, unsigned int  middle){
  double tmp = array[middle];
  array[middle] = array[last];
  array[last] = tmp;
  unsigned int j = first;

  for(unsigned int i = first; i < last - 1; i++){
    if(array[i] < array[last]){
      tmp = array[i];
      array[i] = array[j];
      array[j] = tmp;
      j++;
    }
  }

  tmp = array[last];
  array[last] = array[j];
  array[j] = tmp;
  return j;
}
