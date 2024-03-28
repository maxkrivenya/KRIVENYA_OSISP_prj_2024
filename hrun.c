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

    char* data = (char*)calloc(256,sizeof(char));
    char* task = NULL;
    char* current_time = NULL;
    char plan_time[14];
    
    int i = 0;
    time_t rawtime;
    struct tm * timeinfo;

    struct timespec nan = {1,0};
    struct timespec nan2 = {0,5000000};
    
    if(data==NULL){
        perror("Calloc\n");
        exit(-1);
    }
   
    FILE* fptr = fopen(CONFIG_PATH,"r");
    if(fptr==NULL){
        perror("file open error\n");
        exit(-1);
    }
    
    data = fgets(data, 255, fptr);
    fclose(fptr);
    
    if(fptr==NULL){
        perror("fclose\n");
        exit(-1);
    }
    if(data==NULL){
        perror("calloc\n");
        exit(-1);
    }
    
    for(int j = 0; j < 14; j++){
        plan_time[j] = data[j];     //get plan time from data
    }
    
    task = get_task_from_string(data);
    
    while(1){
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        current_time = asctime(timeinfo);
       
        for(i = 0; i < 12; i++){
            current_time[i] = current_time[i + 8];  //remove weekday, month, year
        }
        current_time[i]='\0';
        
        if(current_time[7]=='7'){                   //REMOVE BEFORE SHIPPING
            free(data);
            exit(1);
        }

        if(time_to_proc(plan_time, current_time)){  //check if time to do stuff
            system(task);
        }
        nanosleep(&nan,&nan2);
    }
    //printf("hrun exit\n");
    exit(1);
    return 0;
}
