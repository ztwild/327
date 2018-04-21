#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <iostream>
#include "game.cpp"


void test(){
  queue q; 
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;

  q.enqueue((void*)&a);
  q.enqueue((void*)&b);
  q.enqueue((void*)&c);
  q.enqueue((void*)&d);
  q.enqueue((void*)&e);
  
  int f = *(int*)q.dequeue();
  cout << "should be 1: " << f << endl;
  int g = *(int*)q.dequeue();
  cout << "should be 2: " << g << endl;
  
  q.enqueue((void*)&f);
  q.enqueue((void*)&g);
  
  int h = *(int*)q.dequeue();
  cout << "should be 3: " << h << endl;
  int i = *(int*)q.dequeue();
  cout << "should be 4: " << i << endl;
  int j = *(int*)q.dequeue();
  cout << "should be 5: " << j << endl;
  int k = *(int*)q.dequeue();
  cout << "should be 1: " << k << endl;
  
}


int main(int argc, char *argv[]){
  srand(time(NULL));

  init_game();
  start_game();
  
  return 0;
}


