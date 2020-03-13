#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "snakecommand.h"
class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;
 
 private:
  
};

#endif