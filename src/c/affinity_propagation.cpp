
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "affinity_propagation.h"

// FIXME Should probably pass a reference of the image
// FIXME Add a tolerance value
void affinity_propagation(double * similarity,
    unsigned int length, unsigned int exemplar[], double lambda, int max_iter){
  // Check parameters
//  if(lambda > (double) 0. && lambda < (double) 1.){
//    error("Damping value lambda should be in range (0, 1)");
//  }
//  if(max_iter <= 1){
//      error("Maximum iteration should be greater than 1");
//  }

  // Availabity and responsability
  double availability[(unsigned long) length * length];
  double responsability[(unsigned long) length * length];

  for(unsigned int it = 0; it < max_iter; it++){
    // Compute responsability
    for(int i = 0; i < length; i++){
      double AS[length];
      for(int j = 0; j < length; j++){
        int index = length * i + j;
        AS[j] = availability[index] + similarity[index];
      }

      for(int j = 0; j < length; j++){
        int index = length * i + j;

        // TODO Move this elsewhere as a method ?
        // Get max of AS
        double max = 0.;
        for(int k = 0; k < length; k++){
          if(k == j){
            continue;
          }
          if(AS[k] > max){
              max = AS[k];
          }
        }
        responsability[index] = lambda * responsability[index] - 
                                (1 - lambda) * (similarity[index] + max);
      }
    }

    // Compute availability
    for(unsigned int i = 0; i < length; i++){
      for(unsigned int j = 0; j < length; j++){
        unsigned int index = length * i + j;
        double sum = 0.;

        for(unsigned int k = 0; k < length; k++){
          if(k == i || k == j){
            continue;
          }
          unsigned int ind = length * i + k;
          if(responsability[ind] > 0){
            sum = sum + responsability[ind];
          }
        }

        double a = responsability[j * length + j] + sum;
        if(i == j){
          a = sum;
        }
        if(a < 0){
          a = 0;
        }

        availability[index] = a;
      }
    } // availability
 
    // TODO Should break out of loop if converged sooner.
  }

  // Compute the exemplars
  for(unsigned int i = 0; i < length; i++){
    double max = availability[length * i];
    exemplar[i] = length * i;
    for(unsigned int j = 0; j < length; j++){
      unsigned int index = length * i + j;
      if(availability[index] + similarity[index] > max){
        max = availability[index] + similarity[index];
        exemplar[i] = j;
      }
    }
    printf("index %d, result %d \n", i, exemplar[i]);
  }
}
