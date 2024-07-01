#include <SDL2/SDL.h>

#include <SDL2/SDL_render.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

const int GRID_SIZE = 50;
const int PIXEL_SIZE = 20;
const int FRAME_RATE = 10;

typedef struct {
  int x;
  int y;
} point;

std::random_device dev;
std::uniform_int_distribution<int> nd(0, GRID_SIZE - 1);

int all_dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

bool is_running = true, is_paused = false;

int snake_length = 3, dir = 1;
point snake_body[GRID_SIZE * GRID_SIZE] = {{2, 1}, {2, 2}, {2, 3}};
point food = {8, 2};
point* snake_head = &snake_body[snake_length - 1];

void end_game(std::string msg) {
  std::cout << "Game Ended: " << msg << "\n";
  is_running = false;
}

void set_pause(bool val, std::string msg) {
  std::cout << (val ? "Paused" : "Played") << ": " << msg << "\n";
  is_paused = val;
}

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

std::vector<int> cached_dirs;

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
    if (!visited[GRID_SIZE * head->x + head->y]) available_dirs.push_back({ distance(&food, head), i });
    unmove_in_dir(head, all_dirs[i]);
  }

  std::sort(available_dirs.begin(), available_dirs.end(), dir_sort_function);

  visited[GRID_SIZE * head->x + head->y] = true;
  for (int i = 0; i < available_dirs.size(); i++) {
    int d = available_dirs[i].second;
    move_in_dir(head, all_dirs[d]);
    if (snake_ai_helper(head, visited)) {
      cached_dirs.push_back(d);
      return true;
    }
    unmove_in_dir(head, all_dirs[d]);
  }

  return false;
}

int snake_ai() {
  // use cached path if avaliable
  if (cached_dirs.empty()) {
    std::cout << "Calculating new cached path\n";

    bool visited[GRID_SIZE * GRID_SIZE] = {false};
    for (int i = 0; i < snake_length; i++) {
      visited[snake_body[i].x * GRID_SIZE + snake_body[i].y] = true;
    }

    point ai_head = { snake_head->x, snake_head->y };
    if (!snake_ai_helper(&ai_head, visited)) {
      end_game("no path found ... giving up");
      SDL_Delay(3000);
    } else std::cout << "New cached path found of length " << cached_dirs.size() << "\n";
  }
  
  int d = cached_dirs.back();
  cached_dirs.pop_back();
  return d;
}

void game_loop() {
  // check for snake_head on food
  if (food.x == snake_head->x + all_dirs[dir][0] && food.y == snake_head->y + all_dirs[dir][1]) {
    // if true, then dont move the snake body forward
    // add a new cell as head to body
    std::cout << "Ate food\n";

    snake_body[snake_length].x = food.x;
    snake_body[snake_length].y = food.y;
    snake_head = &snake_body[snake_length];
    snake_length++;

    // selecting new food
    do {
      food.x = nd(dev);
      food.y = nd(dev);
    } while (arr_includes_point(snake_body, snake_length, &food));

  } else {
    // moving the rest of the body
    for (int i = 0; i < snake_length - 1; i++) {
      snake_body[i].x = snake_body[i + 1].x;
      snake_body[i].y = snake_body[i + 1].y;
    }
    // moving the head
    move_in_dir(snake_head, all_dirs[dir]);
  }
}

int main() {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer((GRID_SIZE + 2) * PIXEL_SIZE,
                              (GRID_SIZE + 2) * PIXEL_SIZE, 0, &window,
                              &renderer);
  SDL_RenderSetScale(renderer, PIXEL_SIZE, PIXEL_SIZE);

  SDL_Event e;

  while (is_running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        end_game("quit event");
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_SPACE:
            set_pause(!is_paused, "Space key");
            break;
          case SDLK_q:
            end_game("Q key");
            break;
          case SDLK_UP:
            dir = 2;
            break;
          case SDLK_DOWN:
            dir = 0;
            break;
          case SDLK_RIGHT:
            dir = 1;
            break;
          case SDLK_LEFT:
            dir = 3;
            break;
        }
      }
    }

    if (is_paused) continue;

    // game win sitution
    if (snake_length == GRID_SIZE * GRID_SIZE - 1) set_pause(true, "Game won");

    // find new direction
    dir = snake_ai();

    game_loop();

    // painting
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // borders
    for (int i = 0; i < GRID_SIZE + 2; i++) {
      SDL_RenderDrawPoint(renderer, 0, i);
      SDL_RenderDrawPoint(renderer, i, 0);
      SDL_RenderDrawPoint(renderer, GRID_SIZE + 1, i);
      SDL_RenderDrawPoint(renderer, i, GRID_SIZE + 1);
    }

    // path ahead
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
    int x = snake_head->x, y = snake_head->y;
    for (int i = cached_dirs.size() - 1; i >= 0; i--) {
      x += all_dirs[cached_dirs[i]][0];
      y += all_dirs[cached_dirs[i]][1];
      SDL_RenderDrawPoint(renderer, x + 1, y + 1);
    }

    // food
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, food.x + 1, food.y + 1);

    // snake
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
    for (int i = 0; i < snake_length; i++) {
      SDL_RenderDrawPoint(renderer, snake_body[i].x + 1, snake_body[i].y + 1);
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(FRAME_RATE);

    // game end situation
    if (arr_includes_point(snake_body, snake_length - 1, snake_head)) {
      end_game("head body collision dectected");
      SDL_Delay(1000);
    }
  }
  std::cout << "score: " << snake_length << "\n";
  return 0;
}
