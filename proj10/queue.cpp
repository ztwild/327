
class node{
  public:
    void *value;
    node *next;
    node(void *v){
      value = v;
      next = NULL;
    }
    ~node(){
      delete next;
    }
};

class queue{
  public:
    int size;
    node *first;
    queue(){
      size = 0;
      first = NULL;
    }
    ~queue(){
      delete first;
    }
    void enqueue(void *);
    //void enqueue_sort(void *, int(*comp)(const void *, const void *));
    void *dequeue();
};

void queue::enqueue(void *value){
  node *n = new node(value);
  
  if(!first){
    first = n;
  }else{
    node *temp = first;
    while(temp->next){
      temp = temp->next;
    }
    temp->next = n;
  }
  size++;
}

void *queue::dequeue(){
  void *value = NULL;
  if(size){
    size--;
    node *n = first;
    first = n->next;
    n->next = NULL;
    value = n->value;
    n->value = NULL;
    delete n;
  }
  return value;
}

/**
void queue::enqueue_sort(void *v, int(*comp)(const void *, const void *)){
  node *n = new node(v);
  
  if(!first){
    first = n;
  }
  else if(comp(v, first->value)){
    n->next = first;
    first = n;
  }
  else{
    int inserted = 0;
    node *prev = first;
    node *cur = prev->next;
    while(!inserted){
      if(!cur){
        prev->next = n;
        inserted = 1;
      }
      else if(comp(v, cur->value)){
        n->next = cur;
        prev->next = n;
        inserted = 1;
      }
      else{
        prev = cur;
        cur = next;
      }
    }
  }
  size++;
}
**/







