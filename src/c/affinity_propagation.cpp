// FIXME Should probably pass a reference of the image
// FIXME Add a tolerance value
int affinity_propagation(double * similarity, double lambda, int max_iter){

  // Check parameters
  if(lambda >= 1 || lambda <= 0){
    error("Damping value lambda should be in range (0, 1)");
  }
  if(max_iter <= 1){
      error("Maximum iteration should be greater than 1");
  }

  // Availabity and responsability
  double * availability = NULL;
  double * responsability = NULL;

  // FIXME get size of matrix

  for(int it = 0; it < max_iter; i++){
    // Compute responsability
    for(int i = 0; i < length; i++){
      double * AS = NULL;
      for(int j = 0; j < length; j++){
        int index = length * i + j;
        AS = availability[index] + similarity[index]
      }

      for(int j = 0; j < length; j++){
        int index = length * i + j;

        // TODO Move this elsewhere as a method ?
        // Get max of AS
        double max = 0.;
        for(int k = 0; k < length; j++){
          if(k == j){
            continue;
          }
          int ind = length * i + k;
          if(AS[ind] > max){
              max = AS[ind];
          }
        }
        responsability[index] = lambda * responsability[index] - (1 - lambda) * (similarity[index] + max);
      }
    }

    // Compute availability
    for(int i = 0; it < max_iter; i++){
      for(int j = 0; j < length, j++){
        int index = length * i + j;
        double sum = 0.;

        for(int k = 0; k < length; k++){
          if(k == i || k == j){
            continue;
          }
          int ind = length * i + k;
          if(responsability[ind] > 0){
            sum = sum + responsability[ind];
          }
        }

        if(i == j){
          double a = sum;
        } else {
          double a = responsability[(j + 1) * length] + sum;
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
  int * exemplar = NULL;
  for(int i = 0; i < length; i++){
    double max = availability[length * i];
    exemplar[i] = length * i;
    for(int j = 1; j < length; j++){
      int index = length * i + j;
      if(availability[index] + similarity[index] > max){
        max = availability[index] + similarity[index];
        exemplar[i] = index;
      }
    }
  }
  return exemplar;
}
