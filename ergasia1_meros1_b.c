#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>


void main(){
  pid_t pid;
  //enas vrgxos gia dhmiourgeia
  for(int i = 0; i<=9; i++){
    //dhmiourgia diergasiwn
    pid = fork();

    //parent process
    if(pid > 0){
      continue;
      }
    //child process
    else{
      printf("Child Process : %i\n", getpid());
      break;
    }

  }

}
