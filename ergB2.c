#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>

//semaphore names
#define SEMAPHORE1 "/msem1"
#define SEMAPHORE2 "/msem2"
#define SEMAPHORE3 "/msem3"
#define SEMAPHORE4 "/msem4"
#define SEMAPHORE5 "/msem5"
//semaphore pointers
sem_t *sema[5];

void main(){
  //semaphore names in an array
  const char *semnames[] = {SEMAPHORE1,SEMAPHORE2,SEMAPHORE3,SEMAPHORE4};
  //open semaphores
  for(int i = 0; i <= 3; i++){sema[i] = sem_open(semnames[i],O_CREAT,0774,0);}

  //save process ids
  int memid[5];
  pid_t *pids[5];
  //initialize shared memory
  for(int i = 0; i<=4; i++){
    memid[i] = shmget(IPC_PRIVATE,sizeof(pid_t *),IPC_CREAT | 0774);
    pids[i] = (pid_t *)shmat(memid[i],NULL,0);
  }

  //buffers
  int bufid[3];
  int *buf[3];
  //initialize buffers
  for(int i = 0; i<=2; i++){
    bufid[i] = shmget(IPC_PRIVATE,sizeof(int *),IPC_CREAT | 0774);
    buf[i] = (int *)shmat(bufid[i],NULL,0);
  }

  //create processes
  for(int i = 0; i<=3; i++){

    pid_t check = fork();
    //child process
    if(check == 0){
      //signal handlers
      signal(SIGCONT,SIG_DFL);
      signal(SIGCONT,SIG_DFL);

      printf("Child process %d\n",getpid() );
      kill(getppid(),18);
      kill(getpid(),19); //%s goes to sleep until all processes are created
      break;

    //Parent Process
    }else{
      signal(SIGCONT,SIG_DFL);
      signal(SIGCONT,SIG_DFL);

      kill(getpid(),19);
      printf("Parent process %i\n", getpid());
      //save child's id
      *pids[i] = check;
      //save parent's id
      if(i==3){*pids[4] = getpid();}
    }
  //end of process creation
  }

  //Process 1
  if(getpid() == *pids[4]){

    //Wake up all processes
    for(int i = 0; i<=3; i++){
      kill(*pids[i],18);
    }

    int i = 0;

    while(1){

      *buf[0] = rand()%10;

      //wake up P2 and P3
      sem_post(sema[0]);
      sem_post(sema[1]);

      //block until P2 and P3 are Done
      usleep(5);
      sem_wait(sema[0]);
      sem_wait(sema[1]);
      if(i == 100){
        printf("Can\'t stand more\n");
        break;
      }

      if(*buf[1] == *buf[2]){
        printf("Tie \nTurn %i\n",i);
      }else if(*buf[1]>*buf[2]){
        printf("Process 4 : %i\nTurn : %i\n",*buf[1],i );
      }else{
        printf("Process 5 : %i\nTurn : %i\n",*buf[2],i );
      }

      i++;

    }

  }
  //Process 2
  else if(getpid() == *pids[0]){

    int i = 0;

    while(1){

            //Block until P1 unblocks
            sem_wait(sema[0]);

            *buf[1] = *buf[0];
            //unblock P4
            sem_post(sema[2]);

            //Block until P4 is Done
            usleep(5);
            sem_wait(sema[2]);

            //unblock P1
            sem_post(sema[0]);
            usleep(5);

            i++;

    }

  }
  //Process 3
  else if(getpid() == *pids[1]){

    int i = 0;

    while(1){

            //Block until P1 unblocks
            sem_wait(sema[1]);
            *buf[2] = *buf[0];
            //unblock P5
            sem_post(sema[3]);

            //Block until P5 is Done
            usleep(5);
            sem_wait(sema[3]);

            //unblock P1
            sem_post(sema[1]);
            usleep(5);

            i++;

    }

  }
  //Process 4
  else if(getpid() == *pids[2]){

    int i = 0;

    while(1){

      //Block until P2 unblocks
      sem_wait(sema[2]);
      *buf[1]=*buf[1] + rand()%10;
      //unblock P2
      sem_post(sema[2]);
      usleep(5);

      i++;

    }

  }
  //Process 5
  else if(getpid() == *pids[3]){

    int i = 0;

    while(1){

      //block until P3 unblocks
      sem_wait(sema[3]);
      *buf[2] = *buf[2] + rand()%10;
      //unblock P3
      sem_post(sema[3]);
      usleep(5);

    i++;

    }

  }

}
