#include "game.h"
#include <iostream>
#include <memory>
#include <thread>
#include <future>
#include "SDL.h"
#include "board.h"
#include "food.h"
#include "snake.h"

/*Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  board = std::make_unique<Board>(grid_width, grid_height);
  PlaceFood();
}*/

Game::Game(std::size_t grid_width, std::size_t grid_height) :snake(grid_width, grid_height),food(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  
  board = std::make_unique<Board>(grid_width, grid_height);
  snake.SetBoard(board.get());
  food.SetBoard(board.get());
  food.SetExpirationTime(10);
  std::cout << "Constructor Game" << std::endl;
  food.Place();

  //PlaceFood();
}

bool Game::HandleInput(Controller const &controller) {
  bool running = true;
  controller.HandleInput(running, snake);
  std::cout << "In handle Input " << std::endl;
  return running;
}

void Game::Render(Renderer *renderer) {
  std::cout << "In Renderer " << std::endl;
  renderer->Render(snake, food);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;
  
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.

    //controller.HandleInput(running, snake);
    std::cout << "Game::Run" << std::endl;
    
    auto handle_input = std::async(std::launch::async, &Game::HandleInput, this, controller);
    running = handle_input.get();
    auto handle_update = std::async(std::launch::async, &Game::Update, this);
    auto handle_render = std::async(std::launch::async, &Game::Render, this, &renderer);
    
    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
    
    handle_update.get();
    handle_render.get();
    if (!snake.GetIsAlive()) {
      board->Clear();     
      renderer.GameOver(score, frame_count);
      snake.AddLife();
    }
  }
}

void Game::PlaceFood() {
  /*int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    //if (!snake.SnakeCell(x, y)) {
    if (board->GetCellValue(x,y) == CellContent::Empty) {
      food.x = x;
      food.y = y;
      board->SetCellValue(x,y,CellContent::FoodKind);
      std::cout << x << " " << y << std::endl;
      return;
    }
  }*/
}

void Game::Update() {
  std::cout << "In Update " << std::endl;
  if (!snake.GetIsAlive()) return;
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  std::cout << "Before Check Food Expired " << std::endl;
  food.CheckExpired();
  snake.Update();
  if (!snake.GetIsAlive())
      return;
  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over h
  if (new_x == food.GetPoint().x && new_y == food.GetPoint().y)
  {
    score++;
    //PlaceFood();
    switch(food.GetFoodType())
    {
      case Food::FoodType::Poison:
        snake.Die();
        break;
      case Food::FoodType::Normal:
        snake.GrowBody();
        snake.IncrementSpeed(0.001);
        food.Place();
        break;
      case Food::FoodType::Plus:
        snake.GrowBody();
        snake.IncrementSpeed(0.002);
        food.Place();
        break;
    }
    
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }