#include <cstring>
#include <iostream>
#include "glob.h"

static const char * get_base_name(const char* path) {
    const char *p = path + std::strlen(path) - 1;
    while (p > path && *p != '\\' && *p != '/') {
        p--;
    }
    return p + 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr
            << "Usage: " << get_base_name(argv[0]) << ' '
            << "pattern1 [pattern2 [...]]"
            << std::endl;
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        glob::glob glob(argv[i]);
        while (glob) {
            std::cout << glob.current_match() << std::endl;
            glob.next();
        }
    }
    return 0;
}
