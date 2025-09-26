
# *myfind* Protocol
*Authors:* Katharina Markus & Felix Dilly

## Building and Running the Program
The project is built using the provided `Makefile`.
To compile the program, run: make all

## Parallelization Concept
Parallelization is achieved using **process-based concurrency**:
- The parent process parses arguments and then uses `fork()` to create one child process per filename.
- Each child searches for its assigned filename in the given directory (recursively if `-R` is enabled).
- The parent collects results from all children until they finish, preventing zombie processes via `waitpid()`.
This design ensures that multiple file searches can run simultaneously.

## Synchronization of Output
A key challenge was avoiding interleaved output from concurrent processes. To handle this, a **message queue** was used:
- Child processes send messages with their results (`pid`, filename, absolute path).
- The parent process reads from the queue and prints each message line by line.
- Since only the parent writes to `stdout`, the output remains clean and synchronized.

## Recursive Search
Recursive search is implemented using `std::filesystem::recursive_directory_iterator`.
If the `-R` option is disabled, only the top-level directory is scanned using `directory_iterator`. 

## Case-Insensitive Search
When `-i` is active, both the target filename and candidate filenames are converted to lowercase before comparison. This is handled via a helper function, ensuring portability.

## Signal Handling
The program installs custom signal handlers for `SIGINT` and `SIGTERM` in the parent process. If the program is terminated early (e.g., by `Ctrl+C`), the handler ensures that the System V message queue is properly removed with `msgctl(..., IPC_RMID, ...)`. This prevents orphaned message queues from remaining in the system after abnormal termination.
In child processes the signal handlers are reset to the default to avoid recursive cleanup attempts. This ensures that only the parent is responsible for resource management.

## Error Handling
The program validates arguments with `getopt()`. Invalid or missing search paths produce error messages. File system errors (e.g., permission denied) are caught and reported, but the program continues searching in other directories.
