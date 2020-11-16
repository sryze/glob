#include <iostream>
#include "glob.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " pattern1 [pattern2 [...]]"
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
