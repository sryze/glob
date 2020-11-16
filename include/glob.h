#ifndef GLOB_H
#define GLOB_H

#include <memory>
#include <string>

namespace glob {

enum class glob_flavor {
    //!
    // Uses native OS-specific APIs: FindFirstFile() on Windows and fnmatch()
    // elsewhere.
    //
    // Features may vary depending on the implementation. Generally supports
    // the following syntax elements:
    // 
    // `*` - match one or more characters
    // TODO: list other syntax features
    //
    native,
    //
    // Custom implementation with extended features.
    // TODO: documentation
    // 
    extended
};

class glob_impl;

class glob final
{
public:
    glob(const std::string &pattern, glob_flavor flavor = glob_flavor::native);
    ~glob();

    operator bool() const {
        return is_valid();
    }

    void open(const std::string &pattern);
    void close();

    std::string current_match() const;
    bool next();
    bool is_valid() const;

private:
    glob(const glob &) = delete;
    void operator=(const glob &) = delete;

private:
    std::unique_ptr<glob_impl> impl_;
};

} // namespace glob

#endif // !GLOB_H
