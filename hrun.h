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
    for(int i = 0; i < 12; i++){
        if(isdigit(plan[i])){
            if(plan[i]!=now[i]){
                return 0;
            }        
        }
    } 
    return -1;
}

char* get_task_from_string(char* string){
    if(string==NULL){return NULL;}
    int length = strlen(string);
    int shift = 12;
    int i = 0;
    if(length < shift){return NULL;}

    for(i = 0; i < length - shift; i++){
        string[i] = string[i+shift];
    }
    string[i] = '\0';
    return string;
}

