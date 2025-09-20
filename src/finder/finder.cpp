///
/// \file finder.cpp
/// \author Felix Dilly Katharina Markus
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#include <cctype>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <system_error>
#include <unistd.h> //for fork()
#include <vector>

#include "../options/options.hpp"
#include "finder.hpp"

namespace fs = std::filesystem;

Finder::Finder(std::shared_ptr<FinderOptions> opts, std::string &filename,
               FILE *output)
    : mOpts(opts), mFilename(filename), mOutput(output) {
  if (mOpts->getCaseInsensitive()) {
    stringToLower(mFilename);
  }
} // namespace

void Finder::search() {
  if (mOpts->getRecursive()) {
    recFind();
  } else {
    Find();
  }
}

void Finder::recFind() {
  std::error_code err;
  // recursive directory iterator
  for (const auto &entry : fs::recursive_directory_iterator(
           mOpts->getStartDirectory(),
           fs::directory_options::skip_permission_denied, err)) {
    if (err) {
      std::cerr << "Cannot open directory " << mOpts->getStartDirectory()
                << ": " << err.message() << std::endl;
      continue;
    }
    std::string currentFile = entry.path().filename().string();

    if (Match(currentFile)) {
      std::ostringstream msg;
      msg << getpid() << ": " << mFilename << ": " << entry.path().string()
          << "\n";
      fprintf(mOutput, msg.str().c_str());
    }
  }
}

void Finder::stringToLower(std::string &str) {
  for (auto &ch : str) {
    ch = std::tolower(ch);
  }
}

bool Finder::Match(std::string &filename) {
  if (mOpts->getCaseInsensitive()) {
    stringToLower(filename);
  }

  return std::ranges::equal(mFilename, filename,
                            [](char a, char b) { return a == b; });
}

void Finder::Find() {
  std::error_code err;
  for (const auto &entry : fs::directory_iterator(
           mOpts->getStartDirectory(),
           fs::directory_options::skip_permission_denied, err)) {

    // I know we basically have the same code twice,
    std::string currentFile = entry.path().filename().string();

    // but I struggled to make it work with a seperate function
    if (Match(currentFile)) {
      std::ostringstream msg;
      msg << getpid() << ": " << mFilename << ": " << entry.path().string()
          << "\n";
      fprintf(mOutput, msg.str().c_str());
    }
  }
}
