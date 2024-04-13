#include "hrun.h"
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]){
    if(argc < 1){
        perror("argc too little\n");
        exit(-1);
    }
    int flag = 0;
    FILE* fptr2 = fopen(".output","a");
    if(fptr2==NULL){
        perror("file open error\n");
        exit(-1);
    }

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
                             free(str);
                             exit(0);
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
                     (void)execve(getenv("EDITOR"),argv,envp);
                     (void)printf("execve failed!\n");
                     exit(-1);
                     break;
                 }
        case 'k':{
                     pid_t root = 0;
                     FILE* fpids = fopen(".pids","r");
                     if(fpids==NULL){
                         perror("file open error\n");
                         exit(-1);
                     }
                     char* line = (char*)calloc(10,1);
                     line = fgets(line, 9, fpids);
                     root = atoi(line);
                     signal(SIGUSR1,sig1_handler);
                     kill(root,SIGUSR1);
                     break;
                 }
        case 'w':{
                     time_t rawtime;
                     struct tm * timeinfo;
                     time ( &rawtime );
                     timeinfo = localtime ( &rawtime );
                     fprintf (stdout, "Current local time and date: %s", asctime (timeinfo));
                     fprintf (fptr2, "Current local time and date: %s", asctime (timeinfo));
                     break;
                 }
        default:{
                    printf("idk\n");
                    break;
                }
    }
    fclose(fptr2);
    exit(1);
    return 0;
}
