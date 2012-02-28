
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
#include "globals.h"
#include "affinity_propagation.h"
#include "utils.h"


// FIXME Should probably pass a reference of the image
// FIXME Add a tolerance value
void affinity_propagation(double * similarity,
    unsigned int length, unsigned int exemplar[], double lambda, int max_iter){
  printf("Computing Affinity propagation\n");

  // Check parameters
//  if(lambda > (double) 0. && lambda < (double) 1.){
//    error("Damping value lambda should be in range (0, 1)");
//  }
//  if(max_iter <= 1){
//      error("Maximum iteration should be greater than 1");
//  }

  // Compute preferences
  //
  // Need to copy the similarity array to sort it, in order to get the median
  double similarity_co[length][length];
  for(unsigned int i = 0; i < length; i++){
    for(unsigned j = 0; j < length; j++){
      similarity_co[i][j] = similarity[i * length + j];
    }
  }

  double preference = quicksort(*similarity_co, 0, length * length);
  cout << "preference : " << preference << endl;

  // Place preference on the diagonal of S
  for(unsigned int i = 0; i < length; i++){
    similarity[i * length + i] = preference;
  }

  // Availabity and responsability
  double availability[length][length];
  double responsability[length][length];

  // Let's initialise:
  for(unsigned int i = 0; i < length; i++){
    for(unsigned int j = 0; j < length; j++){
      availability[i][j] = 0;
      responsability[i][j] = 0;
    }
  }

  for(unsigned int it = 0; it < max_iter; it++){
    cout << '.' << flush;
    // Compute responsability
    for(unsigned int i = 0; i < length; i++){
      double AS[length];
      for(unsigned int j = 0; j < length; j++){
        AS[j] = availability[i][j] + similarity[i * length + j];
      }

      for(unsigned int j = 0; j < length; j++){
        unsigned int index = length * i + j;

        // Get max of AS
        double max = 0.;
        if(j == 0){
          max = AS[1];
        }else{
          max = AS[0];
        }

        for(unsigned int k = 0; k < length; k++){
          if(k == j){
            continue;
          }
          if(AS[k] >= max){
            max = AS[k];
          }
        }

        responsability[i][j] = lambda * responsability[i][j] +
                            (1 - lambda) * (similarity[index] - max);
      }
    }

    // Compute availability
    for(unsigned int i = 0; i < length; i++){
      for(unsigned int j = 0; j < length; j++){
        double sum = 0.;

        for(unsigned int k = 0; k < length; k++){
          if(k == i || k == j){
            continue;
          }
          if(responsability[k][j] > 0.){
            sum = sum + responsability[k][j];
          }
        }

        double a = 0;
        if(i == j){
          a = sum;
        }else{
          a = responsability[j][j] - sum;
          if(a > 0){
            a = 0;
          }
        }

        availability[i][j] = lambda * availability[i][j] + (1 - lambda) * a;
        printf("%d, %d %f %f %f\n", i, j, availability[i][j], responsability[i][j], sum);
      }
    } // availability

    // TODO Should break out of loop if converged sooner.
  }

  cout << endl;
  // Compute the exemplars
  for(unsigned int i = 0; i < length; i++){
    double max = availability[i][0] + responsability[i][0];
    exemplar[i] = 0;
    for(unsigned int j = 0; j < length; j++){
      if(availability[i][j] + responsability[i][j] > max){
        max = availability[i][j] + responsability[i][j];
        exemplar[i] = j;
      }
    }
    printf("index %d, result %d, max %f \n", i, exemplar[i], max);
  }
}
