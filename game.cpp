#include <SDL2/SDL.h>
#include <iostream>
#include <random>

#include "defs.h"

int all_dirs[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

int snake_length = 3, dir = 1;
point snake_body[GRID_SIZE * GRID_SIZE] = {{2, 1}, {2, 2}, {2, 3}};
point food = {8, 2};
point* snake_head = &snake_body[snake_length - 1];

int frame_rate = FRAME_RATE;

extern int (* snake_ai)();
extern std::vector<int> cached_ai_path;
extern bool is_running;
extern bool is_paused;

extern void end_game(std::string);
extern void set_pause(bool, std::string);
extern bool arr_includes_point(point *, int, point *); 
extern void move_in_dir(point *, int[]);

std::random_device dev;
std::uniform_int_distribution<int> nd(0, GRID_SIZE - 1);

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

void paint(SDL_Renderer * renderer) {
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
    for (int i = cached_ai_path.size() - 1; i >= 0; i--) {
      x += all_dirs[cached_ai_path[i]][0];
      y += all_dirs[cached_ai_path[i]][1];
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
    SDL_Delay(frame_rate);
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
          case SDLK_PERIOD:
            frame_rate -= 10;
            if (frame_rate < 10) frame_rate = 10;
            break;
          case SDLK_COMMA:
            frame_rate += 10;
            if (frame_rate > 1000) frame_rate = 1000;
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
    paint(renderer);

    // game end situation
    if (arr_includes_point(snake_body, snake_length - 1, snake_head)) {
      end_game("head body collision dectected");
      SDL_Delay(1000);
    }
  }
  std::cout << "score: " << snake_length << "\n";
  return 0;
}
