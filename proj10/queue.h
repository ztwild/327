
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








