#include <cstring>
#include <iostream>

using namespace std;

#include "affinity_propagation.h"
#include "globals.h"
#include "csv.h"

struct options{
  char file_name[FILENAME_MAX];
  int lambda;
};

void print_usage();

struct options get_default_options() {
  struct options opts;
  opts.lambda = 0.9;

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
      default:
          printf("%s is not recognized as an argument\n", argv[i]);
          print_usage();
          exit(0);
    }
  }
  strcpy(opts->file_name, argv[i]);
}

void print_usage() {
  printf("Usage: ac -l<lambda> \n"
         "          -h prints this help\n"
         "          file_name\n");
}

int main(int argc, char ** argv){
  struct options opts = get_default_options();
  get_args(&opts, argc, argv);
  cout << "Reading input file" << endl << flush;
  CSVReader reader(opts.file_name);

  double * matrix = reader.parse_lines(" ");

  unsigned int results [reader.num_line];
  affinity_propagation(matrix, reader.num_line, results, 0.5, 200);
  cout << "here" << endl;
  CSVWriter writer("output.csv");
  writer.write(results, reader.num_line);
}
