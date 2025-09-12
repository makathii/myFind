/*  first, getopt parses through the arguments given & assigns some variables to check for
    then, we fork a child process for each filename we got. Each child process runs
    the search_f() function which checks if recursive or not & then searches the path
    accordingly. The parent process waits for all children to finish before exiting. 
*/

//what still needs to be done:
//-error handling for invalid search path
//-make sure the output is always correct (I only did a little testing)
//-optimize the code (maybe make search_f() less redundant?)

//on that note: what is the correct way to sort includes? I just put them in alphabetic order
#include <filesystem>
#include <getopt.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h> //for fork()
#include <vector>

void search_f(std::string path, std::string filename, bool rec, bool cis);

int main(int argc, char *argv[]) {
    int c;
    bool rec = false;   //recursive
    bool cis = false;    //case INsensitive

    while((c=getopt(argc,argv, "Ri")) != -1) {
        switch(c) {
            case 'R':
                rec = true;
                break;
            case 'i':
                cis = true;
                break;
            case '?':
                std::cerr<<"Usage: "<<argv[0]<< " [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n";
                return 1;
            default:
                abort();
        }
    }

    if(argc-optind < 2) {   //because we need at least searchpath and one filename
        std::cerr<<"Usage: "<<argv[0]<< " [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n";
        return 1;
    }

    std::string searchpath = argv[optind];
    std::vector<std::string> filenames;
    for (int i = optind + 1; i < argc; i++){
        filenames.push_back(argv[i]);
    }

    std::vector<pid_t> children;
    int status;
    for(auto &f : filenames){
        pid_t pid = fork();
        if(pid==0){ //child process
            search_f(searchpath, f, rec, cis);
            exit(0);    //child process exits here
        }else if(pid<0){
            std::cerr<<"Fork failed!\n";
            return 1;
        }else if(pid>0){ //parent process
            children.push_back(pid);
        }
    }
    for (pid_t pid : children) {
        waitpid(pid, &status, 0);   //parent waits for each child to finish
    }
    return 0;
}

void search_f(std::string path, std::string filename, bool rec, bool cis){
    std::string filename_lower = filename;
    if(cis){
        for(char &ch : filename_lower) ch = tolower(ch);
    }
    if(rec){
        for(const auto & entry : std::filesystem::recursive_directory_iterator(path)){  //recursive directory iterator
            std::string currentFile = entry.path().filename().string();
            if(cis){
                for(char &ch : currentFile) ch = tolower(ch);
            }
            if(currentFile == filename_lower){
                std::cout<<getpid()<<": "<<filename<<": "<<entry.path().string()<<"\n";
            }
        }
    }else if(!rec){
        for(const auto & entry : std::filesystem::directory_iterator(path)){    
            std::string currentFile = entry.path().filename().string();     //I know we basically have the same code twice,
            if(cis){                                                        //but I struggled to make it work with a seperate function
                for(char &ch : currentFile) ch = tolower(ch);
            }
            if(currentFile == filename_lower){
                std::cout<<getpid()<<": "<<filename<<": "<<entry.path().string()<<"\n";
            }
        }
    }
}
