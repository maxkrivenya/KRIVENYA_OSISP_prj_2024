CC=gcc
CFLAGS= -W -Wextra -std=c11
.PHONY: clean

all: hrun hrun_controller hrun_parent hrunik
hrun: hrun.c makefile
		$(CC) $(CFLAGS) hrun.c -o hrun
hrun_controller: hrun_controller.c makefile
		$(CC) $(CFLAGS) hrun_controller.c -o hrun_controller
hrun_parent: hrun_parent.c makefile
		$(CC) $(CFLAGS) hrun_parent.c -o hrun_parent
hrunik: hrunik.c makefile
		$(CC) $(CFLAGS) hrunik.c -o hrunik
		

