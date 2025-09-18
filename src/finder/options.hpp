///
/// \file options.hpp
/// \author Felix Dilly
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#pragma once

#include <filesystem>
#include <string>

class FinderOptions {
public:
  FinderOptions() = default;
  ~FinderOptions() = default;

  void setRecursive(bool rec);
  bool getRecursive() const;
  void setCaseInsensitive(bool cis);
  bool getCaseInsensitive() const;
  void setStartDirectory(std::string startDir);
  std::string getStartDirectory() const;

private:
  bool mRec = false; // recursive flag
  bool mCis = false; // case insesitive flag
  std::filesystem::path mStartDir;

  void isCurrentPath(std::string path);
  void isParentPath(std::string path);
  void isValidPath(std::string path);
};