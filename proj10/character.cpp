
typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

typedef enum char_type {
  PC,
  NPC,
  FIRE_BALL,
} char_type;

typedef int16_t pair_t[num_dims];

class character{
  public:
    bool alive;
    pair_t pos;
    uint32_t color;
    char_type type;
};

