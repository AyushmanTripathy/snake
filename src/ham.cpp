#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <utility>
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

std::unordered_set<int> visited;
std::vector<int> hamiltonian_path;

extern std::random_device dev;
std::uniform_int_distribution<int> ndev(0, 3);

bool generate_hamiltonian_path(int x, int y) {
  if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE)
    return false;
  const int index = x + GRID_SIZE * y;
  if (visited.find(index) != visited.end())
    return false;

  visited.insert(index);
  if (visited.size() == GRID_SIZE * GRID_SIZE) {
    for (int i = 0; i < 4; i++) {
      if (x + all_dirs[i][0] == 0 && y + all_dirs[i][1] == 0) {
        hamiltonian_path.push_back(i);
        return true;
      }
    }
    visited.erase(index);
    return false;
  }

  bool found = false;
  for (int i = 0, o = 0; i < 4; i++, o = (o + i) % 4) {
    if (generate_hamiltonian_path(x + all_dirs[o][0], y + all_dirs[o][1])) {
      hamiltonian_path.push_back(o);
      return true;
    }
  }
  visited.erase(index);
  return found;
}

int snake_ai_ham () {
  static bool path_generated = false;
  if (!path_generated) {
    visited.clear();
    std::cout << "Generating hamiltonian_path\n";
    if (generate_hamiltonian_path(0, 0)) std::cout << "Found hamiltonian path\n";
    else end_game("hamiltonian path not Found");
    std::reverse(hamiltonian_path.begin(), hamiltonian_path.end());
    path_generated = true;
    for (auto i = hamiltonian_path.begin(); i != hamiltonian_path.end(); i++)
      std::cout << *i << " ";
  }
  static int index = -1;
  index = (index + 1) % hamiltonian_path.size();
  return hamiltonian_path[index];
}

int (*snake_ai)() = &snake_ai_ham;
