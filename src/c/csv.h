#include <fstream>
#include <string>


#define MAX_LENGTH 999


class CSVReader {
public:
    CSVReader(const char * inputFile);
    bool next();
    bool parse_line(const std::string & delimiters);
    double * parse_lines(const std::string & delimiters);
    unsigned int num_line;
private:
    bool load_file();
    std::ifstream file;
    char line[MAX_LENGTH];
    double matrix[MAX_LENGTH * MAX_LENGTH];
    unsigned long index;
};
