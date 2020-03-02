#include "snake.h"
#include <cmath>
#include <iostream>
#include <mutex>

Snake::Snake(Board *board_): grid_width(board_->width),
        grid_height(board_->height),
        head_x(board_->width / 2),
        head_y(board_->height / 2) {  
}

Snake::Snake(const Snake &source)
{
    std::unique_lock<std::mutex> lck(mtx_);
    std::unique_lock<std::mutex> lck_src(source.mtx_);
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = source.body;
    growing = source.growing;
    board = source.board;
}

Snake::~Snake() {

}

Snake::Snake(Snake &&source)
{
    std::unique_lock<std::mutex> lck(mtx_);
    std::unique_lock<std::mutex> lck_src(source.mtx_);
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = std::move(source.body);
    growing = source.growing;
    board = source.board; 
    source.board = nullptr;   
}

Snake &Snake::operator=(const Snake &source)
{
  	if (this == &source)
        return *this;

    std::unique_lock<std::mutex> lck(mtx_);
    std::unique_lock<std::mutex> lck_src(source.mtx_);
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = source.body;
    growing = source.growing;
    board = source.board;
    return *this;
}

//// EOF STUDENT CODE
Snake &Snake::operator=(Snake &&source)
{ 
    
    if (this == &source)
      return *this;
    std::unique_lock<std::mutex> lck(mtx_);
    std::unique_lock<std::mutex> lck_src(source.mtx_);
    speed = source.speed;
    size = source.size;
    alive = source.alive;
    head_x = source.head_x;
    head_y = source.head_y;
    body = std::move(source.body);
    growing = source.growing;
    board = source.board;
    source.board = nullptr;
    return *this;
}

void Snake::SetBoard(Board * board_) {
  board = board_;
  board->SetCellValue(static_cast<int>(grid_width/2), static_cast<int>(grid_height/2), Board::CellContent::SnakeHead);
}

void Snake::AddLife() {
  std::unique_lock<std::mutex> lck(mtx_);
  lifeCount++;
  if (lifeCount > maxLifeCount)
      return;
  board->SetCellValue(static_cast<int>(grid_width/2), static_cast<int>(grid_height/2), Board::CellContent::SnakeHead);
  size = 1;
  speed = 0.1f;
  alive = true;
}

void Snake::ChangeDirection(Snake::Direction newDirection) {
  Snake::Direction opposite ;
  switch(newDirection)
  {
    case  Snake::Direction::kDown  : opposite = Snake::Direction::kUp; break;
    case  Snake::Direction::kUp    : opposite = Snake::Direction::kDown;break;
    case  Snake::Direction::kLeft  : opposite = Snake::Direction::kRight; break;
    case  Snake::Direction::kRight : opposite = Snake::Direction::kLeft; break;
  }
  std::unique_lock<std::mutex> lck(mtx_);
  if (direction != opposite || size == 1) 
    direction = newDirection;
  return;
}

void Snake::IncrementSpeed(float value) {
  std::unique_lock<std::mutex> lck(mtx_); 
  speed += value;
}

void Snake::Update() {
  
  std::cout << "update snake" << std::endl;
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // We first capture the head's cell before updating.

  UpdateHead();

  if (!GetIsAlive()){
      std::cout << "Snake die !!!" << std::endl;
      return;
  }

  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {

    UpdateBody(current_cell, prev_cell);
  }
}
bool Snake::GetIsAlive() {
  std::unique_lock<std::mutex> lck(mtx_);
  return alive;
}

void Snake::SetAlive(bool value) {
    std::unique_lock<std::mutex> lck(mtx_);
    alive = value;
}

void Snake::UpdateHead() {
   std::unique_lock<std::mutex> lck(mtx_);  
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
  int new_head_board_x = static_cast<int>(head_x);
  int new_head_board_y = static_cast<int>(head_y);
  if (board->GetCellValue(new_head_board_x, new_head_board_y) == Board::CellContent::SnakeBody) {
     lck.unlock();
     Die();
  }
  else
     board->SetCellValue(new_head_board_y, new_head_board_y, Board::CellContent::SnakeHead);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
   std::unique_lock<std::mutex> lck(mtx_);  
  board->SetCellValue(prev_head_cell.x, prev_head_cell.y, Board::CellContent::SnakeBody);
 
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    board->SetCellValue((*body.begin()).x, (*body.begin()).y, Board::CellContent::Empty);
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  
}

void Snake::Die(){
  std::unique_lock<std::mutex> lck(mtx_);  
  alive = false;
  body.clear();
  
  std::cout << "Snake dying!!!!" << std::endl;
}

void Snake::GrowBody() { 
  std::unique_lock<std::mutex> lck(mtx_);
  growing = true; 
}
