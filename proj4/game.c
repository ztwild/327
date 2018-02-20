#include <ncurses.h>
#include <unistd.h>
#include "dungeon.c"
#include "load_save.c"
#include "path_find.c"


dungeon_t *dungeon;


void init_game(uint8_t load, uint8_t nummon){
  dungeon = load ? load_dungeon() : new_dungeon();
  dungeon->nummon = nummon;
  printf("There should be %d monsters\n", nummon);
  
  /** path_find **/
  init_characters(dungeon);
  init_path(dungeon);
  bfs(dungeon);
  dijkstra(dungeon);
}

queue_t *init_turn_queue(dungeon_t *);

int end_game(dungeon_t *);

void print_queue(queue_t *);

int run_game(uint8_t save){
  uint8_t done = 0;
  queue_t *turn = init_turn_queue(dungeon);
  printf("turn queue is initilaized\n");
  while(!end_game(dungeon)){
    printf("game still in progress\n");
    character_t *c = (character_t*)dequeue(turn);
    printf("dequeued character\n");
    c->turn += 1000 / c->speed;
    printf("character turn finished\n");
    enqueue_sort(turn, (void*)c, compare_characters);
    printf("enqueued character\n");
    print_queue(turn);
    sleep(2);
  }
    
  print_grid(dungeon);
    
  
  if(save){
    save_dungeon(dungeon);
  }
  return 0;
}

queue_t *init_turn_queue(dungeon_t *d){
  int i;
  queue_t *q = init_queue();
  enqueue(q, d->pc);
  for(i = 0; i < d->nummon; i++){
    enqueue(q, d->monsters[i]);
  }
  return q;
}

int end_game(dungeon_t *d){
  int dead = !d->pc->alive;
  int mons_dead = 1;
  int i;
  for(i = 0; i < d->nummon; i++){
    mons_dead &= !d->monsters[i]->alive;
  }
  return dead || mons_dead;
}

void print_queue(queue_t *q){
  node_t *temp = q->first;
  while(temp){
    character_t *c = (character_t*)temp->value;
    printf("Time: %d, Speed: %d, Turn: %d\n", c->time, c->speed, c->turn);
    temp = temp->next;
  }
  printf("\n");
}




void print_test(){
  print_grid(dungeon);
  print_path_open(dungeon);
  print_path_wall(dungeon);
}
