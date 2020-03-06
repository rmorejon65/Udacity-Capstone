#include "snakecommand.h"
#include "command.h"
#include "snake.h"

SnakeCommand::SnakeCommand(Snake *snake, Snake::Direction direction) : snake_(snake), direction_(direction) {
}

SnakeCommand::~SnakeCommand() {
}

void SnakeCommand::execute() {
    snake_->ChangeDirection(direction_);
}
