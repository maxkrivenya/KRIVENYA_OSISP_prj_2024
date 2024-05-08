#include "definitions.h"

void sig1_handler(int sig){
    signal(SIGUSR1, sig1_handler);
    kill(0,SIGINT);
}


//dd hh mm ss task 
//01 34 67 9  12->
int time_to_proc(char* plan, char* now){
    if(plan==NULL){return -1;}
    if(now==NULL){return -1;}
    for(int i = 0; i < TIME_LENGTH; i++){
        if(isdigit(plan[i])){
            if(plan[i]!=now[i]){
                return 0;
            }        
        }
    } 
    return 1;
}

char* get_task_from_string(char* string){
    if(string==NULL){return NULL;}
    int length = strlen(string);
    int shift = TASK_PREFIX;
    int i = 0;
    if(length < shift){return NULL;}

    for(i = 0; i < length - shift; i++){
        string[i] = string[i+shift];
    }
    for(; i < length; ++i){
        string[i] = '\0';
    }
    string[i] = '\0';
    return string;
}

int task_in(char* whole_ass_string){

    if((
            isdigit(whole_ass_string[7]) ||
            isdigit(whole_ass_string[8]) ||
            isdigit(whole_ass_string[10]) ||
            isdigit(whole_ass_string[11]) ||
            isdigit(whole_ass_string[13]) ||
            isdigit(whole_ass_string[14]) ||
            isdigit(whole_ass_string[16]) ||
            isdigit(whole_ass_string[17])
      ) == 0){
            return -1;
    }

    long timer = (
        (whole_ass_string[17] - '0') +
        (whole_ass_string[16] - '0') +
        (60*(whole_ass_string[14] - '0')) +
        (60*(whole_ass_string[13] - '0')) +
        (3600*(whole_ass_string[11] - '0')) +
        (3600*(whole_ass_string[10] - '0')) +
        (86400*(whole_ass_string[8] - '0')) +
        (86400*(whole_ass_string[7] - '0')) 
    );

    int len = strlen(whole_ass_string);

    whole_ass_string = get_task_from_string(whole_ass_string);

    sleep(timer); 

    system(whole_ass_string);

    return 0;
}
