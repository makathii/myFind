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
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h> //for fork()
#include <vector>

#include "finder/finder.hpp"
#include "options/options.hpp"

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

  // Create pipe
  int fd[2];
  if (pipe(fd) < 0) {
    std::cerr << "Failed to create pipe" << std::endl;
    return 1;
  }

  // Spawn children processes
  std::vector<pid_t> children;
  for (auto &f : filenames) {
    pid_t pid = fork();
    if (pid == 0) {
      //
      // Child process
      //

      close(fd[0]);
      FILE *pipe_output = fdopen(fd[1], "w");

      Finder finder(opts, f, pipe_output);
      finder.search();

      exit(EXIT_SUCCESS); // child process exits here
    } else if (pid < 0) {
      std::cerr << "Failed to spawn child" << std::endl;
      return 1;
    } else if (pid > 0) { // parent process
      children.push_back(pid);
    }
  }

  //
  // Parent process
  //
  close(fd[1]); // close write pipe

  // open read file descriptor
  FILE *input = fdopen(fd[0], "r");
  if (input) {
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), input)) {
      std::cout << buffer;
    }
  } else {
    // opening file descriptor failed
    std::cerr << "Failed to open pipe input" << std::endl;
  }
  close(fd[0]); // close read pipe when finished

  // clean up child processes
  for (pid_t pid : children) {
    int status;
    waitpid(pid, &status, 0); // parent waits for each child to finish
  }
  return 0;
}
