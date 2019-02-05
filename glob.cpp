#include "glob.h"
#ifndef _WIN32
  #include <fnmatch.h>
#endif

namespace glob {

#ifdef _WIN32

glob::glob(const std::string &pattern)
 : ok_(false),
   find_handle_(INVALID_HANDLE_VALUE)
{
  find_handle_ = FindFirstFileA(pattern.c_str(), &find_data_);
  ok_ = find_handle_ != INVALID_HANDLE_VALUE;
}

glob::~glob() {
  if (find_handle_ != INVALID_HANDLE_VALUE) {
    FindClose(find_handle_);
  }
}

std::string glob::current_match() const {
  return find_data_.cFileName;
}

bool glob::is_valid() const {
  return ok_;
}

bool glob::next() {
  ok_ = FindNextFileA(find_handle_, &find_data_) != 0;
  return ok_;
}

#else // _WIN32

namespace {

std::pair<std::string, std::string> split_path(const std::string &path) {
  std::string::size_type last_sep = path.find_last_of("/");
  if (last_sep != std::string::npos) {
    return std::make_pair(
      std::string(path.begin(), path.begin() + last_sep),
      std::string(path.begin() + last_sep + 1, path.end()));
  }
  return std::make_pair(".", path);
}

} // namespace

glob::glob(const std::string &pattern)
 : dir_(nullptr),
   dir_entry_(nullptr)
{
  std::pair<std::string, std::string> dir_and_mask = split_path(pattern);

  dir_ = opendir(dir_and_mask.first.c_str());
  pattern_ = dir_and_mask.second;

  if (dir_ != nullptr) {
    next();
  }
}

glob::~glob() {
  if (dir_ != nullptr) {
    closedir(dir_);
  }
}

std::string glob::current_match() const {
  return dir_entry_->d_name;
}

bool glob::is_valid() const {
  return dir_ != nullptr && dir_entry_ != nullptr;
}

bool glob::next() {
  while ((dir_entry_ = readdir(dir_)) != nullptr) {
    if (!fnmatch(pattern_.c_str(), dir_entry_->d_name,
                 FNM_CASEFOLD | FNM_NOESCAPE | FNM_PERIOD)) {
      return true;
    }
  }
  return false;
}

#endif // !_WIN32

} // namespace glob
