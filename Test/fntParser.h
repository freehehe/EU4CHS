#pragma once
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <map>
#include <variant>
#include <cstdint>

struct lineMeta
{
    std::string prefix;
    std::map<std::string, std::string> attributes;
};

struct charValue
{
    std::uint16_t x;
    std::uint16_t y;
    std::uint16_t width;
    std::uint16_t height;
    std::uint16_t xoffset;
    std::uint16_t yoffset;
    std::uint16_t xadvance;
    std::uint16_t page;
};

union charPair
{
    struct
    {
        std::uint16_t first;
        std::uint16_t second;
    };

    std::uint32_t packed;
};

struct fntMeta
{
    std::map<std::uint16_t, charValue> values;
    std::map<std::uint32_t, int> kernings;
};

class fntParser
{
public:
    void parseFile(const char *filename);

private:
    void parseLine(const std::string &line);
    void convertData();

    std::vector<lineMeta> lines;
};
