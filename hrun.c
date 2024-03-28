#include "hrun.h"
#define PARENT_PATH "./hrun_parent"
int main(int argc, char* argv[], char* envp[]){
    //(void)printf("hrun is running\n");

    char opt = getopt(argc,argv, OPT_MASK);
	if(opt != -1){
        argv[0][0] = opt;
        (void)execve(CONTROLLER_PATH, argv, envp);
        (void)printf("execve failed!\n");
        exit(-1);
    }

    execve(PARENT_PATH, argv, envp);
    perror("execve error\n");
    exit(-1);
}
