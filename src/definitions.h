#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>

#define SYSCONF_AMT 5
#define TYPE_LENGTH 2
#define TIME_LENGTH 15
#define TIME_FULL_LENGTH 24
#define TASK_PREFIX (TYPE_LENGTH + 1 + TIME_LENGTH + 1)

#define LOG_ASGN         "[ASGN]:"
#define LOG_EXEC         "[EXEC]:"
#define LOG_KILL         "[KILL]:"
#define LOG_INFO         "[INFO]:"

#define HRUN_CHILD       "./bin/hrunik"
#define PARENT_PATH      "./bin/hrun_parent"
#define CONTROLLER_PATH  "./bin/hrun_controller"

#define LOG_PATH         "./.log"
#define PIDS_PATH        "./sys/.pids"
#define CONFIG_PATH      "./.config"
#define SYSCONF_PATH     "./sys/.sysconfig"

#define LOG_MUTEX_NAME   "HRUN_LOG_MUTEX"
#define PIDS_MUTEX_NAME  "HRUN_PIDS_MUTEX"

#define OPT_MASK         "lewkas"
#define ENV_VAR          "HRUN"
#define EXAMPLE_TASK     "at Sat dd hh mm ss ./hrun -l"

#define DEATH_CAUSE_ONCE       "CAUSE: ONE-TIME TASK"
#define DEATH_CAUSE_PARENT     "CAUSE: .CONFIG CHANGE"
#define DEATH_CAUSE_SYSCONF    "CAUSE: .SYSCONFIG CHANGE"
#define DEATH_CAUSE_CONTROLLER "CAUSE: -k"
#define LOG_CONTROLLER_INVOKED "./hrun -"

#define MAX_LINE_LENGTH 200
#define LINE_SEPARATOR  "\n==========================================================================\n"
#define LOG_SEPARATOR   "\n--------------------------------------------------------------------------\n"
#define HEADER          "[TYPE]:	|	         [TIME]         	|	[PID]   |    [MESSAGE]"

int lstat(const char * file, struct stat * buf);
int nanosleep(const struct timespec *requested_time, struct timespec *remaining);
int kill(__pid_t pid, int sig);
int getopt(int argc, char *const *argv, const char *shortopts);
