#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;

#include "affinity_propagation.h"
#include "globals.h"
#include "csv.h"

struct options{
  char file_name[FILENAME_MAX];
  char output_file[FILENAME_MAX];
  int lambda;
  int n_layers;
};

void print_usage();

struct options get_default_options() {
  struct options opts;
  opts.lambda = 0.5;
  opts.n_layers = 4;
  strcpy(opts.output_file, "output.csv");
  return opts;
}

bool get_args(struct options *opts, int argc, char ** argv){
  int i = 0;
  for(i = 1; i < argc; i++){
    if(argv[i][0] != '-'){
      break;
    }

    switch(argv[i][1]){
      case 'h':
        print_usage();
        exit(0);
        break;
      case 'l':
        opts->lambda = atol(argv[i] + 2);
        break;
      case 'n':
        opts->n_layers = atol(argv[i] + 2);
        break;
      case 'o':
        strcpy(opts->output_file, argv[i] + 2);
        break;
      default:
          printf("%s is not recognized as an argument\n", argv[i]);
          print_usage();
          exit(0);
    }
  }
  strcpy(opts->file_name, argv[argc - 1]);
}

void print_usage() {
  printf("Usage: ac -l<lambda> \n"
         "          -n<number of layers>\n"
         "          -o<output file> --default: output.csv\n"
         "          -h prints this help\n"
         "          file_name\n");
}

int main(int argc, char ** argv){
  struct options opts = get_default_options();
  get_args(&opts, argc, argv);
  cout << "Reading input file" << endl << flush;
  CSVReader reader(opts.file_name);

  vector<double> matrix = reader.read();

  unsigned int results [reader.num_line * opts.n_layers];
  hierarchical_affinity_propagation(matrix, reader.num_line,
            opts.n_layers, results, opts.lambda, 200);

  CSVWriter writer(opts.output_file);
  writer.write(results, reader.num_line, opts.n_layers);
}
