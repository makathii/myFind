///
/// \file main.cpp
/// \author Katharina Markus, Felix Dilly
/// \date Created at: 2025-09-12
/// \date Last modified at: 2025-09-12
/// ---
///

/*  first, getopt parses through the arguments given & assigns some variables to
   check for then, we fork a child process for each filename we got. Each child
   process runs the search_f() function which checks if recursive or not & then
   searches the path accordingly. The parent process waits for all children to
   finish before exiting.
*/

// what still needs to be done:
//-error handling for invalid search path
//-make sure the output is always correct (I only did a little testing)
//-optimize the code (maybe make search_f() less redundant?)

// on that note: what is the correct way to sort includes? I just put them in
// alphabetic order
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <ostream>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h> //for fork()
#include <vector>

#include "finder/finder.hpp"
#include "options/options.hpp"

//message queueueue structure
struct message_t{
  long mtype;
  char mtext[1024];
};

//hit me for this
static int s_msgid=-1;

//Signal handler to clean up message queueueue
void signalHandler(int signum){
  if(s_msgid != -1){
    msgctl(s_msgid, IPC_RMID, nullptr);
  }
  _exit(signum);  //cause we no no want recursive cleanie weanies
}

int main(int argc, char *argv[]) {
  int opt;
  auto opts = std::make_shared<FinderOptions>(); // create options object

  // Parse myfind options
  while ((opt = getopt(argc, argv, "Ri")) != -1) {
    switch (opt) {
    case 'R':
      opts->setRecursive();
      break;
    case 'i':
      opts->setCaseInsensitive();
      break;
    case '?':
      std::cerr
          << "Usage: " << argv[0]
          << " [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n";
      return EXIT_FAILURE;
    default:
      abort();
    }
  }

  // check if required arguments are given
  if (argc - optind < 2) {
    std::cerr << "Usage: " << argv[0]
              << " [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n";
    return 1;
  }

  // try to resolve starting directory
  try {
    if (!opts->setStartDirectory(argv[optind])) {
      return 1;
    }
  } catch (std::invalid_argument &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  // collect files to find
  std::vector<std::string> filenames;
  for (int i = optind + 1; i < argc; i++) {
    filenames.push_back(argv[i]);
  }

  //create message queue
  key_t key = ftok("/tmp", 65); // generate unique key
  if(key==-1){
    std::cerr<<"Failed to generate key for message queue"<<std::endl;
    key = 12345;  //this is not good but should work if only this program uses it
  }

  int msgid = msgget(key, 0666 | IPC_CREAT);
  if(msgid == -1){
    std::cerr<<"Failed to create message queue: "<<strerror(errno)<<std::endl;
    return 1;
  }

  //signal handling for cleanies
  s_msgid=msgid;
  struct sigaction sa;
  sa.sa_handler = signalHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);

  // Spawn children processes
  std::vector<pid_t> children;
  for (auto &f : filenames) {
    pid_t pid = fork();
    if (pid == 0) {
      //
      // Child process
      //
      signal(SIGINT, SIG_DFL); //reset signal handling for child
      signal(SIGTERM, SIG_DFL);

      Finder finder(opts, f, msgid);  
      finder.search();

      exit(EXIT_SUCCESS); // child process exits here
    } else if (pid < 0) {
      std::cerr << "Failed to spawn child" << std::endl;
      msgctl(msgid, IPC_RMID, nullptr); //clean up message queue
      return 1;
    } else if (pid > 0) { // parent process
      children.push_back(pid);
    }
  }

  //
  // Parent process
  //
  
  //recieve messages from queueue
  message_t message;
  int active_children = children.size();
  bool found=false;

  while(active_children>0){
    ssize_t bytes_received = msgrcv(msgid, &message, sizeof(message.mtext), 1, 0);

    if(bytes_received>0){
      //check for termination message
      if(strcmp(message.mtext, "END")==0){
        active_children--;
      }else{
        std::cout<<message.mtext<<std::endl;
        found=true;
      }
  }else if(bytes_received == -1){
    if(errno==EIDRM || errno==EINTR){
      break; //message queue deleted or interrupted by signal
    }else{
      std::cerr<<"Failed to receive message from queue: "<<strerror(errno)<<std::endl;
      break;
    }
  }
}

  // clean up child processes
  for (pid_t pid : children) {
    int status;
    waitpid(pid, &status, 0); //parent waits for each child to finish
  }

  //clean up the message after all bebes are done
  if(msgctl(msgid, IPC_RMID, nullptr) == -1){
    if(errno != EIDRM){ //ignore if already deleted
      std::cerr<<"Failed to delete message queue: "<<strerror(errno)<<std::endl;
    }
  }

  if(!found){
    std::cout<<"No files found"<<std::endl;
  }

  //reset the shameful global
  s_msgid=-1;
  return 0;
}
