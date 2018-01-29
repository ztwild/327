#include <stdio.h>
#include <stdlib.h>
#include "dungeon.c"
#include "utils.c"

#define FILE_PATH "rlg327/dungeon"
#define FILE_TYPE "RLG327-S2018"

int save(dungeon_t *d){
  
  FILE *f = fopen(FILE_PATH, "w");
  if(!f){
    fprintf(stderr, "Couldn't open file\n");
    return 0;
  }
  
  int file_size = 1700 + (d->room_count * 4);
   
  fwrite(FILE_TYPE, sizeof(char), 12, f); // 0 - 11 File Type
  unsigned int temp = 0;
  fwrite(&temp, sizeof(int), 1, f);       // 12-15 File Version
  fwrite(&file_size, sizeof(int), 1, f);  // 16-19 Size of File
  fwrite(&d->grid, sizeof(char), X_LENGTH * Y_LENGTH, f); // 20-1699 Dungeon Matrix
  
  int i;
  for(i = 0; i < d->room_count; i++){ // 1700-end Rooms
    room_t *r = d->rooms[i];
    fwrite(&r->pos_y, sizeof(char), 1, f);
    fwrite(&r->pos_x, sizeof(char), 1, f);
    fwrite(&r->size_y, sizeof(char), 1, f);
    fwrite(&r->size_x, sizeof(char), 1, f);
  }
  
  fclose(f);
  return 1;
}

int load(dungeon_t *d){
  FILE *f = fopen(FILE_PATH, "r");
  if(!f){
    fprintf(stderr, "Couldn't open file\n");
    return 0;
  }
  
  fseek(f, 16, SEEK_SET);
  
  char file_size, room_count;
  fread(&file_size, sizeof(char), 1, f); //File size
  room_count = (file_size - 1700) / 4;
  d->room_count = room_count;
  d->rooms = (room_t**)malloc(sizeof(room_t*) * room_count);
  
  fread(&d->grid, sizeof(char), X_LENGTH * Y_LENGTH, f);
  int i;
  for(i = 0; i < room_count; i++){
    room_t *r = create_room();
    fread(&r->pos_y, sizeof(char), 1, f);
    fread(&r->pos_x, sizeof(char), 1, f);
    fread(&r->size_y, sizeof(char), 1, f);
    fread(&r->pos_x, sizeof(char), 1, f);
    d->rooms[i] = r;
  }
  
  fclose(f);
  return 1;
  
  
}
