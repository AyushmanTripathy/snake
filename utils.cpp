#include "defs.h"

bool is_running = true, is_paused = false;

bool arr_includes_point(point * arr, int arr_length, point * p) {
  for (int i = 0; i < arr_length; i++) {
    if (arr[i].x == p->x && arr[i].y == p->y) {
      return true;
    }
  }
  return false;
}

void fold_through_borders(point * p) {
  if (p->x >= GRID_SIZE) p->x = 0;
  else if (p->x < 0) p->x = GRID_SIZE - 1;
  if (p->y >= GRID_SIZE) p->y = 0;
  else if (p->y < 0) p->y = GRID_SIZE - 1;
}

void move_in_dir(point * p, int d[2]) {
  p->x += d[0];
  p->y += d[1];
  fold_through_borders(p);
}

void unmove_in_dir(point * p, int d[2]) {
  p->x -= d[0];
  p->y -= d[1];
  fold_through_borders(p);
}

void end_game(std::string msg) {
  std::cout << "Game Ended: " << msg << "\n";
  is_running = false;
}

void set_pause(bool val, std::string msg) {
  std::cout << (val ? "Paused" : "Played") << ": " << msg << "\n";
  is_paused = val;
}
