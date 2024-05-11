#include "hrun.h"

int main(int argc, char* argv[], char* envp[]){

    if(argc < 1){
        perror("argc too little\n");
        exit(-1);
    }

    int logging = envp[0][8] - '0';
    int flag = 0;
    char* timer = NULL;
    FILE* logger = NULL;

    time_t rawtime;
    time ( &rawtime );

    sem_t* log_mutex = sem_open(LOG_MUTEX_NAME , 0);
       
    if(logging){
        sem_wait(log_mutex);
        {
            logger = fopen(LOG_PATH, "a");
            if(logger==NULL){
                strerror(errno);
                exit(-1);
            }
            timer = asctime(localtime(&rawtime));
            fprintf(logger,"%s\t|\t", LOG_INFO);
            for(int i = 0; i < TIME_FULL_LENGTH; i++){
                fprintf(logger, "%c", timer[i]); 
            }
            fprintf(logger,"\t|\t%d\t|\t%s%c\n", getpid(), LOG_CONTROLLER_INVOKED, argv[0][0]);

            fclose(logger);
        }
        sem_post(log_mutex);
    }

    switch(argv[0][0]){
        case 'l':{
                     int   i   = 0;
                     char* str = (char*)calloc(MAX_LINE_LENGTH+1,sizeof(char));

                     if(str==NULL){
                         perror("calloc fail\n");
                         exit(-1);
                     }

                     FILE* fptr = fopen(CONFIG_PATH,"r");
                     {
                         while(!feof(fptr)){
                             i++;

                             (void)fgets(str,MAX_LINE_LENGTH , fptr);
                             if(str==NULL){
                                 (void)printf("fgets error. exiting\n");
                                 free(str);
                                 exit(0);
                             }

                             if(!feof(fptr)){
                                 (void)printf("%d| %s", i, str);
                             }
                         }
                     }
                     flag = fclose(fptr);

                     if(flag==EOF){
                         (void)printf("file close error.\n");
                         exit(0);
                     }
                     free(str);
                     break;
                 }
        case 'e':{
                     argv[1] = CONFIG_PATH;
                     argv[2] = NULL;
                     (void)execve(getenv("EDITOR"),argv,envp);
                     (void)strerror(errno);
                     exit(-1);
                     break;
                 }
        case 'k':{
                     pid_t root  = 0;
                     char* line  = (char*)calloc(10,1);

                     sem_t* pids_mutex = sem_open(PIDS_MUTEX_NAME , 0);
                     sem_t* log_mutex  = sem_open(LOG_MUTEX_NAME , 0);

                     sem_wait(pids_mutex);
                     {
                         FILE* fpids = fopen(".pids","r");
                         {
                             if(fpids==NULL){
                                 perror("file open error\n");
                                 exit(-1);
                             }
                             line = fgets(line, 9, fpids);
                             root = atoi(line);
                         }

                         if(logging){
                             sem_wait(log_mutex);
                             {
                                 FILE* flog = fopen(LOG_PATH, "a");
                                 if(flog == NULL){
                                     printf("%s\n", strerror(errno));
                                     exit(-1);
                                 }

                                 while(!feof(fpids)){
                                     (void)fgets(line, MAX_LINE_LENGTH, fpids);
                                     if(!feof(fpids)){
                                         fprintf(flog, "%s\t|\t", LOG_KILL);
                                         timer = asctime(localtime(&rawtime));
                                         for(int i = 0; i < TIME_FULL_LENGTH; i++){
                                             fprintf(flog, "%c", timer[i]); 
                                         }
                                         fprintf(flog, "\t|\t%d\t|\t%s\n", atoi(line), DEATH_CAUSE_CONTROLLER);
                                     }
                                 }
                                 fclose(flog);
                         }
                         sem_post(log_mutex);
                        }
                         fclose(fpids);
                     }
                     sem_post(pids_mutex);

                     signal(SIGUSR1,sig1_handler);
                     kill(root,SIGUSR1);
                     break;
                 }
        case 'w':{
                     time_t rawtime;
                     struct tm * timeinfo;
                     time ( &rawtime );
                     timeinfo = localtime ( &rawtime );
                     fprintf (stdout, "Current local time and date: %s", asctime (timeinfo));
                     break;
                 }
        case 'a':{
                     char* task = (char*)calloc(MAX_LINE_LENGTH + 1, sizeof(char));
                     printf("Example:\n %s\n", EXAMPLE_TASK);
                     printf("New task:\n ");
                     fgets(task, MAX_LINE_LENGTH, stdin);
                     logger = fopen(CONFIG_PATH, "a");
                     fputs(task, logger); 
                     fclose(logger);
                     free(task);
                     break;
                 }
        case 's':{
                     argv[1] = SYSCONF_PATH;
                     argv[2] = NULL;
                     (void)execve(getenv("EDITOR"),argv,envp);
                     (void)strerror(errno);
                     exit(-1);
                     break;
                    
                 }
        default:{
                    printf("idk\n");
                    break;
                }
    }
    exit(1);
    return 0;
}
