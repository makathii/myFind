///
/// \file finder.cpp
/// \author Felix Dilly Katharina Markus
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

#include <cctype>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <system_error>
#include <unistd.h> //for fork()
#include <vector>
#include <sys/msg.h>
#include <cstring>
#include <cerrno>

#include "../options/options.hpp"
#include "finder.hpp"

namespace fs = std::filesystem;

struct message_t{
  long mtype;
  char mtext[1024];
};

/// @brief stores options, file descriptor and filename -> in case of case
///        insensitive option converts filename to lowercase
///
Finder::Finder(std::shared_ptr<FinderOptions> opts, std::string &filename,
                int msgid)
    : mOpts(opts), mFilename(filename), mMsgid(msgid) {
  if (mOpts->getCaseInsensitive()) {
    stringToLower(mFilename);
  }
} // namespace

/// @brief selects correct find method depending on recursive option
///
void Finder::search() {
  try{
    if (mOpts->getRecursive()) {
      recFind();
    } else {
      Find();
    }
}catch(const std::filesystem::filesystem_error& e){
  std::cerr<<"Filesystem error in process: "<<getpid()<<": "<<e.what()<<std::endl;
  }
  //indicate that this child is done
  //printf("Child %d done\n", getpid());  //proper debugging is for losers
  sendMessage("END");
}

/// @brief find method that recurses into subdirectories
///
void Finder::recFind() {
  std::error_code err;

  // recursive directory iterator
  // skipping directories with missing permissions
  for (const auto &entry : fs::recursive_directory_iterator(
           mOpts->getStartDirectory(),
           fs::directory_options::skip_permission_denied, err)) {
    // if any unexpected errors happen skip
    if (err) {
      std::cerr << "Cannot open directory " << mOpts->getStartDirectory()
                << ": " << err.message() << std::endl;
      continue;
    }

    // get filename and try to match
    std::string currentFile = entry.path().filename().string();
    if (Match(currentFile)) {
      // create message and write to pipe
      std::ostringstream msg;
      msg << getpid() << ": " << mFilename << ": " << entry.path().string()
          << "\n";
      sendMessage(msg.str());
    }
  }
}

/// @brief creates a lower case string
///
void Finder::stringToLower(std::string &str) {
  for (auto &ch : str) {
    ch = std::tolower(ch);
  }
}

/// @brief matches given string to filename to find
///        converts given string to lower case if option is active
///
bool Finder::Match(std::string &filename) {
  if (mOpts->getCaseInsensitive()) {
    stringToLower(filename);
  }

  return std::ranges::equal(mFilename, filename,
                            [](char a, char b) { return a == b; });
}

/// @brief find method that stays in working directory
///
void Finder::Find() {
  std::error_code err;

  // directory iterator
  // skips with missing permissions
  for (const auto &entry : fs::directory_iterator(
           mOpts->getStartDirectory(),
           fs::directory_options::skip_permission_denied, err)) {

    // get filename and try to match
    std::string currentFile = entry.path().filename().string();
    if (Match(currentFile)) {
      // create message and write to (pipe) actually message queueueue
      std::ostringstream msg;
      msg << getpid() << ": " << mFilename << ": " << entry.path().string()
          << "\n";
      sendMessage(msg.str());
      }
  }
}

/// @brief sends a message to the message queue
///
void Finder::sendMessage(const std::string& message){
  message_t msg;
  msg.mtype = 1; 
  if(message.size()<sizeof(msg.mtext)){   //to ensure message fits into buffer
    strncpy(msg.mtext, message.c_str(), sizeof(msg.mtext)-1);
    msg.mtext[sizeof(msg.mtext)-1] = '\0'; //ensure null termination
    
    if(msgsnd(mMsgid, &msg, strlen(msg.mtext)+1, 0) == -1){
      std::cerr<<"Failed to send message to queue: "<<strerror(errno)<<std::endl;
    }
  }else{
    strncpy(msg.mtext,message.c_str(), sizeof(msg.mtext)-1);
    msg.mtext[sizeof(msg.mtext)-1] = '\0'; //ensure null

    if(msgsnd(mMsgid, &msg, strlen(msg.mtext)+1, 0) == -1){
      std::cerr<<"Failed to send message to queue: "<<strerror(errno)<<std::endl;
    }
  }
}