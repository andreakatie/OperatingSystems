/*
 * Andrea Gray
 * Operating Systems CS 420
 * Homework 4
 * Inter-Process Communication/Synchronization via Shared Memory
 */ 
 
#include <errno.h>      // for error handling like perror() and strerror()
#include <stdio.h>      // for I/O functions like printf()
#include <sys/types.h>  // for non-standard data types used
#include <string.h>     // for string manipulations and in this file for strerror specifically
#include <unistd.h>     // for declarations that compromise the POSIX OS like NULL
#include <sys/ipc.h>    // for inter-process communications
#include <sys/shm.h>    // for shared memory functions

int main () {
  int shmID;         // shared memory ID
  char* shmAddress;  // memory address of shared memory
  int error;         // variable for error handling
  int parentPID;     // Process ID of the parent process
  int childPID;      // Process ID of the child process
  int process;       // return value of the fork() call
  /* int shmKey is the value stored within the shared memory location, declared below */
  
  /* Creating a shared memory ID */
  shmID = shmget(IPC_PRIVATE, 1024, IPC_CREAT); 
  if (shmID < 0) { /* shmget failed */
    printf("\n\n There's an error at line %d, %s\n\n", __LINE__, strerror(shmID)); // prints the error 
    perror("\n\n Creating a shared memory ID failed.\n\n"); // prints error message
 	}
 	else { /* shmget was successful */
		printf("\nParent: Successfully created shared memory segment with shared memory ID # of %d\n", shmID);
 	}
  
  /* Specifying the address of the shared memory and attaching a shared memory segment */
  shmAddress = (char*)shmat(shmID, NULL, 0); 
  if (shmAddress == (void *)-1) { /* shmat failed */
    perror("\n\n There was an error when a shared memory segment was being attached. \n\n");
  }
  else {
    parentPID = getpid(); /* shmat was successful */
    printf("\n Parent: My PID is %d; now spawning a child after setting the shared integer to 0.\n", parentPID);
  }
  
   int shmKey __attribute__ ((at(shmAddress)));
   shmKey = 0; /* Parent process sets the shared memory value to zero */
  
  /* Child Process */
  process = fork();
  if (process != 0) { /* If the child process creation fails or runs into an error */
    printf("\n\n There's an error at line %d, %s\n\n", __LINE__, strerror(process)); 
    perror("\n\n There was an error when creating the process.\n\n"); 
 	}
  else { /* If the child process creation is successful */
    childPID = getpid();
    printf("\n     Child: My PID is %d, my parent's PID is %d;\n", childPID, parentPID);
    printf("\n     the shared integer value is currently 0; I'll spin until its not 0.\n");
    while (shmKey = 0) {} /* Spinning until user puts a new integer into the shared memory ID */
    printf("\n     Child: The value in the shared integer is now %i; I'll set it back to 0.", shmKey);
    shmKey = 0; /* Resets the shared memory ID back to zero */
    printf("\n     Child process terminating.\n");
  }
  
  /* Back to Parent Process */
  printf("\n Parent: My PID is %d, spawned a child with PID of %d;", parentPID, childPID);
  scanf(" Please enter an integer to be stored in the shared memory: %i", shmKey);
  
  while(shmKey != 0) {} /* spinning until the shared memory ID is 0 again */
  
  printf("\n Parent: the child has re-zeroed our shared integer. \n");
  
  error = shmdt(&shmAddress); /* detaching the shared memory from the address space */
  if (error != 0) { /* shmdt failed */
    printf("\n\n There's an error at line %d, %s\n\n", __LINE__, strerror(error)); 
    perror("\n\n Error in shmdt.\n\n");
  }
  
  printf("\n Parent: Child terminated; parent successfully removed segment whose ID # was %d", shmID);
  
  error = shmctl(shmID, IPC_RMID, NULL); /* returning the shared memory to the memory manager */
  if (error != 0) { /* shmctl failed */
    printf("\n\n There's an error at line %d, %s\n\n", __LINE__, strerror(error)); 
    perror("\n\n Error in shmctl\n\n"); 
  }
}