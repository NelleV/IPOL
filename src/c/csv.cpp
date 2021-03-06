#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include "csv.h"

using namespace std;

CSVReader::CSVReader(string file)
{
    this->filename = file;
    this->num_line = 0;
}

vector<double> CSVReader::read(){
    ifstream csvfile;
    csvfile.open(this->filename.c_str(), ifstream::in);
    string delimiters(" ");

    if(csvfile.is_open()){
        while(!csvfile.eof()){
            this->line = "";
            getline(csvfile, this->line);
            parse_line(delimiters);
        }
        csvfile.close();
    }else{
      cout << "problem reading file" << endl << flush;
      exit(0);
    }
    this->num_line--;
    return this->matrix;
}

bool CSVReader::parse_line(const string& delimiters){
  string str(this->line);
  this->num_line++;
  string::size_type last_pos = str.find_first_not_of(delimiters, 0);
  string::size_type pos = str.find_first_of(delimiters, last_pos);
  string substring = str;

  while(string::npos != pos || string::npos != last_pos){
    substring = str.substr(last_pos, pos - last_pos);
    this->matrix.push_back(atof(substring.c_str()));
    last_pos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, last_pos);
  }
  return true;
}

CSVWriter::CSVWriter(const char * file_name){
  this->file.open(file_name);
}

CSVWriter::~CSVWriter(){
  this->file.close();
}

bool CSVWriter::write(vector< vector<unsigned int> > result,
                      unsigned int num_line,
                      unsigned int n_layers){

  for(unsigned int i = 0; i < num_line; i++){
    for(unsigned int l = 0; l < n_layers; l++){
      char buffer[100];
      if(l == n_layers - 1){
        sprintf(buffer, "%u\n\0", result[l][i]);
      }else{
        sprintf(buffer, "%u \0", result[l][i]);
      }
      unsigned int j = 0;
      while(buffer[j] != '\0'){
        j++;
      }

      this->file.write(buffer, j);
    }
  }
}
