#include "iostream"
#include "algorithm"
#include <cfloat>
#include <vector>
#include <math.h>

#include "defs.h"

std::vector<int> cached_ai_path;

extern bool is_on_boundry(point *); 
extern void move_in_dir(point *, int[]);
extern float distance(point *, point *);
extern void unmove_in_dir(point *, int[]);
extern void end_game(std::string);

extern int all_dirs[][2];
extern int snake_length, dir;
extern point food, * snake_head;
extern point snake_body[];

float distance(point * dest, point * src) {
  return sqrt(pow(dest->x - src->x, 2) + pow(dest->y - src->y, 2));
}

bool dir_sort_function(std::pair<float, int>& a, std::pair<float, int>& b) {
  return a.first < b.first;
}

bool snake_ai_helper(point * head, bool visited[]) {
  if (food.x == head->x && food.y == head->y) return true;

  std::vector<std::pair<float, int>> available_dirs;
  float best_dir = FLT_MIN;

  for (int i = 0; i < 4; i++) {
    move_in_dir(head, all_dirs[i]);
    if (!is_on_boundry(head) && !visited[GRID_SIZE * head->x + head->y])
      available_dirs.push_back({ distance(&food, head), i });
    unmove_in_dir(head, all_dirs[i]);
  }

  std::sort(available_dirs.begin(), available_dirs.end(), dir_sort_function);

  visited[GRID_SIZE * head->x + head->y] = true;
  for (int i = 0; i < available_dirs.size(); i++) {
    int d = available_dirs[i].second;
    move_in_dir(head, all_dirs[d]);
    if (snake_ai_helper(head, visited)) {
      cached_ai_path.push_back(d);
      return true;
    }
    unmove_in_dir(head, all_dirs[d]);
  }

  return false;
}

int snake_ai_simple() {
  // use cached path if avaliable
  if (cached_ai_path.empty()) {
    std::cout << "Calculating new cached path\n";

    bool visited[GRID_SIZE * GRID_SIZE] = {false};
    for (int i = 0; i < snake_length; i++) {
      visited[snake_body[i].x * GRID_SIZE + snake_body[i].y] = true;
    }

    point ai_head = { snake_head->x, snake_head->y };
    if (!snake_ai_helper(&ai_head, visited)) {
      end_game("no path found ... giving up");
    } else std::cout << "New path found of length " << cached_ai_path.size() << "\n";
  }
  
  int d = cached_ai_path.back();
  cached_ai_path.pop_back();
  return d;
}

int (*snake_ai)() = &snake_ai_simple;
