#pragma once
#include <iostream>
#include <string>
namespace mesh_processing
{
    // Utility functions for file input/output operations

    // io manipulation
    bool seek_keyword(FILE *f, const char *keyword);
    bool next_is_keyword(FILE *f, const char *keyword);
    bool eat_word(FILE *f, char *word);
    bool eat_double(FILE *f, double &d);
    bool eat_int(FILE *f, int &i);
    bool eat_uint(FILE *f, unsigned int &i);

    bool TrimSpaceHeadAndTail(std::string &str);
    bool TrimComment(std::string &str, const std::string &commentMark = "#");
    bool TrimLine(std::string &str);
    bool ReadNextTrimLine(std::ifstream &file, std::string &line);



    // string
    std::string GetFileExtension(const std::string &s);
    std::string GetFilePath(const std::string &s, const bool with_basename = false);
    std::string GetFileName(const std::string &s, const bool with_extension = true);

}