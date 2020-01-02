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
#include <string.h>

typedef struct {

  int random;
  int counter;
  pid_t pid;

}entry;

//entry constructor
entry constructor(pid_t pid,int counter){
  entry to_be_returned;
  to_be_returned.random = rand();
  to_be_returned.counter = counter;
  to_be_returned.pid = pid;
  return to_be_returned;
}

//heap's length
int get_len(entry **heap){
  return (int)( sizeof(heap)/sizeof(entry *) );
}

//swap two elements in heap
entry **swap(entry **heap,int entr1,int entr2){

  entry help = *heap[entr1];
  heap[entr1] = (entry *)realloc(heap[entr1] ,sizeof( *heap[entr2] ) );

  *heap[entr1] = *heap[entr2];

  heap[entr2] = (entry *)realloc(heap[entr2] ,sizeof(help) );
  *heap[entr2] = help;

  return heap;

}

//move an element one entry upwards
entry **heap_up(entry **heap,int len){
  if(len == 1 || len == 0){
  //there are 1 or 0 elements
    if(heap[len]->random <= heap[0]->random){
      //if it is to swap with the root
      return swap(heap,len,0);
    }else{
      //the heap is OK!
      return heap;
    }

  }
  else if(len%2 == 0){
  //len is a multiple of 2
    if( heap[len]->random < heap[ (int)(len/2) ]->random ){
      //Have to move upwards
      return heap_up(heap, (int)(len/2) );
    }else{
      //the heap is OK!
      return heap;
    }

  }
  else{
    //len is not a multiple of 2
    if( heap[len]->random < heap[ (int)( (len-1) / 2 ) ]->random ){
      //Have to move upwards
      return heap_up(heap ,(int)( (len-1) / 2 ) );
    }else{
      //The heap is OK!
      return heap;
    }

  }

}

//Add an element and sort the heap
entry **add_element(entry **heap,entry element){
  //Heap's length
  static int len = 0;
  len+=1;

  heap = (entry **)realloc(heap,sizeof(element));
  heap[len-1] = (entry *)realloc(heap[len-1],sizeof(element));
  *heap[len-1] = element;
  return heap_up(heap,len-1);
}

void main(){
  entry **mheap = (entry **)malloc( sizeof(entry *) );
  for(int i = 0; i<=10; i++){
    entry entr = constructor(i+2,i);
    mheap = add_element(mheap,entr);
    printf("Turn %d\nProcess %d\nRandom %d\n",i,mheap[i]->pid,mheap[i]->random);
  }
}
