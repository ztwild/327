#include "load_save.h"

char *get_path(){
  int length = strlen(getenv("HOME")) + strlen(FILE_PATH) + 1;
  char *p = (char*)malloc(sizeof(char) * length);
  strcpy(p, getenv("HOME"));
  strcat(p, FILE_PATH);
  return p;
}

//dungeon_t *load_dungeon(char *file_name){
dungeon_t *load_dungeon(){
  int i;
  uint32_t version, size, little_endian;
  char *semantic = (char*)malloc(sizeof(char) * 12);
  
  char *file_path = get_path();
  
  /**
  char *temp = "examples/";
  char *temp2 = ".rlg327";
  int length = strlen(file_name) + strlen(temp) + strlen(temp2) + 1;
  char *file_path = malloc(sizeof(char)*length);
  file_path = strcpy(file_path, temp);
  file_path = strcat(file_path, file_name);
  file_path = strcat(file_path, temp2);
  **/
  
  dungeon_t *d = (dungeon_t*)malloc(sizeof(dungeon_t));
  init_grid(d);
  
  FILE *f = fopen(file_path, "r");
  if(!f){
    fprintf(stderr, "Couldn't open file\n");
    return NULL;
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
  free(file_path);
  
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
    
  }
  
  
  fclose(f);
  return 1;
}


