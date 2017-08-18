#include "fntParser.h"
#include <algorithm>

void fntParser::parseFile(const char * filename)
{
    lines.clear();

    std::ifstream ifs(filename);
    std::string line;

    if (!ifs)
    {
        return;
    }

    while (std::getline(ifs, line))
    {
        parseLine(line);
    }
}

void fntParser::parseLine(const std::string & line)
{
    std::vector<std::string> elements;

    auto headit = line.begin();
    auto endit = line.begin();


}

void fntParser::convertData()
{

}
