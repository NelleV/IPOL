#include "csv.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

CSVReader::CSVReader(const char * file_name){
  this->file.open(file_name);
  this->index = 0;
  this->num_line = 0;
};

bool CSVReader::load_file(){
  this->file.seekg(0, ios::end);
  int length = this->file.tellg();
  this->file.read(this->line, length);
}

bool CSVReader::next(){
  this->file.getline(this->line, MAX_LENGTH);
  if(!this->file.eof()){
    this->num_line++;
    return true;
  }else{
    return false;
  }
}

bool CSVReader::parse_line(const string& delimiters){
  string str(this->line);
  string::size_type last_pos = str.find_first_not_of(delimiters, 0);
  string::size_type pos = str.find_first_of(delimiters, last_pos);
  string substring = str;

  while(string::npos != pos || string::npos != last_pos){
    substring = str.substr(last_pos, pos - last_pos);
    this->matrix.push_back(atof(substring.c_str()));
    last_pos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, last_pos);
    this->index++;
  }
  return true;
}

vector<double> CSVReader::parse_lines(const string& delimiters){
  while(this->next()){
    this->parse_line(delimiters);
  }
  return this->matrix;
}


CSVWriter::CSVWriter(const char * file_name){
  this->file.open(file_name);
}

CSVWriter::~CSVWriter(){
  this->file.close();
}

bool CSVWriter::write(unsigned int result[], unsigned int num_line){
  for(unsigned int i = 0; i < num_line; i++){
    cout << i <<  " " << result[i] << endl;
    char buffer[100];
    sprintf(buffer, "%u\n\0", result[i]);
    unsigned int j = 0;
    while(buffer[j] != '\0'){
      j++;
    }
    this->file.write(buffer, j);
  }
}
