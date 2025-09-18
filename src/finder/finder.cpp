///
/// \file finder.cpp
/// \author Felix Dilly
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <string>
#include <unistd.h> //for fork()
#include <vector>

#include "../options/options.hpp"
#include "finder.hpp"

namespace fs = std::filesystem;

Finder::Finder(std::shared_ptr<FinderOptions> opts, std::string filename)
    : mOpts(opts), mFilename(filename) {
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
  try {
    for (const auto &entry : fs::recursive_directory_iterator(
             mOpts->getStartDirectory())) { // recursive directory iterator
      std::string currentFile = entry.path().filename().string();

      if (Match(currentFile)) {
        std::cout << getpid() << ": " << mFilename << ": "
                  << entry.path().string() << "\n";
      }
    }
  } catch (fs::filesystem_error &e) {
    std::cerr << "myfind failed: " << e.what() << std::endl;
  }
}

void Finder::stringToLower(std::string &str) {
  for (char &ch : str) {
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
  try {
    for (const auto &entry :
         fs::directory_iterator(mOpts->getStartDirectory())) {

      // I know we basically have the same code twice,
      std::string currentFile = entry.path().filename().string();

      // but I struggled to make it work with a seperate function
      if (Match(currentFile)) {
        std::cout << getpid() << ": " << mFilename << ": "
                  << entry.path().string() << "\n";
      }
    }
  } catch (fs::filesystem_error &e) {
    std::cerr << "myfind failed: " << e.what() << std::endl;
  }
}
