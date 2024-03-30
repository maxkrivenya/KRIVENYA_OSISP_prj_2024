#include "hrun.h"

int main(int argc, char* argv[], char* envp[]){
    if(argc<0 || envp[0]==NULL){return -1;}
    //char* data = (char*)calloc(256,sizeof(char));
    char* task = NULL;
    char* current_time = NULL;
    char plan_time[14];
    
    int i = 0;
    time_t rawtime;
    struct tm * timeinfo;

    struct timespec nan = {1,0};
    struct timespec nan2 = {0,5000000};
    
    if(argv[1]==NULL){
        perror("no argument passed\n");
        exit(-1);
    }
   
    for(int j = 0; j < 14; j++){
        plan_time[j] = argv[1][j];     //get plan time from data
    }
    
    task = get_task_from_string(argv[1]);
    
    while(1){
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        current_time = asctime(timeinfo);
       
        for(i = 0; i < 12; i++){
            current_time[i] = current_time[i + 8];  //remove weekday, month, year
        }
        current_time[i]='\0';
        
        if(current_time[7]=='5'){                   //REMOVE BEFORE SHIPPING
            exit(1);
        }

        if(time_to_proc(plan_time, current_time)){  //check if time to do stuff
            system(task);
        }
        nanosleep(&nan,&nan2);
    }
    exit(1);
    return 0;
}
