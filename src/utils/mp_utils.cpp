#include "utils/mp_utils.h"
#include <cstring>

namespace mesh_processing
{
    bool seek_keyword(FILE *f, const char *keyword)
    {
        static char s[4097];
        s[0] = '\0';
        do
        {
            fscanf(f, "%4096s", s);
        } while (strcmp(s, keyword) && !feof(f));

        if (feof(f))
            return false;
        return true;
    }

    bool next_is_keyword(FILE *f, const char *keyword)
    {
        if (feof(f))
            return false;
        static char s[256];
        s[0] = '\0';
        fscanf(f, "%255s", s);
        if (strcmp(s, keyword))
            return true;
        return false;
    }

    bool eat_word(FILE *f, char *word)
    {
        word[0] = '\0';
        if (feof(f))
            return false;
        fscanf(f, "%s", word);
        return true;
    }

    bool eat_double(FILE *f, double &d)
    {
        if (feof(f))
            return false;
        fscanf(f, "%lf", &d);
        return true;
    }

    bool eat_int(FILE *f, int &i)
    {
        if (feof(f))
            return false;
        fscanf(f, "%d", &i);
        return true;
    }

    bool eat_uint(FILE *f, unsigned int &i)
    {
        if (feof(f))
            return false;
        fscanf(f, "%d", &i);
        return true;
    }

    std::string GetFileExtension(const std::string &s)
    {
        size_t pos = s.find_last_of(".");
        if (pos >= s.size())
            return "";
        return s.substr(pos + 1);
    }

    std::string GetFilePath(const std::string &s, const bool with_basename)
    {
        size_t pos = (with_basename) ? s.find_last_of(".") : s.find_last_of("/");
        if (pos >= s.size())
            return "./";
        if (with_basename)
            return s.substr(0, pos);
        return s.substr(0, pos + 1);
    }

    std::string GetFileName(const std::string &s, const bool with_extension)
    {
        size_t pos = s.find_last_of("/");
        std::string ss = (pos >= s.size()) ? s : s.substr(pos + 1);
        if (with_extension)
            return ss;
        pos = ss.find_last_of(".");
        return ss.substr(0, pos);
    }

    


} // namespace mesh_processing