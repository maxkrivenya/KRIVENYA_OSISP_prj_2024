#include "hrun.h"
#include <string.h>

int main(int argc, char* argv[], char* envp[]){


    if(argc < 1){return -1;}

    //.config change check
    time_t last_change_date;
    struct stat entstat;	

    //nanosleep
    struct timespec nan1 = {1, 0};
    struct timespec nan2 = {1, 0};

    time_t      rawtime;
    struct tm * timeinfo;
    char*       current_time;

    sem_unlink(LOG_MUTEX_NAME);
    sem_unlink(PIDS_MUTEX_NAME);
    sem_t* log_mutex    = sem_open(LOG_MUTEX_NAME , O_CREAT, 0666, 1);
    sem_t* pids_mutex   = sem_open(PIDS_MUTEX_NAME , O_CREAT, 0666, 1);

    int   flag  = 0;    
    pid_t pid   = 0;

    FILE* fpids = NULL;
    FILE* flog  = NULL;

    signal(SIGUSR1, sig1_handler);

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
            printf("fcuk you%s\n",strerror(errno));
            perror("file open error\n");
            exit(-1);
        }

        sem_wait(log_mutex);
        {
            flog = fopen(".log","a");
            if(flog==NULL){
                printf("%s\n",strerror(errno));
                exit(-1);
            }

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            current_time = asctime(timeinfo);

            fprintf(flog, "\n%s%s%s%s%s", LINE_SEPARATOR, current_time, LOG_SEPARATOR, HEADER, LOG_SEPARATOR);
            flag = fclose(flog);
            if(flag==EOF){
                printf("%s\n",strerror(errno));
                exit(-1);
            }
        }
        sem_post(log_mutex);

        sem_wait(pids_mutex);
        {
            fpids = fopen(".pids","w");
            if(fpids==NULL){
                perror("file open error\n");
                exit(-1);
            }
            (void)fprintf(fpids, "%d\n" ,getpid());


            while(!feof(fptr)){

                (void)fgets(str, MAX_LINE_LENGTH, fptr);
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
                                   argv[0] = "hrunik";
                                   argv[1] = str;
                                   flag = execve(HRUN_CHILD, argv, NULL);
                                   if(flag == -1){
                                       (void)printf("execve error:%s\n", strerror(errno));
                                       (void)exit(EXIT_FAILURE);
                                   }
                                   exit(1);
                                   break;
                               }
                        default:{   
                                    //for parent
                                    fprintf(fpids,"%d\n",pid);
                                    break;
                                }    

                    } //switch

                } //if !feof fptr
            } //while !feof fptr 

            flag = fclose(fpids);
            if(flag==EOF){
                (void)printf("file close error.\n");
                exit(-1);
            }
        }
        sem_post(pids_mutex);
        fclose(fptr);

        while(1){

            /*-------------CONFIG STAT------------*/
            flag=lstat(CONFIG_PATH,&entstat);			//get entry type
            if(flag==-1){
                (void)perror("lstat failed\n");
                exit(-1);
            }
            if(last_change_date != entstat.st_mtime){
                break;
            }

            /*----------------LOGGING-------------*/

            time ( &rawtime );
            timeinfo = localtime ( &rawtime );
            current_time = asctime(timeinfo);
            if(
                    (
                     //current_time[11]==
                     //current_time[12]==
                     //current_time[14]==
                     //current_time[15]==
                     current_time[17]==
                     current_time[18]
                    )
                    &&
                    current_time[18]=='0'
              ){

                sem_wait(log_mutex);

                system("sh ./.sysconfig");
                flog = fopen(LOG_PATH, "w");
                if(flog==NULL){
                    printf("%s\n",strerror(errno));
                    exit(-1);
                }

                time ( &rawtime );
                timeinfo = localtime ( &rawtime );
                current_time = asctime(timeinfo);

                fprintf(flog, "\n%s%s%s%s%s", LINE_SEPARATOR, current_time, LOG_SEPARATOR, HEADER, LOG_SEPARATOR);
                flag = fclose(flog);
                if(flag==EOF){
                    printf("%s\n",strerror(errno));
                    exit(-1);
                }

                sem_post(log_mutex);
            }

            (void)nanosleep(&nan1,&nan2);

        } //while 1

        last_change_date = entstat.st_mtime;

        sem_wait(pids_mutex);
        {
            fpids = fopen(".pids","r");      //kill children here 
            if(fpids==NULL){
                printf("%s\n",strerror(errno));
                exit(-1);
            }

            sem_wait(log_mutex);
            {
                flog = fopen(LOG_PATH, "a");
                if(flog == NULL){
                    printf("couldnt open log:%s\n", strerror(errno));
                    exit(-1);
                }

                (void)fgets(str, MAX_LINE_LENGTH, fpids);

                while(!feof(fpids)){
                    (void)fgets(str, MAX_LINE_LENGTH, fpids);
                    if(!feof(fpids)){
                        fprintf(flog, "%s\t|\t", LOG_KILL);
                        time ( &rawtime );
                        current_time = asctime(localtime(&rawtime));
                        for(int i = 0; i < TIME_FULL_LENGTH; i++){
                            fprintf(flog, "%c", current_time[i]); 
                        }
                        fprintf(flog, "\t|\t%d\t|\t%s\n", atoi(str), DEATH_CAUSE_PARENT);
                    }
                }

                flag = fclose(flog);
                if(flag==EOF){
                    (void)printf("file close error.\n");
                    exit(-1);
                }
            }
            sem_post(log_mutex);

            flag = fclose(fpids);
            if(flag==EOF){
                (void)printf("file close error.\n");
                exit(-1);
            }
        }
        sem_post(pids_mutex);

    } // while 1

    free(str);
    exit(1);
    return 0;
}
