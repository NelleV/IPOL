#include <fstream>
#include <string>
#include <vector>
#define MAX_LENGTH 999

class CSVReader {
public:
    CSVReader(const char * file_name);
    bool next();
    bool parse_line(const std::string & delimiters);
    std::vector<double> parse_lines(const std::string & delimiters);
    unsigned int num_line;
private:
    bool load_file();
    std::ifstream file;
    char line[MAX_LENGTH];
    std::vector<double> matrix;
    unsigned long index;
};

class CSVWriter {
  public:
    CSVWriter(const char * file_name);
    ~CSVWriter();
    bool write(unsigned int result[], unsigned int num_line);

  private:
    std::ofstream file;
};
