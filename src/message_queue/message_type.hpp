///
/// \file message_type.hpp
/// \author Felix Dilly
/// \date Created at: 2025-09-26
/// \date Last modified at: 2025-09-26
/// ---
///

#pragma once

#include <limits.h>

// message queueueue structure
struct message_t {
  long mType;
  int mPid;
  char mFilename[NAME_MAX + 1];
  char mPath[1024];
};