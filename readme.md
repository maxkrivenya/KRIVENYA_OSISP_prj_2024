hrun - the best task scheduler

======================================HOW TO RUN===============================

  1. "make" in hrun directory
  2. ./hrun
     
  optpars: 
    -l (list current tasks)
    -e (edit .config with your default system editor)

======================================CONFIG===================================

  .config - file with jobs
    format: dd hh mm ss task_that_hrun_needs_to_do
      -example: ** ** *5 1* ~/script.sh
        ~/script.sh is executed when current time is *5 minutes (15,25,...) and 1* seconds (11,12,...)
      -can use any non-digit placeholder instead of *

======================================ROLES====================================

  hrun - if has optpars, calls hrun_controller, else starts scheduling
  
  hrun_controller - parses getopts
  
  hrun_config_checker - checks if .config has been modified since start;
    -if changed, restarts hrun. (WIP)
