#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <endian.h>
#include <string.h>
#include <unistd.h>
#include "dungeon.c"
#include "utils.c"

//#define FILE_PATH "examples/1522914515.rlg327"
#define FILE_PATH "/.rlg327/dungeon"
#define SEMANTIC "RLG327-S2018"

char *get_path(){
  int length = 1024 + strlen(FILE_PATH) + 1;
  char *p = malloc(sizeof(char) * length);
  getcwd(p, length);
  strcat(p, FILE_PATH);
  //printf("File Path: %s\n", p);
  return p;
}

dungeon_t *load_dungeon(){
  int i;
  uint32_t version, size, little_endian;
  char *semantic = malloc(sizeof(char) * 12);
  char *file_path = get_path();
  
  dungeon_t *d = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(d);
  
  FILE *f = fopen(file_path, "r");
  if(!f){
    fprintf(stderr, "Couldn't open file\n");
    return NULL;
  }
  
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
  free(file_path);
  return d;
}


int save_dungeon(dungeon_t *d){
  int i;
  uint32_t be, version, size;
  char *file_path = get_path();
  version = 0;
  size = 1700 + (4 * d->room_count);
  be = htobe32(size);
  
  FILE *f = fopen(file_path, "w");
  if(!f){
    fprintf(stderr, "Couldn't open file\n");
    return 0;
  }
  
  fwrite(SEMANTIC, sizeof(char), 12, f);
  fwrite(&version, sizeof(uint32_t), 1, f);
  fwrite(&be, sizeof(uint32_t), 1, f);
  fwrite(&d->hardness, sizeof(uint8_t), X_LENGTH * Y_LENGTH, f);
  
  for(i = 0; i < d->room_count; i++){
    room_t *r = d->rooms[i];
    fwrite(&r->pos_y, sizeof(char), 1, f);
    fwrite(&r->pos_x, sizeof(char), 1, f);
    fwrite(&r->size_y, sizeof(char), 1, f);
    fwrite(&r->size_x, sizeof(char), 1, f);
    free(d->rooms[i]);
  }
  free(d->rooms);
  free(d);
  fclose(f);
  free(file_path);
  return 1;
}


