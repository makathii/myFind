#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>

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

    int filecount = argc - optind - 1;

    std::cout<<"Search Path: "<<searchpath<<"\n";
    std::cout<<"Filenames:\n";
    for (const auto &f : filenames){
        std::cout<<"  "<<f<<"\n";
    }
    std::cout<<"Options: recursive="<<rec<<", insensitive="<<cis<<"\n";

    return 0;
}