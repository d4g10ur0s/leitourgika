#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <sys/wait.h>

void foo(void){
  int x = 0;
  x+=10;
}

void main(){

  clock_t start, end;
  start = clock();
  printf("Arxh programmatos : %.5f\n",(double)(start/CLOCKS_PER_SEC) );

  //pid
  pid_t pid;
  //save status
  int status;

  for(int i = 0; i<=99; i++){

    //produce processes
    pid = fork();

    if(pid<0){printf("Error %d\n", errno);}

    if(pid == 0){
      //child process
      foo();
      break;
    }else{
      //parent process
      continue;
    }

  }

  if(pid>0){
    //anamonh na teleiwsoun oles oi diergasies
    for(int i = 0; i<=99; i++){waitpid(-1,&status,0);}

    end = clock();
    printf("Telikh timh deuteroleptwn : %.5f\n",(double) (end-start)/CLOCKS_PER_SEC);
    printf("(end - start)/100 = %.5f\n",(double) ( (end-start)/CLOCKS_PER_SEC )/100 );
  }


}
