#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>

#define LOG_ASGN         "[ASGN]:"
#define LOG_EXEC         "[EXEC]:"
#define LOG_KILL         "[KILL]:"
#define LOG_INFO         "[INFO]:"

#define TIME_FULL_LENGTH 24

#define LOG_CONTROLLER_INVOKED "INVOKED ./hrun -"
#define DEATH_CAUSE_CONTROLLER "CAUSE: -k"
#define DEATH_CAUSE_PARENT     "CAUSE: .CONFIG CHANGE"

#define HRUN_CHILD      "./hrunik"
#define PARENT_PATH     "./hrun_parent"
#define CONTROLLER_PATH "./hrun_controller"

#define CONFIG_PATH     "./.config"
#define PIDS_PATH       "./.pids"
#define LOG_PATH        "./.log"

#define OPT_MASK        "lewk"
#define ENV_VAR         "HRUN"

#define LOG_MUTEX_NAME   "HRUN_LOG_MUTEX"
#define PIDS_MUTEX_NAME  "HRUN_PIDS_MUTEX"

#define LINE_SEPARATOR "\n==========================================================================\n"
#define LOG_SEPARATOR "\n--------------------------------------------------------------------------\n"
#define HEADER            "[TYPE]:	|	         [TIME]         	|	[PID]   |    [MESSAGE]"
#define MAX_LINE_LENGTH 80

int lstat(const char * file, struct stat * buf);
int nanosleep(const struct timespec *requested_time, struct timespec *remaining);
int kill(__pid_t pid, int sig);
int getopt(int argc, char *const *argv, const char *shortopts);
