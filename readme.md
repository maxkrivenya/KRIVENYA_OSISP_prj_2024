
====================================HRUN===============================

hrun - the best task scheduler

==================================HOW TO RUN===========================
```shell
git clone https://github.com/maxkrivenya/KRIVENYA_OSISP_prj_2024 hrun
```
```shell
make
```
```shell
./hrun &
```
  optpars:  
 >   -l = list current tasks   
 >   -e = edit .config with your default system editor     
 >   -s = edit hrun settings (logging/archiving; only do this if yk what ur doing)  
 >   -k = stop hrun execution  
 >   -a = add a new task from the console  

==================================CONFIG===============================

  .config - file with jobs
    format: in DAY dd hh mm ss task_that_hrun_needs_to_do     

  task types:
  1) none : a recurring task that happens at specified times
  2) at   : one-time execution at a specified time
  3) in   : one-time execution after the specified amt of time passes (needs explicit time declaration)

==================================ROLES================================

  > hrun - if has optpars, calls hrun_controller, else starts scheduling
  
  > hrun_controller - parses getopts
  
  > hrun_parent - spawns hruniks, checks if .config has been modified since start;

  > hrunik - executes a task at the specified time
