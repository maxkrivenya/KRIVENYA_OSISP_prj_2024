#include "hrun.h"
#include <unistd.h>

int main(int argc, char* argv[], char* envp[]){

    char opt = getopt(argc,argv, OPT_MASK);

    int i = 0;
    int j = 0;

    //reading sysconf from file
    FILE* fsys = fopen(SYSCONF_PATH, "r");
    if(fsys == NULL){
        printf("%s\n", strerror(errno));
        exit(-1);
    }

    char** sysconf = (char**)calloc(SYSCONF_AMT + 1, sizeof(char*));
    for(int i = 0; i < SYSCONF_AMT; i++){
        sysconf[i] = (char*)calloc(MAX_LINE_LENGTH + 1, sizeof(char));
        fgets(sysconf[i], MAX_LINE_LENGTH, fsys);
    }

    fclose(fsys);

    //removing var names from variables
    for(j = 0; sysconf[1][j] != '='; j++);
    j++;
    for(i = 0; sysconf[1][j] != '\0';i++, j++){
        sysconf[1][i] = sysconf[1][j];
    }
    sysconf[1][i] = '\0';

    for(j = 0; sysconf[2][j] != '='; j++);
    j++;
    for(i = 0; sysconf[2][j] != '\n';i++, j++){
        sysconf[2][i] = sysconf[2][j];
    }
    sysconf[2][i] = '\0';


	if(opt != -1){ //call controller 
        argv[0][0] = opt;
        (void)execve(CONTROLLER_PATH, argv, sysconf);
        (void)strerror(errno);
        exit(-1);
    }else{          //call planner
        argv[0] = PARENT_PATH;
        execve(PARENT_PATH, argv, sysconf);
        (void)strerror(errno);
    }

    printf("%s\n", strerror(errno));
    exit(-1);
}
