#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>

/*
Ar fi fost frumos sa scriu functii, dar un ultima clipa am terminat tema si nu am mai avut timp
de scris functii, imi cer scuze.
*/

#define STRING_MAX_LENGTH 31 //size of longest request message

char *sharedValue = NULL;
char *data = NULL;

void writeSuccess(int fd){
  int msgLen = 7;
  write(fd, &msgLen , 1);
  write(fd, "SUCCESS", msgLen);
}
void writeError(int fd){
  int msgLen = 5;
  write(fd, &msgLen, 1);
  write(fd, "ERROR", msgLen);
}


int main(int argc, char** argv){

  int fd1, fd2, shmId;
  int size; //file size
  unsigned int offset;
  unsigned int no_of_bytes;
  int section_SF;
  int offset_SF;
  int size_SF;

  if(mkfifo("RESP_PIPE_96623", 0600) < 0){
    perror("ERROR\ncannot create the response pipe\n");
    exit(0);
  }

  fd2 = open("REQ_PIPE_96623", O_RDONLY);
  if(fd2 < 0){
    perror("ERROR\n cannot open the request pipe\n");
    exit(0);
  }

  fd1 = open("RESP_PIPE_96623", O_WRONLY);
  if(fd1 < 0){
    perror("ERROR\n cannot open the response file\n");
    exit(0);
  }
  int length = 7;
  write(fd1, &length, 1);
  if(write(fd1, "CONNECT", 7) < 0){
    perror("ERROR\ncannot write\n");
    exit(0);
  }
  printf("SUCCESS\n");


  int stringLen = 0;
  int shmSize = 0;

  char request[STRING_MAX_LENGTH];

  while(12 > 11){
    read(fd2, &stringLen, 1);
    read(fd2, request, stringLen);
    request[stringLen] = '\0';

    if(strcmp(request, "PING") == 0){

      int lengthPong = 4;
      write(fd1, &lengthPong, 1);
      write(fd1, "PING", 4);
      write(fd1, &lengthPong, 1);
      write(fd1, "PONG", 4);
      int value = 96623;
      write(fd1, &value, 4);
    }



    if(strcmp(request, "CREATE_SHM") == 0){

      read(fd2, &shmSize, 4);
      shmId = shmget(18847, sizeof(char) * shmSize, IPC_CREAT | 0664);
      write(fd1, &stringLen, 1);
      write(fd1, "CREATE_SHM", stringLen);
      if(shmId < 0){
        writeError(fd1);
        continue;
      }
      sharedValue = (char *) shmat(shmId, NULL, 0);

      writeSuccess(fd1);

    }



    if(strcmp(request, "WRITE_TO_SHM") == 0){
      offset = 0;
      read(fd2, &offset, 4);
      unsigned int value = 0;
      read(fd2, &value, 4);

      write(fd1, &stringLen, 1);
      write(fd1, "WRITE_TO_SHM", stringLen);

      if(offset <= shmSize && offset > 0){
          if(offset+4 <= shmSize){
            if(sharedValue == (void*)-1){
              perror("Could not attach to shm\n");
              exit(0);
            }
            char *bytesOfValue = (char*)(&value);
            for(int i = 0; i < 4; i++){
              sharedValue[offset + i] = bytesOfValue[i];
            }
            writeSuccess(fd1);
            continue;
          }
        }

        writeError(fd1);
      }



    if(strcmp(request, "MAP_FILE") == 0){
      int nameLength = 0;
      read(fd2, &nameLength, 1);
      char fileName[nameLength];
      read(fd2, fileName, nameLength);
      fileName[nameLength] = '\0';

      int fd = open(fileName, O_RDONLY);
      if(fd == -1){
        printf("%s\n", "file opening error");
      }

      size = lseek(fd, 0, SEEK_END);
      lseek(fd, 0, SEEK_SET);

      data = (char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
      write(fd1, &stringLen, 1);
      write(fd1, "MAP_FILE", stringLen);

      if(data == (void*)-1){
        writeError(fd1);
        close(fd);
        continue;
      }
      writeSuccess(fd1);
    }



    if (strcmp(request, "READ_FROM_FILE_OFFSET") == 0){
      offset = 0;
      read(fd2, &offset, 4);
      no_of_bytes = 0;
      read(fd2, &no_of_bytes, 4);
      char buffer[no_of_bytes];
      write(fd1, &stringLen, 1);
      write(fd1, "READ_FROM_FILE_OFFSET", stringLen);
      if(offset >= 0 && offset <= size){
        //memcpy(buffer, data + offset, no_of_bytes);
        //memcpy(sharedValue + 0, buffer, no_of_bytes);
        for(int i = 0; i < no_of_bytes; i++){
          buffer[i] = data[offset + i];
          sharedValue[i] = buffer[i];
        }
        writeSuccess(fd1);
        continue;
      }
      writeError(fd1);
    }



    if(strcmp(request, "READ_FROM_FILE_SECTION") == 0){
      unsigned int section_no = 0;
      read(fd2, &section_no, 4);
      offset = 0;
      read(fd2, &offset, 4);
      no_of_bytes = 0;
      read(fd2, &no_of_bytes, 4);

      write(fd1, &stringLen, 1);
      write(fd1, "READ_FROM_FILE_SECTION", stringLen);

      section_SF = data[5];
      if(section_no > section_SF){
        printf("Section NO is %d and section SF is %d\n", section_no, section_SF);
        writeError(fd1);
        continue;
      }

      offset_SF = 0;
      memcpy(&offset_SF, data + 6 + (section_no-1) * 27 + 19, 4);
      size_SF = 0;
      memcpy(&size_SF, data + 6 + (section_no-1) * 27 + 23, 4);

    //  printf("offset_SF is %d and size_SF is %d\n", offset_SF, size_SF);
      if(offset + no_of_bytes > offset_SF + size_SF || no_of_bytes > size_SF){
        printf("OFFSET + no_of_bytes is %d and offset_SF is %d\n", offset + no_of_bytes, offset_SF);
        writeError(fd1);
        continue;
      }
    //  printf("No of bytes is %d\n", no_of_bytes);
      char buffer2[no_of_bytes+1];
      buffer2[no_of_bytes] = '\0';
      memcpy(buffer2, data + offset_SF + offset, no_of_bytes);
      memcpy(sharedValue, buffer2, no_of_bytes);

      writeSuccess(fd1);
    }



    if(strcmp(request, "READ_FROM_LOGICAL_SPACE_OFFSET") == 0){
      read(fd2, &offset, 4);
      read(fd2, &no_of_bytes, 4);
      section_SF = data[5];
      int logicalSections = 0;
      int isInMemoryFlag = 0;
      char buffer3[no_of_bytes+1];
      buffer3[no_of_bytes] = '\0';

      write(fd1, &stringLen, 1);
      write(fd1, "READ_FROM_LOGICAL_SPACE_OFFSET", stringLen);

      for(int i = 0; i < section_SF; i++){
        memcpy(&offset_SF, data + 6 + i * 27 + 19, 4);
        memcpy(&size_SF, data + 6 + i * 27 + 23, 4);
        logicalSections += size_SF / 5210 + 1;
        if(offset <= logicalSections * 5120){
          isInMemoryFlag = 1;
          if(no_of_bytes > size_SF || offset % 5120 + no_of_bytes > offset_SF + size_SF){
            writeError(fd1);
            break;
          }
          memcpy(buffer3, data + offset_SF + offset % 5120, no_of_bytes);
          memcpy(sharedValue, buffer3, no_of_bytes);
          writeSuccess(fd1);
          break;
        }
      }

      if(isInMemoryFlag == 0){
        writeError(fd1);
      }

    }



    if(strcmp(request, "EXIT") == 0){
        close(fd2);
        close(fd1);
        unlink("RESP_PIPE_96623");
        break;
      }


  }

  return 0;
}
