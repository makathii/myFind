///
/// \file options.hpp
/// \author Felix Dilly
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#pragma once

#include <string>

struct FinderOptions {
  bool rec; // recursive flag
  bool cis; // case insesitive flag
  std::string startDir;
};