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
#include <system_error>
#include <variant>

#include "options.hpp"

namespace fs = std::filesystem;

void FinderOptions::setRecursive(bool rec) { mRec = rec; }

bool FinderOptions::getRecursive() const { return mRec; }

void FinderOptions::setCaseInsensitive(bool cis) { mCis = cis; }

bool FinderOptions::getCaseInsensitive() const { return mCis; }

bool FinderOptions::setStartDirectory(std::string startDir) {
  fs::path path = startDir;
  try {
    if (path.is_absolute()) {
      mStartDir = fs::absolute(path);
    } else {
      mStartDir = fs::canonical(path);
    }
    return true;
  } catch (fs::filesystem_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
}

std::string FinderOptions::getStartDirectory() const { return mStartDir; }
