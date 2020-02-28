#include <iostream>
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

FoodType Food::GetFoodType() {
    std::unique_lock<std::mutex> lck(mtx_);
    return foodType;
}

void Food::Place() {
  int x, y;
  std::unique_lock<std::mutex> lck(mtx_);
  foodType = static_cast<FoodType>(random_food(engine));

  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    
    if (board->GetCellValue(x,y) == CellContent::Empty) {
      point.x = x;
      point.y = y;
      board->SetCellValue(x,y,CellContent::FoodKind);
      return;
    }
  }    
}
