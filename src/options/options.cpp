///
/// \file options.cpp
/// \author Felix Dilly Katharina Markus
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

/// @brief sets the signal for recursive find
///
void FinderOptions::setRecursive() { mRec = true; }

/// @brief get the signal for recursive find
///
bool FinderOptions::getRecursive() const { return mRec; }

/// @brief activates the signal for cas insesitive find
//
void FinderOptions::setCaseInsensitive() { mCis = true; }

/// @brief get the signal for case insensitie find
//
bool FinderOptions::getCaseInsensitive() const { return mCis; }

/// @brief tries to parse the path for the starting directory
/// @param[in] startDir string of the desired path to use find in
///
bool FinderOptions::setStartDirectory(std::string startDir) {
  // create path from string
  fs::path path = startDir;
  try {
    // find out if path is absolute
    if (path.is_absolute()) {
      mStartDir = fs::absolute(path);
    } else {
      // create relative path from current working directory
      mStartDir = fs::canonical(path);
    }

    return true;
  } catch (fs::filesystem_error &e) {
    // catch any path creation errors
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
}

/// @brief get the path of the starting directory
///
std::string FinderOptions::getStartDirectory() const { return mStartDir; }
