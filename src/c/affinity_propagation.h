#include <vector>

void affinity_propagation(vector <double> image, unsigned int line,
                unsigned int exemplar[],
                  double lambda, int max_iter);

void hierarchical_affinity_propagation(vector <double> image,
                unsigned int line,
                unsigned int n_layers,
                unsigned int exemplar[],
                double lambda, int max_iter);
