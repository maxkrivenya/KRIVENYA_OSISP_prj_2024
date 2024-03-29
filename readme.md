=================================ПРОЦЕНТОВКА==============================

выполняет задачи в .config и обрабатывает указанные getopts

==================================STATE===================================

планировка и исполнение задач   
[ + ]

проверка изменения .config  
[ + ]

опции             
[ +/- ] : нет остановить/удалить/...

установка в bin    
[ - ]  : план - прописать в makefile

резидентность      
[ - ]   (демон?) (исполняется на фоне)

====================================HRUN===============================

hrun - the best task scheduler

==================================HOW TO RUN===========================
```shell
git clone https://github.com/maxkrivenya/KRIVENYA_OSISP_prj_2024
```
```shell
make
```
```shell
./hrun 
```
  optpars:  
 >   -l (list current tasks)   
 >   -e (edit .config with your default system editor)
    

==================================CONFIG===============================

  .config - file with jobs
    format: dd hh mm ss task_that_hrun_needs_to_do  
    example:
>   DD HH M5 1S ~/script.sh   
>   ~/script.sh is executed when current time is X5 minutes (15,25,...) and 1X seconds (11,12,...)         
>   -can use any non-digit placeholder    

==================================ROLES================================

  > hrun - if has optpars, calls hrun_controller, else starts scheduling
  
  > hrun_controller - parses getopts
  
  > hrun_config_checker - checks if .config has been modified since start;
  >  -if changed, restarts hrun. (WIP)
