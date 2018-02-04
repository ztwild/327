
typedef struct node{
  void *value;
  struct node *next;
}node_t;

typedef struct queue{
  int size;
  node_t *first;
}queue_t;

node_t *create_node(void *v){
  node_t *n = (node_t*)malloc(sizeof(node_t));
  if(!n){
    fprintf(stderr, "create_node: Cannot allocate mem for node\n");
    return NULL;
  }
  n->value = v;
  n->next = NULL;
  return n;
}

queue_t *init_queue(){
  queue_t *q = (queue_t*)malloc(sizeof(queue_t));
  if(!q){
    fprintf(stderr, "init_queue: Cannot allocate mem for queue\n");
    return NULL;
  }
  q->size = 0;
  q->first = NULL;
  return q;
}

void enqueue(queue_t *q, void *value){
  node_t *n = create_node(value);
  
  if(!q->first){
    q->first = n;
  }else{
    node_t *temp = q->first;
    while(temp->next){
      temp = temp->next;
    }
    temp->next = n;
  }
  q->size++;
}

void *dequeue(queue_t *q){
  void *value = NULL;
  if(q->size){
    q->size--;
    node_t *n = q->first;
    q->first = n->next;
    n->next = NULL;
    value = n->value;
    n->value = NULL;
    free(n);
  }
  return value;
}
