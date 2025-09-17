///
/// \file options.cpp
/// \author Felix Dilly
/// \date Created at: 2025-09-17
/// \date Last modified at: 2025-09-17
/// ---
///

#include "options.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

void FinderOptions::setRecursive(bool rec) { mRec = rec; }

bool FinderOptions::getRecursive() const { return mRec; }

void FinderOptions::setCaseInsensitive(bool cis) { mCis = cis; }

bool FinderOptions::getCaseInsensitive() const { return mCis; }

void FinderOptions::setStartDirectory(std::string startDir) {
  if ("." == startDir) {
    mStartDir = fs::current_path();
  } else if (".." == startDir) {
    mStartDir = fs::current_path().parent_path();
  } else {
    mStartDir = startDir;
  }
}

std::string FinderOptions::getStartDirectory() const { return mStartDir; }