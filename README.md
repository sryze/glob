Glob
====

[![Build Status][build_status]][build]

Glob is a simple cross-platform wrapper around system APIs for finding files
based on patterns.

Below is an example program:

```cpp
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
}
```

## TODO

* Custom glob parser with identical behavior across platforms

[build]: https://ci.appveyor.com/project/sryze/glob/branch/master
[build_status]: https://ci.appveyor.com/api/projects/status/mmy2ha5jqoi6dxa5/branch/master?svg=true
