#include "game.h"
#include <iostream>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <future>
#include "SDL.h"
#include "board.h"
#include "food.h"
#include "snake.h"


Game::Game(std::size_t grid_width, std::size_t grid_height) :snake(grid_width, grid_height),food(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  
  board = std::make_unique<Board>(grid_width, grid_height);
  snake.SetBoard(board.get());
  food.SetBoard(board.get());
  food.SetExpirationTime(10);

  food.Place();

}

bool Game::HandleInput(Controller const &controller) {
  bool running = true;
  controller.HandleInput(running, snake);
  return running;
}

void Game::Render(Renderer *renderer) {
  //std::unique_lock<std::mutex> lck(mtx);
  //cv.wait(lck);
  renderer->Render(snake);
  renderer->Render(food);
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
    auto handle_input = std::async(std::launch::async, &Game::HandleInput, this, controller);
    running = handle_input.get();
    auto handle_update = std::async(std::launch::async, &Game::Update, this, &renderer);
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
 
  }
}

void Game::Update(Renderer *renderer) {
  
  if (!snake.GetIsAlive()) 
      return;
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  snake.Update(food);
  score += snake.GetReward();
  //cv.notify_one();
  if (!snake.GetIsAlive()) {
    board->Clear();   
    snake.AddLife();  
    renderer->GameOver(score, snake.GetRemainingLifeCount());
    score = 0;
    
    food.Place();
  }
  else
    food.CheckExpired();  
  
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }