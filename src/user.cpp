#include <vector>

std::vector<int> cached_ai_path;

extern int dir;
int prev_dir = dir;

int user_input() {
  if ((prev_dir + 2) % 4 == dir) return prev_dir;
  prev_dir = dir;
  return dir;
}

int (*snake_ai)() = &user_input;
