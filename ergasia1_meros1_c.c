#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>


void main(){
  //save child's exit status
  int status;
  pid_t pid;

  for(int i = 0; i<=9; i++){

    pid = fork();
    //child process
    if(pid == 0){
      printf("**********\n\n");
    }
    //parent process
    else{
      printf("**********\n\n");
      printf("Parent\'s id : %i\n", getppid());
      printf("My id : %i\n", getpid());
      printf("Child\'s id : %i\n", pid);
      break;
    }

  }

  wait(&status);

}
