#include "hrun.h"

int main(int argc, char* argv[], char* envp[]){

    char opt = getopt(argc,argv, OPT_MASK);
	if(opt != -1){
        argv[0][0] = opt;
        (void)execve(CONTROLLER_PATH, argv, envp);
        (void)printf("execve failed!\n");
        exit(-1);
    }
    argv[0] = PARENT_PATH;
    execve(PARENT_PATH, argv, envp);
    (void)printf("execve error:%s\n", strerror(errno));
    exit(-1);
}
