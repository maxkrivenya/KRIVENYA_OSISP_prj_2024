#include "hrun.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[], char* envp[]){
    time_t last_change_date;
    struct stat entstat;	
    int flag;    
    pid_t pid       = 0;

    char* str = (char*)calloc(256, sizeof(char));
    if(str==NULL){
        perror("calloc\n");
        exit(-1);
    }

    flag=lstat(CONFIG_PATH,&entstat);			//get entry type
    if(flag==-1){
        perror("lstat failed\n");
        exit(-1);
    }

    last_change_date = entstat.st_mtime;
    
    while(1){
       FILE* fptr = fopen(".config","r");
       if(fptr==NULL){
           perror("file open error\n");
           exit(-1);
       }

       while(!feof(fptr)){

           (void)fgets(str,MAX_LINE_LENGTH , fptr);
           if(str==NULL){
               (void)printf("fgets error. exiting\n");
               free(str);
               exit(0);
           }

           if(!feof(fptr)){

               pid = fork();

               switch (pid) {
                   case -1:{                           //fork failed
                               printf("%s\n",strerror(errno));
                               free(str);
                               exit(EXIT_FAILURE);
                           }

                   case 0:{                            //for forked
                                                       //name[CHILD_NAME_SIZE-2] = '0' + counter;
                                                       //argv[0] = name;
                              argv[1] = str;
                              flag = execve(HRUN_CHILD, argv, envp);
                              if(flag == -1){
                                  (void)printf("execve error:%s\n", strerror(errno));
                                  (void)exit(EXIT_FAILURE);
                              }
                              exit(1);
                              break;
                          }
                   default:{   /*
                               //for parent
                               printf("created C%d-%d\n",counter,pid);
                               child[counter] = pid;
                               break;
                               */
                               break;
                           }    

               } //switch

           } //feof fptr
       } //feof fptr #2

       fclose(fptr);

       while(1){
           flag=lstat(CONFIG_PATH,&entstat);			//get entry type
           if(flag==-1){
               (void)perror("lstat failed\n");
               exit(-1);
           }
           if(last_change_date != entstat.st_mtime){
               //kill();
               last_change_date = entstat.st_mtime;
               break;
           }
           struct timespec nan1 = {1, 0};
           struct timespec nan2 = {1, 0};
           nanosleep(&nan1,&nan2);

       } //while 1

    } // while 1


    free(str);
    exit(1);
    return 0;
}
