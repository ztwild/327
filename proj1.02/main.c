#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX 100

struct stat st = {0};

void init_directory(char *path){
  if(stat(path, &st) == -1){
    mkdir(path, 0700);
  }
}

char  *get_path(){
  char *home = getenv("HOME");
  char *path = malloc(MAX);
  path = strcat(home, "/.rlg327");
  return path;
}

int main(int argc, char **argv){
    


  return 0;
}
