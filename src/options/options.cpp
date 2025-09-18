///
/// \file options.cpp
/// \author Felix Dilly
/// \date Created at: 2025-09-17
/// \date Last modified at: 2025-09-17
/// ---
///

#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <variant>

#include "options.hpp"

namespace fs = std::filesystem;

void FinderOptions::setRecursive(bool rec) { mRec = rec; }

bool FinderOptions::getRecursive() const { return mRec; }

void FinderOptions::setCaseInsensitive(bool cis) { mCis = cis; }

bool FinderOptions::getCaseInsensitive() const { return mCis; }

void FinderOptions::setStartDirectory(std::string startDir) {
  auto length = startDir.size();
  if (1 == length) {
    isCurrentPath(startDir);
  } else if (2 == length) {
    isParentPath(startDir);
  } else {
    isValidPath(startDir);
  }
}

std::string FinderOptions::getStartDirectory() const { return mStartDir; }

void FinderOptions::isCurrentPath(std::string path) {
  if (path.starts_with(".")) {
    mStartDir = fs::current_path();
  } else if (path.starts_with("/")) {
    mStartDir = path;
  } else {
    throw std::invalid_argument("Invalid path");
  }
}

void FinderOptions::isParentPath(std::string path) {
  if (path.starts_with("..")) {
    mStartDir = fs::current_path().parent_path();
  } else if (path.starts_with("./")) {
    mStartDir = fs::current_path();
  } else {
    throw std::invalid_argument("Invalid path");
  }
}

void FinderOptions::isValidPath(std::string path) {
  mStartDir = fs::current_path();
  std::size_t index = path.find_first_not_of("../");
  int parent_count;
  std::string suffix;

  if (std::variant_npos != index) {
    parent_count = path.substr(0, index).size() / 3;
    suffix = path.substr(index);
  } else {
    parent_count = path.size() / 3;
  }

  for (int i = 0; i < parent_count; i++) {
    mStartDir = mStartDir.parent_path();
  }

  if (!suffix.empty()) {
    mStartDir = mStartDir.string() + "/" + suffix;
  }
}