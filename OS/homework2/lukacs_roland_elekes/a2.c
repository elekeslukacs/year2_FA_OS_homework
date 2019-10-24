#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdbool.h>
#include "a2_helper.h"

int sem_id;
// pthread_t th7[40];

void P(int sem_id, int sem_no)
{
    struct sembuf op = {sem_no, -1, 0};

    semop(sem_id, &op, 1);
}

void V(int sem_id, int sem_no)
{
    struct sembuf op = {sem_no, +1, 0};

    semop(sem_id, &op, 1);
}

pthread_t th[4];

void *threadFunctionP6(void *th_number){
  int *number = (int *)th_number;
  if(*number == 2){
    P(sem_id, 4);

    info(BEGIN, 6, 2);
    info(END, 6, 2);

    V(sem_id, 3);
  }
  else{

  info(BEGIN, 6, *number);

  if(*number == 3){
    int t1 = 1;
    if(pthread_create(&th[0], NULL, threadFunctionP6, (void *)&t1)!=0){
      perror("Cannot create threads");
      exit(1);
    }
    pthread_join(th[0], NULL);
  }

  info(END, 6, *number);
}
  return NULL;
}


void *threadFunctionP7( void *th_number){
  int *number = (int *)th_number;

  P(sem_id, 0);
  switch (*number){
    case 1:
      info(BEGIN, 7, 1);
      V(sem_id, 1);

      P(sem_id, 2);
      info(END, 7, 1);
      break;
    case 2:
      info(BEGIN, 7, 2);
      V(sem_id, 1);

      P(sem_id, 2);
      info(END, 7, 2);
      break;
    case 3:
      info(BEGIN, 7, 3);
      V(sem_id, 1);

      P(sem_id, 2);
      info(END, 7, 3);
      break;
    case 4:
      info(BEGIN, 7, 4);
      V(sem_id, 1);

      P(sem_id, 2);
      info(END, 7, 4);
      break;
    case 13:
      P(sem_id, 1);
      P(sem_id, 1);
      P(sem_id, 1);
      P(sem_id, 1);
      info(BEGIN, 7, 13);
      info(END, 7, 13);
      V(sem_id, 2);
      V(sem_id, 2);
      V(sem_id, 2);
      V(sem_id, 2);

      break;
    default:
      info(BEGIN, 7, *number);
      info(END, 7, *number);
      break;
  }

  V(sem_id, 0);

  return NULL;
}


// pthread_t th2[4];

void *threadFunctionP2(void *th_number){
  int *number = (int *)th_number;
if(*number == 1){
  info(BEGIN, 2, 1);
  info(END, 2, 1);
  V(sem_id, 4);
}
else if(*number == 4){

  P(sem_id, 3);
  info(BEGIN, 2, 4);
  info(END, 2, 4);
}
else{
  info(BEGIN, 2, *number);
  info(END, 2, *number);
}
  return NULL;
}

int main(){
    init();
    int pid[9];
    pid[0] = 0;
    pid[1] = getpid();
    info(BEGIN, 1, 0);
    sem_id = semget(IPC_PRIVATE, 5, IPC_CREAT | 0600);
        if (sem_id < 0) {
            perror("Error creating the semaphore set");
            exit(2);
        }
    semctl(sem_id, 0, SETVAL, 5);
    semctl(sem_id, 1, SETVAL, 0);
    semctl(sem_id, 2, SETVAL, 0);
    semctl(sem_id, 3, SETVAL, 0);
    semctl(sem_id, 4, SETVAL, 0);
    pid[2] = fork();
    switch (pid[2]) {
      case -1: // error case
          perror("Cannot create a new child");
          exit(1);
      case 0: //child, process p2
          info(BEGIN, 2, 0);
          int t2[] = {1, 2, 3, 4};
          int i;
          pthread_t th2[4];
          for(i = 0; i <4; i++){
            if (pthread_create(&th2[i], NULL, threadFunctionP2, (void *)&t2[i])!= 0){
              perror("Cannot create threads");
              exit(1);
            }
          }

          // for(int i = 0 ; i < 4; i++){
          //     pthread_join(th2[i], NULL);
          //  }
          pid[4] = fork();
          switch (pid[4]){
            case -1: // error case
                perror("Cannot create a new child");
                exit(1);
            case 0: //child of p2, p4
                info(BEGIN, 4, 0);
                pid[7] = fork();
                switch (pid[7]){
                    case -1: // error case
                        perror("Cannot create a new child");
                        exit(1);
                    case 0: //child of p4, p7
                        info(BEGIN, 7, 0);
                        pthread_t th7[40];
                        int i;
                        int iduri[40];
                        for(int i = 0; i < 40; i++){
                          iduri[i] = i+1;
                        }
                        for(i = 0; i <40; i++){
                          if (pthread_create(&th7[i], NULL, threadFunctionP7, (void *)&iduri[i])!= 0){
                            perror("Cannot create threads");
                            exit(1);
                        }
                      }
                        for(int i = 0 ; i < 40; i++){
                            pthread_join(th7[i], NULL);
                         }

                        info(END, 7, 0);
                        break;
                    default: //parent, p4
                        pid[8] = fork();
                        if(pid[8] == 0){
                          info(BEGIN, 8, 0);
                          info(END, 8, 0);
                        }
                        else{
                        waitpid(pid[7], NULL, 0);
                        waitpid(pid[8], NULL, 0);
                        info(END, 4, 0);
                      }
                        break;
                }
                break;
            default: //parent, p2
                pid[5] = fork();
                switch (pid[5]) {
                  case -1: // error case
                      perror("Cannot create a new child");
                      exit(1);
                  case 0: //child of p2, process p5
                      info(BEGIN, 5, 0);
                      info(END, 5, 0);
                      break;
                  default: //parent, p2
                      pid[6] = fork();
                      switch (pid[6]) {
                        case -1: // error case
                            perror("Cannot create a new child");
                            exit(1);
                        case 0: //child of p2, process p6
                            info(BEGIN, 6, 0);
                            int t[] = {2, 3, 4};
                            if(pthread_create(&th[2], NULL, threadFunctionP6, (void *)&t[0])!=0){
                              perror("Cannot create threads");
                              exit(1);
                            }

                            if(pthread_create(&th[1], NULL, threadFunctionP6, (void *)&t[1])!=0){
                              perror("Cannot create threads");
                              exit(1);
                            }

                            if(pthread_create(&th[3], NULL, threadFunctionP6, (void *)&t[2])!=0){
                              perror("Cannot create threads");
                              exit(1);
                            }

                            pthread_join(th[2], NULL);
                            pthread_join(th[1], NULL);
                            pthread_join(th[3], NULL);
                            info(END, 6, 0);
                            break;
                        default: //parent, p2
                            waitpid(pid[6], NULL, 0);
                            waitpid(pid[5], NULL, 0);
                            waitpid(pid[4], NULL, 0);
                            for(int i = 0 ; i < 4; i++){
                                pthread_join(th2[i], NULL);
                             }
                            info(END, 2, 0);
                            break;
                      }
                      break;
                }
              break;
          }
          break;
      default: //parent, p1
          pid[3] = fork();
          if(pid[3] == 0){
            info(BEGIN, 3, 0);
            info(END, 3, 0);
          }
          else{
          //remove semaphores
          waitpid(pid[2], NULL, 0);
          waitpid(pid[3], NULL, 0);
          semctl(sem_id, 0, IPC_RMID, 0);
          semctl(sem_id, 1, IPC_RMID, 0);
          semctl(sem_id, 2, IPC_RMID, 0);
          semctl(sem_id, 3, IPC_RMID, 0);
          semctl(sem_id, 4, IPC_RMID, 0);
          info(END, 1, 0);
        }
          break;
    }

    return 0;
}
