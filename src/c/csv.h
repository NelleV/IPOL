#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class CSVReader{
    public:
        CSVReader(std::string file);

        int num_line;

        std::vector<double> read();
        bool parse_line(const std::string& delimiters);

    private:
        std::string filename;
        std::string line;
        std::vector<double> matrix;
        int index;

};

class CSVWriter {
  public:
    CSVWriter(const char * file_name);
    ~CSVWriter();
    bool write(std::vector <std::vector<unsigned int> > result,
               unsigned int num_line,
               unsigned int n_layers);

  private:
    std::ofstream file;
};
