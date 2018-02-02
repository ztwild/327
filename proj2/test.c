#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "load_save.c"


void print_hardness(dungeon_t *d){
  int y, x;
  for(x = 0; x < X_LENGTH; x++){
    for(y = 0; y < Y_LENGTH; y++){
      printf("%3d ", d->hardness[y][x]);
    }
    printf("\n");
  }
}

dungeon_t *load_file(char *name){
  int i;
  uint32_t version, size, little_endian;
  char *semantic = malloc(sizeof(char) * 12);
  
  dungeon_t *d = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(d);
  
  char *file_path = malloc(strlen(name) + strlen("examples/") + 1);
  strcpy(file_path, "examples/");
  strcat(file_path, name);
  
  FILE *f = fopen(file_path, "w");
  if(!f){
    fprintf(stderr, "Couldn't open file\n");
    return 0;
  }
  free(file_path);
  
  fread(semantic, sizeof(char), 12, f);
  fread(&version, sizeof(uint32_t), 1, f);
  fread(&size, sizeof(uint32_t), 1, f);
  fread(d->hardness, sizeof(uint8_t), X_LENGTH * Y_LENGTH, f);
  
  little_endian = be32toh(size);
  d->room_count = (little_endian - 1700) / 4;
  d->rooms = (room_t**)malloc(sizeof(room_t*) * d->room_count);
  
  for(i = 0; i < d->room_count; i++){
    room_t *r = (room_t*)malloc(sizeof(room_t));
    fread(&r->pos_y, sizeof(uint8_t), 1, f);
    fread(&r->pos_x, sizeof(uint8_t), 1, f);
    fread(&r->size_y, sizeof(uint8_t), 1, f);
    fread(&r->size_x, sizeof(uint8_t), 1, f);
    d->rooms[i] = r;
  }
  
  hardness_to_grid(d);
  insert_rooms(d);
  
  fclose(f);
  return d;
}

int main(){
  char *a = "1521618087.rlg327";
  char *b = "1522914515.rlg327";
  char *c = "1523530501.rlg327";
  char *d = "1524171099.rlg327";
  
  char *e = "1524787644.rlg327";
  char *f = "1525363666.rlg327";
  char *g = "1525972216.rlg327";
  char *h = "1526652288.rlg327";
  
  char *i = "1527308739.rlg327";
  
  
  dungeon_t *dun = load_file(a);
  print_hardness(dun);

}
