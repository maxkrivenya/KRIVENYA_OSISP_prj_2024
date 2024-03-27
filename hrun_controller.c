#include "hrun.h"

int main(int argc, char* argv[], char* envp[]){
    if(argc < 1){
        perror("argc too little\n");
        exit(-1);
    }
    int flag = 0;
    //printf("hrun_controller is running\n");
    switch(argv[0][0]){
        case 'l':{
                     char* str = (char*)calloc(MAX_LINE_LENGTH+1,sizeof(char));
                     if(str==NULL){
                         perror("calloc fail\n");
                         exit(-1);
                     }

                     FILE* fptr = fopen(CONFIG_PATH,"r");
                     while(!feof(fptr)){
                         (void)fgets(str,MAX_LINE_LENGTH , fptr);
                         if(str==NULL){
                             (void)printf("fgets error. exiting\n");
                             (void)exit(0);
                         }
                         if(!feof(fptr)){
                             (void)printf("^| %s", str);
                         }
                     }
                     flag = fclose(fptr);
                     if(flag==EOF){
                         (void)printf("file close error.\n");
                         exit(0);
                     }
                     free(str);
                     break;
                 }
        case 'e':{
                     argv[1] = CONFIG_PATH;
                     argv[2] = NULL;
                     (void)execve("/usr/bin/vim",argv,envp);
                     (void)printf("execve failed!\n");
                     exit(-1);
                     break;
                 }
        default:{
                    printf("idk\n");
                    break;
                }
    }
    //printf("hrun_controller exit\n");
    exit(-1);
    return 0;
}
