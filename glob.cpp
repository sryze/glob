#include "glob.h"
#ifdef _WIN32
    #include <windows.h>
    #define GLOB_WINDOWS
#else
    #include <dirent.h>
    #include <fnmatch.h>
#endif

namespace glob {

#ifdef GLOB_WINDOWS

class glob_impl
{
public:
    glob_impl():
        find_handle(INVALID_HANDLE_VALUE),
        find_data(),
        has_next(false)
    {}

    HANDLE find_handle;
    WIN32_FIND_DATAA find_data;
    bool has_next;
};

#else // GLOB_WINDOWS

class glob_impl
{
public:
    glob_impl():
        dir(nullptr),
        dir_entry(nullptr)
    {}

    std::string file_pattern;
    DIR *dir;
    struct dirent *dir_entry;
};

namespace {

std::pair<std::string, std::string> split_path(const std::string &path)
{
    std::string::size_type last_sep = path.find_last_of("/");
    if (last_sep != std::string::npos) {
        return std::make_pair(
            std::string(path.begin(), path.begin() + last_sep),
            std::string(path.begin() + last_sep + 1, path.end()));
    }
    return std::make_pair(".", path);
}

} // anonymous namespace

#endif // !GLOB_WINDOWS

glob::glob(const std::string &pattern):
    impl_(std::make_unique<glob_impl>())
{
    open(pattern);
}

glob::~glob()
{
    close();
}

void glob::open(const std::string &pattern)
{
#ifdef GLOB_WINDOWS
    impl_->find_handle =
        FindFirstFileA(pattern.c_str(), &impl_->find_data);
    impl_->has_next = impl_->find_handle != INVALID_HANDLE_VALUE;
#else
    auto dir_and_file = split_path(pattern);
    impl_->dir = opendir(dir_and_file.first.c_str());
    impl_->file_pattern = dir_and_file.second;

    if (impl_->dir != nullptr) {
        next();
    }
#endif
}

void glob::close()
{
#ifdef GLOB_WINDOWS
    if (impl_->find_handle != INVALID_HANDLE_VALUE) {
        FindClose(impl_->find_handle);
        impl_->find_handle = INVALID_HANDLE_VALUE;
        impl_->has_next = false;
    }
#else
    if (impl_->dir != nullptr) {
        closedir(impl_->dir);
        impl_->dir = nullptr;
    }
#endif
}

std::string glob::current_match() const
{
#ifdef GLOB_WINDOWS
    return impl_->find_data.cFileName;
#else
    return impl_->dir_entry->d_name;
#endif
}

bool glob::is_valid() const
{
#ifdef GLOB_WINDOWS
    return impl_->has_next;
#else
    return impl_->dir != nullptr && impl_->dir_entry != nullptr;
#endif
}

bool glob::next()
{
#ifdef GLOB_WINDOWS
    impl_->has_next =
        FindNextFileA(impl_->find_handle, &impl_->find_data) != 0;
    return impl_->has_next;
#else
    while ((impl_->dir_entry = readdir(impl_->dir)) != nullptr) {
        if (!fnmatch(impl_->file_pattern.c_str(),
                    impl_->dir_entry->d_name,
                    FNM_CASEFOLD | FNM_NOESCAPE | FNM_PERIOD)) {
            return true;
        }
    }
    return false;
#endif
}

} // namespace glob
