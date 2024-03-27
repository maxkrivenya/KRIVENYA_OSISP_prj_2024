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
        char* mf = asctime(gmtime(&entstat.st_mtime));



    }

    return 0;
}
