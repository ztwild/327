#include <stdio.h>
#include <stdlib.h>

#define grid_size 8

void print_result(int *rows){
  int i;
  char *letters = "abcdefgh";
  
  for(i = 0; i < grid_size; i++){
    printf("%c%d", letters[i], rows[i]+1);
  }
  printf("\n");
}

int is_diagonal(int* r, int c1, int c2){
  int j = abs(r[c1] - r[c2]);
  int k = abs(c1 - c2);
  return j == k;
}

int is_valid(int *r, int c){
  int i;
  for(i = 0; i < c; i++){
    if(r[i] == r[c] || is_diagonal(r, i, c)){
      return 0;
    }
  }
  return 1;
}

int rec(int *rows, int c){
  int i;

  for(i = 0; i < grid_size; i++){
    rows[c] = i;
    if(is_valid(rows, c)){
      if(c < grid_size - 1){
        rec(rows, c+1);
      }
      else{
        print_result(rows);
      }
    }
  }

  return 0;
}

int main(int argc, char* argv){
  int i, rows[grid_size];

  for(i = 0; i < grid_size; i++){
    rows[0] = i;
    rec(rows, 1);
  }

  return 0;
}