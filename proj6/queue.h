
class node_t{
  public:
    void *value;
    node_t *next;
    
    node_t *create_node(void *v);
};

class queue_t{
  public:
    int size;
    node_t *first;
    
    queue_t *init_queue();
    void enqueue(queue_t *, void *);
    void enqueue_sort(queue_t *, void *, 
                          int(*comp)(const void *, const void *));
    void *dequeue(queue_t *);
};








