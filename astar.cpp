#include <vector>

#include "defs.h"

std::vector<int> cached_ai_path;

extern void move_in_dir(point *, int[]);
extern float distance(point *, point *);
extern void unmove_in_dir(point *, int[]);
extern void end_game(std::string);

extern int all_dirs[][2];
extern int snake_length, dir;
extern point food, * snake_head;
extern point snake_body[];

int snake_ai_astar() {
  return 1;
}

int (*snake_ai)() = &snake_ai_astar;
