#ifndef GLOB_H
#define GLOB_H

#include <string>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
#endif

namespace glob {

class glob
{
public:
    glob(const std::string &pattern);
    ~glob();

    std::string current_match() const;
    bool next();
    bool is_valid() const;

    operator bool() const {
        return is_valid();
    }

private:
    glob(const glob &) = delete;
    void operator=(const glob &) = delete;

private:
#ifdef _WIN32
    HANDLE find_handle_;
    WIN32_FIND_DATA find_data_;
    bool ok_;
#else
    std::string pattern_;
    DIR *dir_;
    struct dirent *dir_entry_;
#endif
};

} // namespace glob

#endif // !GLOB_H
