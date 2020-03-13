#include "controller.h"
#include <iostream>
#include <memory>
#include "SDL.h"
#include "snake.h"

void Controller::HandleInput(bool &running, Snake &snake)  const {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      
      switch (e.key.keysym.sym) {
        case SDLK_UP:
            { 
              SnakeCommand snakeCommand_(&snake, Snake::Direction::kUp);
              snakeCommand_.execute();
            }
          break;

        case SDLK_DOWN:
            {
              SnakeCommand snakeCommand_ (&snake, Snake::Direction::kDown);
              snakeCommand_.execute();
            }
          break;
 
        case SDLK_LEFT:
          {
            SnakeCommand snakeCommand_(&snake, Snake::Direction::kLeft);
            snakeCommand_.execute();
          }
          break;

        case SDLK_RIGHT:
          {
            SnakeCommand snakeCommand_(&snake, Snake::Direction::kRight);
            snakeCommand_.execute();
          }
          break;
      }
      
    }
  }
}
