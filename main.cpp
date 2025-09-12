#include <iostream>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int c;
    bool rec = false;   //recursive
    bool cs = false;    //case sensitive

    while((c=getopt(argc,argv, "-R -i")) != -1) {
        switch(c) {
            case 'R':
                rec = true;
                break;
            case 'i':
                cs = true;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n", argv[0]);
                return 1;
            default:
                abort();
        }
    }

    if(argc-optind < 2) {   //because we need at least searchpath and one filename
        fprintf(stderr, "Usage: %s [-R] [-i] searchpath filename1 [filename2]...[filenameN]\n", argv[0]);
        return 1;
    }

    char *searchpath = argv[optind];
    char** filenames = &argv[optind+1];
    int filecount = argc - optind - 1;

    printf("Search Path: %s\n", searchpath);
    printf("Filenames:\n");
    for(int i = 0; i < filecount; i++) {
        printf("  %s\n", filenames[i]);
    }

    printf("Options: recursie=%d, case_sensitive=%d\n", rec, cs);

    return 0;
}