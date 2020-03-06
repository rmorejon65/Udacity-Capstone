#ifndef SNAKECOMMAND_H
#define SNAKECOMMAND_H
#include "command.h"
#include "snake.h"

class SnakeCommand : public Command {
    public:
        SnakeCommand(Snake *snake, Snake::Direction direction);
        virtual void execute();
        virtual ~SnakeCommand();
    private:
        Snake * snake_;
        Snake::Direction direction_;
};

#endif