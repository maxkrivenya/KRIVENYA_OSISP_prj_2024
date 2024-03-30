#include "hrun.h"
int lstat(const char *restrict file, struct stat *restrict buf);

int main(int argc, char* argv[], char* envp[]){

    if(argc < 1){return -1;}
    
    //.config change check
    time_t last_change_date;
    struct stat entstat;	
    
    //nanosleep
    struct timespec nan1 = {1, 0};
    struct timespec nan2 = {1, 0};
    
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

       FILE* fpids = fopen(".pids","w");
       if(fpids==NULL){
           perror("file open error\n");
           exit(-1);
       }
       
       FILE* fstat = fopen(".stat","w");
       if(fstat==NULL){
           perror("file open error\n");
           exit(-1);
       }

       (void)fputs(LINE_SEPARATOR, fstat); 
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
                   default:{   
                               //for parent
                               fprintf(fstat,"created hrunik | %d\n",pid);
                               fprintf(fpids,"%d\n",pid);
                               break;
                           }    

               } //switch

           } //feof fptr
       } //feof fptr #2

       flag = fclose(fstat);
       if(flag==EOF){
           (void)printf("file close error.\n");
           exit(-1);
       }

       flag = fclose(fpids);
       if(flag==EOF){
           (void)printf("file close error.\n");
           exit(-1);
       }

       flag = fclose(fptr);
       if(flag==EOF){
           (void)printf("file close error.\n");
           exit(-1);
       }

       while(1){
           flag=lstat(CONFIG_PATH,&entstat);			//get entry type
           if(flag==-1){
               (void)perror("lstat failed\n");
               exit(-1);
           }
           if(last_change_date != entstat.st_mtime){
               break;
           }
           (void)nanosleep(&nan1,&nan2);

       } //while 1

       last_change_date = entstat.st_mtime;

       fpids = fopen(".pids","r");      //kill children here 
       if(fpids==NULL){
           perror("file open error\n");
           exit(-1);
       }
       while(!feof(fpids)){
           (void)fgets(str,MAX_LINE_LENGTH , fpids);
           if(!feof(fpids)){
               printf("killing %s\n",str);
               kill(atoi(str),SIGINT);
           }
       }
       flag = fclose(fpids);
       if(flag==EOF){
           (void)printf("file close error.\n");
           exit(-1);
       }

    } // while 1

    free(str);
    exit(1);
    return 0;
}
