#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SEMAPH1 "/msem1"
#define SEMAPH2 "/msem2"
#define SEMAPH3 "/msem3"
#define SEMAPH4 "/msem4"
#define SEMAPH5 "/msem5"

//save semaphore names in an array
const char *arr[] = {SEMAPH1,SEMAPH2,SEMAPH3,SEMAPH4,SEMAPH5};

//My semaphores
sem_t *msema[5];

void main(){

  //apo8hkeush twn pid gia swsth leitourgia twn signal
  int memid[4];
  pid_t *pid[4];
  //init shared memory
  for(int i = 0; i<=3; i++){
    memid[i] = shmget(IPC_PRIVATE,sizeof(pid_t *),IPC_CREAT | 0774);
    pid[i] = (pid_t *)shmat(memid[i],NULL,0);
  }

  //init semaphores
  for(int i = 0; i<=4; i++){
    msema[i] = sem_open(arr[i],O_CREAT,0774,0);
  }

  //Buffers
  int bufid[3];
  int *buffer[3];
  //init Buffers
  for(int i = 0; i<=2; i++){
    bufid[i] = shmget(IPC_PRIVATE,sizeof(int *),IPC_CREAT | 0774);
    buffer[i] = (int *)shmat(bufid[i],NULL,0);
  }

  //Produce Processes
  for(int i = 0; i<=4; i++){
    pid_t tempid = fork();

    //Child process
    if(tempid == 0){
      //signal handlers
      signal(SIGCONT,SIG_DFL);
      signal(SIGSTOP,SIG_DFL);

      printf("Child process %i goes to sleep\n" ,getpid() );
      kill(getpid(),19);
      break;
    }
    //Parent process
    else if(tempid>0){
      *pid[i]=tempid;
    }
    //Error
    else{
      printf("Fork error %i\n", errno);
    }

  }

  for(int j = 0; j<=100; j++){

  //Process 2
  //Child Process
  if(getpid() == *pid[0]){
    //Block until P1 is done
    sem_wait(msema[0]);
    //P1 is Unblocked
    *buffer[1] = *buffer[0];
    //Unblock P4
    sem_post(msema[2]);
    //Block P2 (self)
    usleep(5);
    sem_wait(msema[2]);
    sem_post(msema[0]);
  }
  //Process 3
  //Child Process
  else if(getpid() == *pid[1]){
    //Block until P1 is done
    sem_wait(msema[1]);
    //P1 is Unblocked
    *buffer[2] = *buffer[0];
    //Unblock P5
    sem_post(msema[3]);
    //Block P3 (self)
    usleep(5);
    sem_wait(msema[3]);
    sem_post(msema[1]);
  }
  //Process 4
  //Child Process
  else if(getpid() == *pid[2]){
    //Block until P2 is done
    sem_wait(msema[2]);
    //P4 is Unblocked
    *buffer[1] += rand()%10;
    //Unblock P2
    usleep(5);
    sem_post(msema[2]);
  }
  else if(getpid() == *pid[3]){
    //Block until P3 is done
    sem_wait(msema[3]);
    //P5 is Unblocked
    *buffer[1] += rand()%10;
    //Unblock P3
    usleep(5);
    sem_post(msema[3]);
  }
  //Process 1
  //Parent Process
  else{
    //signal handlers
    signal(SIGCONT,SIG_DFL);
    signal(SIGSTOP,SIG_DFL);
    //Wake up children
    for(int i = 0; i<=3; i++){
      printf("Child %i wakes up\n", *pid[i]);
      kill(*pid[i],18);
    }

    *buffer[0] = rand()%10;
    //Unblock P2
    sem_post(msema[0]);
    //Unblock P3
    sem_post(msema[1]);

    //Block P1 (self)
    usleep(5);
    sem_wait(msema[0]);
    sem_wait(msema[1]);

    if(*buffer[1]>*buffer[2]){printf("Buffer 1 : %i > Buffer 2 : %i\nP3 : %i > P4 : %i\n",*buffer[1],*buffer[2],*pid[3],*pid[4]);}
    else{printf("Buffer 1 : %i > Buffer 2 : %i\nP3 : %i > P4 : %i\n",*buffer[2],*buffer[1],*pid[4],*pid[3]);}
    //Wait for children
    //int status;
    //for(int i = 0; i<=3; i++){ waitpid(-1,&status,0); }
  }
}

}
