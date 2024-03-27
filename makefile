CC=gcc
CFLAGS= -W -Wall -Wextra -std=c11
.PHONY: clean

all: hrun hrun_controller
hrun: hrun.c makefile
		$(CC) $(CFLAGS) hrun.c -o hrun
hrun_controller: hrun_controller.c makefile
		$(CC) $(CFLAGS) hrun_controller.c -o hrun_controller
