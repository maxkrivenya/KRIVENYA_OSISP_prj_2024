#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define CONFIG_PATH "./.config"
#define CONTROLLER_PATH "./hrun_controller"
#define OPT_MASK "le"
#define MAX_LINE_LENGTH 80
