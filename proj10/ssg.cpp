#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include "game.cpp"


void test(){
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;

  environment *env = new environment();
  env->q->enqueue((void*)&a);
  env->q->enqueue((void*)&b);
  env->q->enqueue((void*)&c);
  env->q->enqueue((void*)&d);
  env->q->enqueue((void*)&e);
  
  int f = *(int*)env->q->dequeue();
  cout << "should be 1: " << f << endl;
  int g = *(int*)env->q->dequeue();
  cout << "should be 2: " << g << endl;
  
  env->q->enqueue((void*)&f);
  env->q->enqueue((void*)&g);
  
  int h = *(int*)env->q->dequeue();
  cout << "should be 3: " << h << endl;
  int i = *(int*)env->q->dequeue();
  cout << "should be 4: " << i << endl;
  int j = *(int*)env->q->dequeue();
  cout << "should be 5: " << j << endl;
  int k = *(int*)env->q->dequeue();
  cout << "should be 1: " << k << endl;
  
  delete env;
}


int main(int argc, char *argv[]){
  srand(time(NULL));

  init_game();
  start_game();
  
  return 0;
}


