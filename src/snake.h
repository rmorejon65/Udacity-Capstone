#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <mutex>
#include "SDL.h"
#include "board.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {}

  Snake(Board *board_);
  Snake(const Snake &source);
  Snake &operator=(const Snake &source); 
  Snake(Snake &&source);
  Snake &operator=(Snake &&source); 
  ~Snake();
  
  void AddLife();
  void SetLifeCount();
  void SetBoard(Board *board_);   
  void ChangeDirection(Direction newDirection);
  void Update();
  void IncrementSpeed(float value);
  void Die();
  void GrowBody();
  bool GetIsAlive();
  void SetAlive(bool value);
  //bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  int maxLifeCount {5};
  int lifeCount {0};
  std::vector<SDL_Point> body;

 private:

  
  Board *board;
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
  mutable std::mutex mtx_;
};

#endif