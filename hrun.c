#include "hrun.h"
int main(int argc, char* argv[], char* envp[]){
    //(void)printf("hrun is running\n");

    char opt = getopt(argc,argv, OPT_MASK);
	if(opt != -1){
        argv[0][0] = opt;
        (void)execve(CONTROLLER_PATH, argv, envp);
        (void)printf("execve failed!\n");
        exit(-1);
    }
	
    struct stat entstat;	
    int flag;    
	flag=lstat(CONFIG_PATH,&entstat);			//get entry type
    if(flag==-1){
        (void)perror("lstat failed\n");
        exit(-1);
    }

    printf("\tseconds since epoch began %jd\n",(intmax_t)(entstat.st_mtime));
    printf("\t.config last access: %s\n",asctime(gmtime(&entstat.st_mtime)));
    //printf("hrun exit\n");
    exit(1);
    return 0;
}
