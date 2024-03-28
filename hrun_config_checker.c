#include "hrun.h"
#include <time.h>

int main(){
    time_t last_change_date;
    struct stat entstat;	
    int flag;    

    flag=lstat(CONFIG_PATH,&entstat);			//get entry type
    if(flag==-1){
        (void)perror("lstat failed\n");
        exit(-1);
    }
    last_change_date = entstat.st_mtime;
    while(1){
        flag=lstat(CONFIG_PATH,&entstat);			//get entry type
        if(flag==-1){
            (void)perror("lstat failed\n");
            exit(-1);
        }
        if(last_change_date != entstat.st_mtime){
            //kill();
            last_change_date = entstat.st_mtime;
        }
        struct timespec nan1 = {1, 0};
        struct timespec nan2 = {1, 0};
        nanosleep(&nan1,&nan2);

    }

    return 0;
}
