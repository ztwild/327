#include <stdio.h>

typedef enum action {
  read_binary,
  write_binary,
  read_text,
  write_text  
} action_t;

int main(int argc, char *argv[]){
  FILE *f;
  struct {
    char i;
    char j;
    
  } s = { 1, 2 };
  action_t action;
  
  if(argc != 3){
    fprintf(stderr, "wrong number of parameters.\n");
    return -1;
  }
  
  /* ./program -wt -wb -rt -rb */
  
  if(argv[1][1] == 'w'){
    if(argv[1][2] == 't'){
      action = write_text;
    }else{
      action = write_binary;
    }
    fopen(argv[2], 'r');
  }else{
    if(argv[1][2] == 't'){
      action = read_text;
    }else{
      action = read_binary;
    }
    fopen(argv[2], 'r');
  }
  
  if(!f) {
    
  }
  
  return 0;
}
