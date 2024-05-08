#include "hrun.h"

int main(int argc, char* argv[], char* envp[]){

    if(argc<0){return -1;}

    if(argv[1][0]=='i' && argv[1][1] == 'n'){
        if(task_in(argv[1])){
            printf("task_in failed\n");
            exit(-1);
        }
        else{
           printf("task_in success\n"); 
           exit(1);
        }
    }

    char* task = NULL;
    char* current_time = NULL;
    char  plan_time[TIME_LENGTH];
    
    int i = 0;
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
    
    while(1){
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        current_time = asctime(timeinfo);
       
        current_time[4] = ' ';
        if(current_time[8] == ' '){
            current_time[8] = '0';
        }

        for(i = 4; i < TIME_LENGTH; i++){
            current_time[i + 1] = current_time[i + 5];  //remove weekday, month, year
        }
        current_time[i]='\0';


        if(current_time[11]=='0'){                   //REMOVE BEFORE SHIPPING
            exit(1);
        }

        if(time_to_proc(plan_time, current_time) == 1){  //check if time to do stuff
            system(task);

            sem_wait(log_mutex);
            {
                logger = fopen(LOG_PATH, "a");
                if(logger==NULL){
                    strerror(errno);
                    exit(-1);
                }

                current_time = asctime(timeinfo);

                fprintf(logger, "%s\t|\t", LOG_EXEC);
                for(int i = 0; i < TIME_FULL_LENGTH; i++){
                    fprintf(logger, "%c", current_time[i]);
                } 
                fprintf(logger, "\t|\t%d\t|\t%s", getpid(), task);

                fclose(logger);
            }
            sem_post(log_mutex);

        }
        nanosleep(&nan,&nan2);
    }
    exit(1);
    return 0;
}
