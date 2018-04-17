
typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef int16_t pair_t[num_dims];

class character{
  public:
    pair_t pos;
    bool alive;
    uint32_t color;
};
