#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PATH_LEN 256

// listCommand, pathCommand, recursiveCommand, greaterCommand, permissionsCommand check if among the arguments there exists an
// argument for the corresponding command. Returns the index of the argument if yes, returns -1 if nothing is found.

int listCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter ; i++){
    if(strcmp("list", arguments[i]) == 0){
      return i;
    }
  }
  return -1;
}

int parseCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter ; i++){
    if(strcmp("parse", arguments[i]) == 0){
      return i;
    }
  }
  return -1;
}

int pathCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if(strncmp("path=", arguments[i], 5) == 0){
      return i;
    }
  }
  return -1;
}

int recursiveCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if (strcmp("recursive", arguments[i]) == 0){
      return i;
    }
  }
  return -1;
}

int greaterCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if(strncmp("size_greater=", arguments[i], 13) == 0){
      return i;
    }
  }
  return -1;
}

int permissionsCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if(strncmp("permissions=", arguments[i], 12) == 0){
      return i;
    }
  }
  return -1;
}

int extractCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if (strcmp("extract", arguments[i]) == 0){
      return i;
    }
  }
  return -1;
}

int sectionCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if(strncmp("section=", arguments[i], 8) == 0){
      int section = atoi(arguments[i] + 8);
      return section;
    }
  }
  return -1;
}

int lineCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if(strncmp("line=", arguments[i], 5) == 0){
      int line = atoi(arguments[i] + 5);
      return line;
    }
  }
  return -1;
}

int findallCommand(int argCounter, char **arguments){
  for(int i = 1; i < argCounter; i++){
    if (strcmp("findall", arguments[i]) == 0){
      return i;
    }
  }
  return -1;
}

//extracts the permission string from a file given by filename and stores in permFile.
void getPermission(char filename[MAX_PATH_LEN], char permFile[9]){
  struct stat inode;
  stat(filename, &inode);

  strcpy(permFile, "");

  if (inode.st_mode & S_IRUSR)        // check owner's READ permission (S_IRUSR is the mask 0400)
        strcat(permFile, "r");
  else
        strcat(permFile, "-");

  if (inode.st_mode & S_IWUSR)        // check owner's WRITE permission (S_IWUSR is the mask 0200)
        strcat(permFile, "w");
  else
        strcat(permFile, "-");

  if (inode.st_mode & S_IXUSR)        // check owner's EXECUTION permission (S_IXUSR is the mask 0100)
        strcat(permFile, "x");
  else
        strcat(permFile, "-");

  if (inode.st_mode & S_IRGRP)        // check group's READ permission (S_IRGRP is the mask 0040)
        strcat(permFile, "r");
  else
        strcat(permFile, "-");

  if (inode.st_mode & S_IWGRP)        // check group's WRITE permission (S_IWGRP is the mask 0020)
        strcat(permFile, "w");
  else
      strcat(permFile, "-");

  if (inode.st_mode & S_IXGRP)        // check group's EXECUTION permission (S_IXGRP is the mask 0010)
        strcat(permFile, "x");
  else
        strcat(permFile, "-");

  if (inode.st_mode & S_IROTH)        // check others' READ permission (S_IROTH is the mask 0004)
        strcat(permFile, "r");
  else
      strcat(permFile, "-");

  if (inode.st_mode & S_IWOTH)        // check others' WRITE permission (S_IWOTH is the mask 0002)
        strcat(permFile, "w");
  else
        strcat(permFile, "-");


  if(inode.st_mode & S_IXOTH){
    strcat(permFile, "x");

  }
  else {
    strcat(permFile, "-");

  }


}

//simple listing
void listDir(char path[MAX_PATH_LEN], int argCounter, char **arguments){
   int greater = 0;
   char name[MAX_PATH_LEN];
   bool greaterFlag = false;
   bool permissionsFlag = false;
   char permString[10] = "" ;
   char permFile[10] = "";

  if(greaterCommand(argCounter, arguments) > 0){
    greaterFlag = true;
    greater = atoi(arguments[greaterCommand(argCounter, arguments)] + 13);
  }

  if(permissionsCommand(argCounter, arguments) > 0){
    permissionsFlag = true;
    strncpy(permString, arguments[permissionsCommand(argCounter, arguments)] + 12, 10);   //copies the input permission string.
  }


  DIR *dir;
  struct dirent *dirE;
  struct stat inode;

  dir = opendir(path);
  if(dir == 0){
    printf("ERROR\ninvalid directory path");
    return;
  }

   printf("SUCCESS\n");

  while((dirE = readdir(dir)) != 0){
    if(strcmp(dirE->d_name, "..") != 0 && strcmp(dirE->d_name, ".") != 0 ){       // . and .. directories not listed, even if they are listed, tester doesn't accept
      strcpy(name, "");
      strcpy(name, path);
      strcat(name, "/");
      strcat(name, dirE->d_name);
      stat(name, &inode);

      if(greaterFlag){                                                             // if the option introduced was greater_than, this branch active
          if(S_ISREG(inode.st_mode)){
              if(inode.st_size > greater){
                  printf("%s\n", name);
                }
          }
          continue;
      }

      if(permissionsFlag){                                                         // if the option introduces was permissions, this branch active
       getPermission(name, permFile);
       if(strcmp(permString, permFile) == 0){
            printf("%s\n", name);
        }
        continue;
      }
      printf("%s\n", name);                                                         //if no options, just list the names
    }
  }

  closedir(dir);
}
//recursive listing
void listDirRecursive(char path[MAX_PATH_LEN], int argCounter, char **arguments){
  int greater = 0;
  char name[MAX_PATH_LEN];
  bool greaterFlag = false;
  bool permissionsFlag = false;
  char permString[10] = "" ;
  char permFile[10] = "";

 if(greaterCommand(argCounter, arguments) > 0){
   greaterFlag = true;
   greater = atoi(arguments[greaterCommand(argCounter, arguments)] + 13);
 }

 if(permissionsCommand(argCounter, arguments) > 0){
   permissionsFlag = true;
   strncpy(permString, arguments[permissionsCommand(argCounter, arguments)] + 12, 10);   //copies the input permission string.
 }


 DIR *dir;
 struct dirent *dirE;
 struct stat inode;

 dir = opendir(path);
 if(dir == 0){
   printf("ERROR\ninvalid directory path");
   return;
 }

 //printf("SUCCESS\n");

 while((dirE = readdir(dir)) != 0){
   if(strcmp(dirE->d_name, "..") != 0 && strcmp(dirE->d_name, ".") != 0 ){       // . and .. directories not listed, even if they are listed, tester doesn't accept
     strcpy(name, "");
     strcpy(name, path);
     strcat(name, "/");
     strcat(name, dirE->d_name);
     stat(name, &inode);

     if(greaterFlag){                                                             // if the option introduced was greater_than, this branch active
         if(S_ISREG(inode.st_mode)){
             if(inode.st_size > greater){
                 printf("%s\n", name);
               }
         }
         else if(S_ISDIR(inode.st_mode)){
           listDirRecursive(name, argCounter, arguments);
         }
         continue;
     }

     if(permissionsFlag){                                                         // if the option introduces was permissions, this branch active
      getPermission(name, permFile);
      if(strcmp(permString, permFile) == 0){
           printf("%s\n", name);
          // printf("%s\n", name);
           if(S_ISDIR(inode.st_mode)){
             listDirRecursive(name, argCounter, arguments);
           }
       }
       continue;
     }
     printf("%s\n", name);
     if(S_ISDIR(inode.st_mode)){
       listDirRecursive(name, argCounter, arguments);
     }                                                        //if no options, just list the names
   }
 }

 closedir(dir);
}
//parse funtction
void parseFunction(char path[MAX_PATH_LEN]){
  int fd = open(path, O_RDONLY);

  if(fd < 0){
    printf("Invalid file path\n");
  //  exit(1);
    return;
  }
  char magic[2];
  read(fd, magic, 2);
  if(strcmp(magic, "om") != 0){
    printf("ERROR\nwrong magic\n");
    return;
  }

  int header_size = 0;
  int version = 0;
  read(fd, &header_size,  2);

  read(fd, &version, 1);
  if(version < 110 || version > 165){
    printf("ERROR\nwrong version\n");
    return;
  }

  int sect_nr = 0;
  read(fd, &sect_nr, 1);

  if(sect_nr < 5 || sect_nr > 11){
    printf("ERROR\nwrong sect_nr\n");
    return;
  }

  char name[sect_nr][16];
  int type[sect_nr];
  int offset[sect_nr];
  int size[sect_nr];

  for(int i = 0; i < sect_nr; i++){
    read(fd, &name[i], 15);
    name[i][15] = '\0';
    type[i] = 0;
    read(fd, &type[i], 4);
    if(!(type[i] == 67 || type[i] == 94 || type[i] == 95)){
      printf("ERROR\nwrong sect_types\n");
      return;
    }
    offset[i] = 0;
    read(fd, &offset[i], 4);
    size[i] = 0;
    read(fd, &size[i], 4);
  //  printf("szia");
  }

  printf("SUCCESS\n");
  printf("version=%d\nnr_sections=%d\n", version, sect_nr);
  for(int i = 0; i < sect_nr; i++){
    printf("section%d: %s %d %d\n", i+1, name[i], type[i], size[i]);
  }

}
//parse, only chechks if a file is of SF format, if not only returns -1, no message is printed
int parseFunctionNoMessage(char path[MAX_PATH_LEN]){
  int fd = open(path, O_RDONLY);

  if(fd < 0){
    printf("invalid file\n");
  //  exit(1);
    return -1;
  }
  char magic[2];
  read(fd, magic, 2);
  if(strcmp(magic, "om") != 0){
    //printf("ERROR\nwrong magic\n");
    return -1;
  }

  int header_size = 0;
  int version = 0;
  read(fd, &header_size,  2);

  read(fd, &version, 1);
  if(version < 110 || version > 165){
  //  printf("ERROR\nwrong version\n");
    return -1;
  }

  int sect_nr = 0;
  read(fd, &sect_nr, 1);

  if(sect_nr < 5 || sect_nr > 11){
    //printf("ERROR\nwrong sect_nr\n");
    return -1;
  }

  char name[sect_nr][16];
  int type[sect_nr];
  int offset[sect_nr];
  int size[sect_nr];

  for(int i = 0; i < sect_nr; i++){
    read(fd, &name[i], 15);
    name[i][15] = '\0';
    type[i] = 0;
    read(fd, &type[i], 4);
    if(!(type[i] == 67 || type[i] == 94 || type[i] == 95)){
      //printf("ERROR\nwrong sect_types\n");
      return -1;
    }
    offset[i] = 0;
    read(fd, &offset[i], 4);
    size[i] = 0;
    read(fd, &size[i], 4);
  //  printf("szia");
  }

  // printf("SUCCESS\n");
  // printf("version=%d\nnr_sections=%d\n", version, sect_nr);
  // for(int i = 0; i < sect_nr; i++){
  //   printf("section%d: %s %d %d\n", i+1, name[i], type[i], size[i]);
  // }
  return 1;

}

//extracts the desired line
void extractFunction (char path[MAX_PATH_LEN], int sect_nr, int line){
  int fd = open(path, O_RDONLY);
  if(parseFunctionNoMessage(path) < 0){
    printf("invalid file\n");
    return;
  }
  lseek(fd, 0, SEEK_SET);
  lseek(fd, 5, SEEK_SET);
  int sectionNumber = 0;
  read(fd, &sectionNumber, 1);

  if(sectionNumber < sect_nr){
    printf("invalid section\n");
    return;
  }

  lseek(fd, (sect_nr-1) * 27 + 19, SEEK_CUR);

  int size = 0, offset = 0;
  read(fd, &offset, 4);
  read(fd, &size, 4);

  char *section = (char *)malloc(size * sizeof(char));
  lseek(fd, offset, SEEK_SET);
  read(fd, section, size);
  int lineCounter = 0;

  for (int i = 0; i < strlen(section); i++){
    if(section[i] == '\n'){
      lineCounter++;
    }
  }
  int totalLines = lineCounter + 1;
  if(line - 1 > lineCounter){
    printf("invalid line\n");
    return;
  }

  lineCounter = 0;
  char *lineContent = strtok(section, "\n");
  while(lineContent != NULL){
    lineCounter++;
    if(lineCounter == totalLines - line + 1){
      break;
    }
    lineContent = strtok(NULL, "\n");
  }

  printf("SUCCESS\n%s\n", lineContent);
  free(section);
}
//checks if a file is SF with size smaller than 1453
int parseSizeFilter(char path[MAX_PATH_LEN]){
  int fd = open(path, O_RDONLY);

  if(fd < 0){
    printf("invalid file\n");
  //  exit(1);
    return -1;
  }
  char magic[2];
  read(fd, magic, 2);
  if(strcmp(magic, "om") != 0){
    //printf("ERROR\nwrong magic\n");
    return -1;
  }

  int header_size = 0;
  int version = 0;
  read(fd, &header_size,  2);

  read(fd, &version, 1);
  if(version < 110 || version > 165){
  //  printf("ERROR\nwrong version\n");
    return -1;
  }

  int sect_nr = 0;
  read(fd, &sect_nr, 1);

  if(sect_nr < 5 || sect_nr > 11){
    //printf("ERROR\nwrong sect_nr\n");
    return -1;
  }

  char name[sect_nr][16];
  int type[sect_nr];
  int offset[sect_nr];
  int size[sect_nr];

  for(int i = 0; i < sect_nr; i++){
    read(fd, &name[i], 15);
    name[i][15] = '\0';
    type[i] = 0;
    read(fd, &type[i], 4);
    if(!(type[i] == 67 || type[i] == 94 || type[i] == 95)){
      //printf("ERROR\nwrong sect_types\n");
      return -1;
    }
    offset[i] = 0;
    read(fd, &offset[i], 4);
    size[i] = 0;
    read(fd, &size[i], 4);
    if (size[i] > 1453){
      return -1;
    }
  }
  return 1;

}

void findAllFunction(char path[MAX_PATH_LEN]){
  DIR *dir;
  struct dirent *dirE;
  struct stat inode;
  char name[MAX_PATH_LEN];

  dir = opendir(path);
  if (dir == 0)
  {
      printf("ERROR\ninvalid directory path\n");
      return;
  }

  while((dirE = readdir(dir)) != 0){
    if(strcmp(dirE->d_name, "..") != 0 && strcmp(dirE->d_name, ".") != 0 ){       // . and .. directories not listed, even if they are listed, tester doesn't accept
      strcpy(name, "");
      strcpy(name, path);
      strcat(name, "/");
      strcat(name, dirE->d_name);
      stat(name, &inode);

      if(S_ISDIR(inode.st_mode)){
        findAllFunction(name);
      }

      if(S_ISREG(inode.st_mode)){
        if(parseSizeFilter(name) > 0){
          printf("%s\n", name);
        }
      }
    }
  }
  closedir(dir);
}


int main (int argc, char **argv){
  int index1, index2, indexRec, indexParse, indexExtract, indexFindall;
  char path[MAX_PATH_LEN] = "";
  index1 = listCommand(argc, argv);
  index2 = pathCommand(argc, argv);
  indexRec = recursiveCommand(argc, argv);
  indexParse = parseCommand(argc, argv);
  indexExtract = extractCommand(argc, argv);
  indexFindall = findallCommand(argc, argv);

  if(index2 > 0){
    strncpy(path, argv[index2] + 5, MAX_PATH_LEN);
  }

  if(argc >= 2){
      if(strcmp(argv[1], "variant") == 0){
          printf("96623\n");
          return 0;
      }
        if(index1 > 0 && index2 > 0){
          if(indexRec < 0){
            listDir(path, argc, argv);
          }
          else {
            printf("SUCCESS\n");
            listDirRecursive(path, argc, argv);
            return 0;
          }
        }
          if (indexParse > 0 && index2 > 0){
            parseFunction(path);
            return 0;
          }

          if(indexExtract > 0 && index2 > 0){
            int section = sectionCommand(argc, argv);
            int line = lineCommand(argc, argv);
            extractFunction(path, section, line);
            return 0;
          }

          if(indexFindall > 0 && index2 > 0){
            printf("SUCCESS\n");
            findAllFunction(path);
            return 0;
          }
    return 0;
    }

}
