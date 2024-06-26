#include "hrun.h"

int main(int argc, char* argv[], char* envp[]){

    if(argc<0){return -1;}

    int logging = envp[0][8] - '0';

    if(argv[1][0]=='i' && argv[1][1] == 'n'){
        if(task_in(argv[1])){
            exit(-1);
        }
        else{
           exit(1);
        }
    }

    char* task = NULL;
    char* current_time = NULL;
    char  plan_time[TIME_LENGTH];
    char  type[3];
    type[0] = argv[1][0];
    type[1] = argv[1][1];
    type[2] = '\0';

    time_t rawtime;
    struct tm * timeinfo;
    
    FILE* logger = NULL;

    struct timespec nan  = {1,0};
    struct timespec nan2 = {0,5000000};

    sem_t* log_mutex = sem_open(LOG_MUTEX_NAME , 0);
    
    if(argv[1]==NULL){
        perror("no argument passed\n");
        exit(-1);
    }
   
    for(int j = 0; j < TIME_LENGTH; j++){
        plan_time[j] = argv[1][j + TYPE_LENGTH + 1];     //get plan time from data
    }
    
    task = get_task_from_string(argv[1]);

    if(logging){
    sem_wait(log_mutex);
    {
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        current_time = asctime(timeinfo);
       
        logger = fopen(LOG_PATH, "a");
        if(logger==NULL){
            strerror(errno);
            exit(-1);
        }

        fprintf(logger, "%s\t|\t", LOG_ASGN);
        for(int i = 0; i < TIME_FULL_LENGTH; i++){
            fprintf(logger, "%c", current_time[i]);
        } 
        fprintf(logger, "\t|\t%d\t|\t%s", getpid(), task);

        fclose(logger);
    }
    sem_post(log_mutex);
    }
    while(1){
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        current_time = asctime(timeinfo);
       
        if(time_to_proc(plan_time, time_format(current_time)) == 1){  //check if time to do stuff
            system(task);

            if(logging){
                sem_wait(log_mutex);
                {
                    logger = fopen(LOG_PATH, "a");
                    if(logger==NULL){
                        strerror(errno);
                        exit(-1);
                    }

                    current_time = asctime(timeinfo);

                    fprintf(logger, "%s\t|\t", LOG_EXEC);
                    for(int i = 0; i < TIME_FULL_LENGTH; i++){ fprintf(logger, "%c", current_time[i]); } 
                    fprintf(logger, "\t|\t%d\t|\t%s", getpid(), task);

                    if(type[0] == 'a' && type[1] == 't'){
                        fprintf(logger, "%s\t|\t", LOG_KILL);
                        for(int i = 0; i < TIME_FULL_LENGTH; i++){ fprintf(logger, "%c", current_time[i]); } 
                        fprintf(logger, "\t|\t%d\t|\t%s\n", getpid(), DEATH_CAUSE_ONCE);
                    }
                    fclose(logger);
                }
                sem_post(log_mutex);
            }

            if(type[0] == 'a' && type[1] == 't'){
                exit(1);
            }
        }
        nanosleep(&nan,&nan2);
    }
    exit(1);
    return 0;
}
