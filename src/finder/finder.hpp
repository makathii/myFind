///
/// \file finder.hpp
/// \author Felix Dilly
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#pragma once

#include "options.hpp"
#include <memory>
#include <vector>

class Finder {
public:
  Finder(std::shared_ptr<FinderOptions> opts, std::string filename);
  ~Finder() = default;

  void search();

private:
  std::shared_ptr<FinderOptions> mOpts;
  std::string mFilename;

  void stringToLower(std::string &str);
  bool Match(std::string &filename);
  void recFind();
  void Find();
};