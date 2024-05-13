CC=gcc
CFLAGS= -W -Wextra -std=c11
.PHONY: clean

all: hrun hrun_controller hrun_parent hrunik
hrun: ./src/hrun.c makefile
		$(CC) $(CFLAGS) ./src/hrun.c -o ./hrun
hrun_controller: ./src/hrun_controller.c makefile
		$(CC) $(CFLAGS) ./src/hrun_controller.c -o ./bin/hrun_controller
hrun_parent: ./src/hrun_parent.c makefile
		$(CC) $(CFLAGS) ./src/hrun_parent.c -o ./bin/hrun_parent
hrunik: ./src/hrunik.c makefile
		$(CC) $(CFLAGS) ./src/hrunik.c -o ./bin/hrunik
		

