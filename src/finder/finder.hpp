///
/// \file finder.hpp
/// \author Felix Dilly Katharina Markus
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#pragma once

#include "../options/options.hpp"
#include <cstdio>
#include <memory>
#include <ostream>
#include <vector>

/// @class object with the sole task to find file(s) from a specified root path
class Finder {
public:
  Finder(std::shared_ptr<FinderOptions> opts, std::string &filename,
         FILE *output);
  ~Finder() = default;

  void search();

private:
  std::shared_ptr<FinderOptions> mOpts;
  std::string &mFilename;
  FILE *mOutput;

  void stringToLower(std::string &str);
  bool Match(std::string &filename);
  void recFind();
  void Find();
};