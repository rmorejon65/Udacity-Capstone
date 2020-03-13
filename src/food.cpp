#include <iostream>
#include <chrono>
#include <mutex>
#include <random>
#include "food.h"
#include "board.h"


Food::Food(size_t width, size_t height):engine(dev()), 
                                        random_food(1,3),
                                        random_w(0, static_cast<int>(width)),
                                        random_h(0, static_cast<int>(height))
{
    
}

SDL_Point Food::GetPoint() 
{
    std::unique_lock<std::mutex> lck(mtx_);
    return point;
}

void Food::SetBoard(Board *board_) {
    board = board_;   
}

void Food::SetExpirationTime(int value) {
  std::unique_lock<std::mutex> lck(mtx_);
  expirationTime = value;
}

int Food::GetExpirationTime() {
  std::unique_lock<std::mutex> lck(mtx_);
  return expirationTime;
}

void Food::CheckExpired(){
  std::unique_lock<std::mutex> lck(mtx_);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now() ;
  typedef std::chrono::duration<int,std::milli> millisecs_t ;
  millisecs_t duration( std::chrono::duration_cast<millisecs_t>(end-lastTimePlaced) ) ;
  
  int elapsedTimeSeconds = duration.count()/1000;
  if (elapsedTimeSeconds >= expirationTime) {
    lck.unlock();
    Place();
  }
}

Food::FoodType Food::GetFoodType() {
    std::unique_lock<std::mutex> lck(mtx_);
    return foodType;
}

void Food::Place() {
  int x, y;
  std::unique_lock<std::mutex> lck(mtx_);
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    if (!board->IsCellValid(x,y))
       continue;

    if (board->GetCellValue(x,y) == Board::CellContent::Empty) {
      std::cout << "Found Empty Cell For FOOD" << std::endl;
      Food::FoodType previous = foodType;
      while (previous == foodType)
            foodType = static_cast<FoodType>(random_food(engine));
      int previous_x = point.x;
      int previous_y = point.y;
      point.x = x;
      point.y = y;
      board->SetCellValue(x,y,Board::CellContent::FoodKind);
      board->SetCellValue(previous_x, previous_y, Board::CellContent::Empty);
      lastTimePlaced = std::chrono::steady_clock::now() ;
      std::cout << "Food Placed " << std::endl;
      return;
    }
  }    
}
