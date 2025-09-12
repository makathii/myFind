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
#include <string>
#include <unistd.h> //for fork()
#include <vector>

#include "finder.hpp"
#include "options.hpp"

Finder::Finder(std::shared_ptr<FinderOptions> opts, std::string filename)
    : mOpts(opts), mFilename(filename) {
  if (mOpts->cis) {
    stringToLower(mFilename);
  }
}

void Finder::search() {
  if (mOpts->rec) {
    recFind();
  } else {
    Find();
  }
}

void Finder::recFind() {
  for (const auto &entry : std::filesystem::recursive_directory_iterator(
           mOpts->startDir)) { // recursive directory iterator
    std::string currentFile = entry.path().filename().string();

    if (Match(currentFile)) {
      std::cout << getpid() << ": " << mFilename << ": "
                << entry.path().string() << "\n";
    }
  }
}

void Finder::stringToLower(std::string &str) {
  for (char &ch : str) {
    ch = std::tolower(ch);
  }
}

bool Finder::Match(std::string &filename) {
  if (mOpts->cis) {
    stringToLower(filename);
  }

  return std::ranges::equal(mFilename, filename,
                            [](char a, char b) { return a == b; });
}

void Finder::Find() {
  for (const auto &entry :
       std::filesystem::directory_iterator(mOpts->startDir)) {

    // I know we basically have the same code twice,
    std::string currentFile = entry.path().filename().string();

    // but I struggled to make it work with a seperate function
    if (Match(currentFile)) {
      std::cout << getpid() << ": " << mFilename << ": "
                << entry.path().string() << "\n";
    }
  }
}
