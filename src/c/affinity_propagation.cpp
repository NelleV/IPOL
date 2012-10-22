
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <algorithm>
using namespace std;
#include "globals.h"
#include "affinity_propagation.h"


HierarchicalAffinityPropagation::HierarchicalAffinityPropagation(
    vector <double> similarities, unsigned int length,
    unsigned int n_layers,
    double lambda,
    unsigned int max_iter){
  this->similarities = similarities;
  this->length = length;
  this->n_layers = n_layers;
  this->lambda = lambda;
  this->max_iter = max_iter;

  // Initialise phi and tau to 0
  for(unsigned int l = 0; l < this->n_layers; l++){
    vector<double> t;
    vector<double> f;
    for(unsigned int i = 0; i < this->length; i++){
      t.push_back(0);
      f.push_back(0);
    }
    this->tau.push_back(t);
    this->phi.push_back(f);
  }

  // Allocate space for responsabilities and availabilities
  for(unsigned int l = 0; l < this->n_layers; l++){
    vector<double> availability;
    vector<double> responsability;
    for(unsigned int i = 0; i < this->length; i++){
      for(unsigned int j = 0; j < this->length; j++){
        availability.push_back(0);
        responsability.push_back(0);
      }
    }
    this->availabilities.push_back(availability);
    this->responsabilities.push_back(responsability);
  }

  // Compute preferences
  this->compute_preferences();
}


void HierarchicalAffinityPropagation::compute_preferences(){
  // Compute preferences
  // Need to copy the similarity array to sort it, in order to get the median
  vector<double> similarity_co;
  for(unsigned int i = 0; i < length; i++){
    for(unsigned j = 0; j < length; j++){
      similarity_co.push_back(similarities[i * length + j]);
    }
  }

  sort(similarity_co.begin(), similarity_co.begin() + length * length);
  double preference = similarity_co[length * length / 2];

  // Create n_layers layers of length preference
  for(unsigned int l = 0; l < n_layers; l++){
    vector<double> lpref;
    for(unsigned int i = 0; i < length; i++){
      lpref.push_back(preference);
    }
    this->preferences.push_back(lpref);
  }
}


void HierarchicalAffinityPropagation::update_responsabilities(unsigned int l){
  if(l == 0){
    for(unsigned int i = 0; i < length; i++){
        // Compute the sum availibility + similarity for each line
      double AS[length];
      for(unsigned int j = 0; j < length; j++){
        AS[j] = availabilities[l][i * length + j] + similarities[i * length + j];
      }
      AS[i] = AS[i] + this->preferences[l][i];

      for(unsigned int j = 0; j < length; j++){

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
                                          (1 - lambda) * (similarities[i * length + j] - max);
        }
      } // Finished computation for line i
    } // Finished computation for layer 0
  } else {
    // Compute responsabilities for l > 1

    for(unsigned int i = 0; i < length; i++){
      double AS[length];

      for(unsigned int j = 0; j < length; j++){
        AS[j] = availabilities[l][i * length + j] + similarities[i * length + j];
      }

      for(unsigned int j = 0; j < length; j++){
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
        responsabilities[l][i * length + j] = 
              lambda * responsabilities[l][i * length + j] +
              (1 - lambda) * (similarities[i * length + j] + max);
      }
    }
  }
}


void HierarchicalAffinityPropagation::update_availabilities(unsigned int l){
  if(l == n_layers - 1) {
    // Compute availibility for l = L
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
    // Compute availability for l < L

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


void HierarchicalAffinityPropagation::update_phi(unsigned int l){
  for(unsigned int j = 0; j < length; j++){

    double max = availabilities[l][j * length] + similarities[j * length];

    for(unsigned int k = 1; k < length; k++){
      if(availabilities[l][j * length + k] + similarities[j * length + k] > max){
        max = availabilities[l][j * length + k] + similarities[j * length + k];
      }
    }
    phi[l - 1][j] = lambda * phi[l - 1][j] * (1 - lambda ) * max;
  }
}


void HierarchicalAffinityPropagation::update_tau(unsigned int l){
  for(unsigned int j = 0; j < length; j++){
    // calculate sum.
    double sum = 0;
    for(unsigned int k = 0; k < length; k++){
      if(k == j){
        continue;
      }
      if(responsabilities[l][k * length + j] < 0){
        sum = sum + responsabilities[l][k * length + j];
      }
    }
    this->tau[l + 1][j] = this->lambda * this->tau[l + 1][j] + 
        (1 - this->lambda) * (this->preferences[l][j] +
                              this->responsabilities[l][j * this->length + j]
                              + sum);
  }
}

void HierarchicalAffinityPropagation::update_exemplars(vector< vector<unsigned int> > & exemplar){
  // Initialize exemplars:
  for(unsigned int l = 0; l < n_layers; l++){
    vector< unsigned int > ex;
    for(unsigned int i = 0; i < length; i++){
      ex.push_back(0);
    }
    exemplar.push_back(ex);
  }
 

  // Compute the exemplars
  for(unsigned int l = 0; l < n_layers; l++){
    for(unsigned int i = 0; i < length; i++){
      double max = availabilities[l][i * length + 0] +
                   responsabilities[l][i * length + 0];
      exemplar[l][i] = 0;
      for(unsigned int j = 0; j < length; j++){
        if(availabilities[l][i * length + j] +
           responsabilities[l][i * length + j] >= max){
          max = availabilities[l][i * length + j] +
                responsabilities[l][i * length + j];
          exemplar[l][i] = j;
        }
      }
    }
  }
}

void HierarchicalAffinityPropagation::fit(vector< vector <unsigned int> > & exemplar){
  unsigned int internal_loop_iter = 10;

  for(unsigned int it = 0; it < max_iter; it++){
    // Go up then down
    // FIXME n_layers or n_layers - 1 ???
    for(unsigned int l = 0; l < n_layers - 1; l++){
      // internal loop 
      for(unsigned int ilit = 0; ilit < internal_loop_iter; ilit++){

        this->update_responsabilities(l);
        this->update_availabilities(l);
      }
      this->update_tau(l);
    }

    // Now go down
    // FIXME n_layers -1 or n_layers ?
    // FIXME l> 0 or l >= 0 ?
    for(unsigned int l = n_layers - 1; l > 0; l--){
      //internal loop
      for(unsigned ilit = 0; ilit < internal_loop_iter; ilit++){
        this->update_availabilities(l);
        this->update_responsabilities(l);
      }
      this->update_phi(l);
    }
  }

  this->update_exemplars(exemplar);
}
