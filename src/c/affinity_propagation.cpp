
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <algorithm>
using namespace std;
#include "globals.h"
#include "affinity_propagation.h"


// FIXME Should probably pass a reference of the image
// FIXME Add a tolerance value
void affinity_propagation(vector<double> similarity,
    unsigned int length, unsigned int exemplar[], double lambda, int max_iter){

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
  vector<double> similarity_co;
  for(unsigned int i = 0; i < length; i++){
    for(unsigned j = 0; j < length; j++){
      similarity_co.push_back(similarity[i * length + j]);
    }
  }

  sort(similarity_co.begin(), similarity_co.begin() + length * length);
  double preference = similarity_co[length * length / 2];

  // Place preference on the diagonal of S
  for(unsigned int i = 0; i < length; i++){
    similarity[i * length + i] = preference;
  }

  // Availabity and responsability
  vector<double> availability;
  vector<double> responsability;

  // Let's initialise:
  for(unsigned int i = 0; i < length; i++){
    for(unsigned int j = 0; j < length; j++){
      availability.push_back(0);
      responsability.push_back(0);
    }
  }

  for(unsigned int it = 0; it < max_iter; it++){
    // Compute responsability
    for(unsigned int i = 0; i < length; i++){
      double AS[length];
      for(unsigned int j = 0; j < length; j++){
        AS[j] = availability[i * length + j] + similarity[i * length + j];
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

        responsability[i * length + j] = lambda * responsability[i * length + j] +
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
          if(responsability[k * length + j] > 0.){
            sum = sum + responsability[k * length + j];
          }
        }

        double a = 0;
        if(i == j){
          a = sum;
        }else{
          a = responsability[j * length + j] + sum;
          if(a > 0){
            a = 0;
          }
        }

        availability[i * length + j] = lambda * availability[i * length + j] + (1 - lambda) * a;
      }
    } // availability

    // TODO Should break out of loop if converged sooner.
  }

  // Compute the exemplars
  for(unsigned int i = 0; i < length; i++){
    double max = availability[i * length + 0] + responsability[i * length + 0];
    exemplar[i] = 0;
    for(unsigned int j = 0; j < length; j++){
      if(availability[i * length + j] + responsability[i * length + j] > max){
        max = availability[i * length + j] + responsability[i * length + j];
        exemplar[i] = j;
      }
    }
  }
}

/**
 * Hierarchical Affinity Propagation
 *
 **/
void hierarchical_affinity_propagation(vector<double> similarity,
    unsigned int length, unsigned int n_layers, unsigned int exemplar[],
    double lambda, int max_iter){

  cout << "Affinity Propagation" << endl;
  // Compute preferences
  //
  // Need to copy the similarity array to sort it, in order to get the median
  vector<double> similarity_co;
  for(unsigned int i = 0; i < length; i++){
    for(unsigned j = 0; j < length; j++){
      similarity_co.push_back(similarity[i * length + j]);
    }
  }

  sort(similarity_co.begin(), similarity_co.begin() + length * length);
  double preference = similarity_co[length * length / 2];

  // Create 8 layers of length preference
  vector<vector<double> > preferences;
  for(unsigned int l = 0; l < n_layers; l++){
    vector<double> lpref;
    for(unsigned int i = 0; i < length; i++){
      lpref.push_back(preference);
    }
    preferences.push_back(lpref);
  }

  // Availabity and responsability
  vector<vector<double> > availabilities;
  vector<vector<double> > responsabilities;

  // Let's initialise:
  for(unsigned int l = 0; l < n_layers; l++){
    vector<double> availability;
    vector<double> responsability;
    for(unsigned int i = 0; i < length; i++){
      for(unsigned int j = 0; j < length; j++){
        availability.push_back(0);
        responsability.push_back(0);
      }
    }
    availabilities.push_back(availability);
    responsabilities.push_back(responsability);
  }

  for(unsigned int it = 0; it < max_iter; it++){
    cout << "--- Iteration " << it << " ---" << endl;
    vector<vector<double> > tau;
    vector<vector<double> > phi;
    for(unsigned int l = 0; l < n_layers; l++){
      vector<double> t;
      vector<double> f;
      for(unsigned int i = 0; i < length; i++){
        t.push_back(0);
        f.push_back(0);
      }
      tau.push_back(t);
      phi.push_back(f);
    }

    for(unsigned int l = 0; l < n_layers - 1; l++){


      for(unsigned int j = 0; j < length; j++){
        // calculate sum.
        double sum = 0;
        for(unsigned int k = 0; k < length; k++){
          if(k == j){
            continue;
          }
          if(responsabilities[l][k * length + j] > 0){
            sum = sum + responsabilities[l][k * length + j];
          }
        }
        tau[l + 1][j] = preferences[l][j] + responsabilities[l][j * length + j] + sum;
      }
    }

    for(int l = n_layers - 1; l >= 0; l--){
      for(unsigned int j = 0; j < length; j++){

        double max = availabilities[l][j * length] + similarity[j * length];

        for(unsigned int k = 1; k < length; k++){
          if(availabilities[l][j * length + k] + similarity[j * length + k] > max){
            max = availabilities[l][j * length + k] + similarity[j * length + k];
          }
        }
      }
      // FIXME
    }


    for(unsigned int l = 0; l < n_layers; l++){
      // Compute responsability

      if(l == 0){
        for(unsigned int i = 0; i < length; i++){
          double AS[length];
          for(unsigned int j = 0; j < length; j++){
            AS[j] = availabilities[l][i * length + j] + similarity[i * length + j];
          }
          AS[i] = AS[i] + preferences[l][i];

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

            if(i == j) {
              responsabilities[l][i * length + j] = lambda * 
                                                  responsabilities[l][i * length + j] +
                                               (1 - lambda) * (preferences[l][i] - max);

            } else {
              responsabilities[l][i * length + j] = lambda * 
                                                  responsabilities[l][i * length + j] +
                                               (1 - lambda) * (similarity[index] - max);
            }
          }
        }
      } else {

        for(unsigned int i = 0; i < length; i++){
          double AS[length];

          for(unsigned int j = 0; j < length; j++){
            AS[j] = availabilities[l][i * length + j] + similarity[i * length + j];
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
            max = - max;

            if(tau[l][i] < max){
              max = tau[l][i];
            }
            responsabilities[l][i * length + j] = lambda * responsabilities[l][i * length + j] +
                                (1 - lambda) * (similarity[index] + max);
          }
        }
      }

      // Compute availability
      if(l == n_layers - 1) {
        for(unsigned int i = 0; i < length; i++){
          for(unsigned int j = 0; j < length; j++){

            double sum = 0.;

            for(unsigned int k = 0; k < length; k++){
              if(k == i || k == j){
                continue;
              }
              if(responsabilities[l][k * length + j] > 0.){
                sum = sum + responsabilities[l][k * length + j];
              }
            }

            double a = 0;
            if(i == j){
              a = sum;
            }else{
              a = responsabilities[l][j * length + j] + sum;
              if(a > 0){
                a = 0;
              }
            }

            availabilities[l][i * length + j] = lambda * availabilities[l][i * length + j] + (1 - lambda) * a;
          }
        } // availability

      } else {


        for(unsigned int i = 0; i < length; i++){
          for(unsigned int j = 0; j < length; j++){
            double sum = 0.;

            for(unsigned int k = 0; k < length; k++){
              if(k == i || k == j){
                continue;
              }
              if(responsabilities[l][k * length + j] > 0.){
                sum = sum + responsabilities[l][k * length + j];
              }
            }
            double a = 0;
            if(i == j){
              a = preferences[l][j] + phi[l][j] + sum;
            }else{
              a = preferences[l][j] + phi[l][j] + responsabilities[l][j * length + j] + sum;
              if(a > 0){
                a = 0;
              }
            }

            availabilities[l][i * length + j] = lambda * availabilities[l][i * length + j] + (1 - lambda) * a;
          }
        } // availability
      }

    }

    // TODO Should break out of loop if converged sooner.
  }
  // Compute the exemplars
  for(unsigned int l = 0; l < n_layers; l++){
    for(unsigned int i = 0; i < length; i++){
      double max = availabilities[l][i * length + 0] + responsabilities[l][i * length + 0];
      exemplar[i + l * length] = 0;
      for(unsigned int j = 0; j < length; j++){
        if(availabilities[l][i * length + j] + responsabilities[l][i * length + j] > max){
          max = availabilities[l][i * length + j] + responsabilities[l][i * length + j];
          exemplar[i + l * length] = j;
        }
      }
    }
  }
}

