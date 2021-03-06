#include <vector>

void hierarchical_affinity_propagation(vector <double> image,
                unsigned int line,
                unsigned int n_layers,
                vector< vector<unsigned int> > * exemplar,
                double lambda, int max_iter);


class HierarchicalAffinityPropagation{
  public:
      unsigned int n_layers;
      unsigned int line;
      double lambda;
      unsigned int max_iter;
      vector <double> similarities;
      vector <vector <unsigned int> > * exemplar;
      vector <vector <double> > tau;
      vector <vector <double> > phi;
      vector <vector <double> > responsabilities;
      vector <vector <double> > availabilities;
      vector <vector <double> > preferences;
      unsigned int length;

      HierarchicalAffinityPropagation(vector <double> similarities,
                                      unsigned int length,
                                      unsigned int n_layers,
                                      double lambda,
                                      unsigned int max_iter);
      void fit(vector< vector<unsigned int> > & exemplar);

  private:
    void update_responsabilities(unsigned int l);
    void update_availabilities(unsigned int l);
    void compute_preferences();
    void update_tau(unsigned int l);
    void update_phi(unsigned int l);
    void update_exemplars(vector< vector<unsigned int> > & exemplar);
};
