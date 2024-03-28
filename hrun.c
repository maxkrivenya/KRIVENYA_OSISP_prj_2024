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
    char* task = (char*)calloc(256,sizeof(char));
    char* current_time = (char*)calloc(80,sizeof(char));
    char* plan_time = (char*)calloc(15,sizeof(char));
    if(data==NULL || task==NULL || current_time == NULL){
        perror("Calloc\n");
        exit(-1);
    }
    FILE* fptr = fopen(CONFIG_PATH,"r");
    if(fptr==NULL){
        perror("file open error\n");
        exit(-1);
    }
    FILE* fptr2 = fopen("time_save.txt","w");
    if(fptr2==NULL){
        perror("file open error\n");
        exit(-1);
    }
    data = fgets(data, 255, fptr);
    if(data==NULL){
        perror("calloc\n");
        exit(-1);
    }
    for(int j = 0; j < 14; j++){
        plan_time[j] = data[j];
    }
    task = get_task_from_string(data);
    
    int i = 0;
    time_t rawtime;
    struct tm * timeinfo;
    while(1){
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        current_time = asctime(timeinfo);
        i = 0;
        for(i = 0; i < 12; i++){
            current_time[i] = current_time[i + 8];
        }
        current_time[i]='\0';
        if(current_time[4]=='3'){
            fclose(fptr);
            fclose(fptr2);
            free(data);
            free(task);
            free(current_time);
            free(plan_time);
            exit(1);
        }
        if(time_to_proc(plan_time, current_time)){
            system(task);
        }
        struct timespec nan = {1,0};
        struct timespec nan2 = {0,5000000};

        nanosleep(&nan,&nan2);
    }
    //printf("hrun exit\n");
    exit(1);
    return 0;
}
