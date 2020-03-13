# CPPND: Capstone Snake Game Example
The project built is the Snake Game.

We Include the following classes:

1. Game : In this class we implement the GameLoop method in this specific case is the Run method that is in charge to invoke the 3 main parts of the game : The handle_input, the update, and the rendering, we are running each of this parts in separate threads. 
2. Board : In this class we stablish a relationship between the game with a matrix of points, that represent all the posible positions within the game. Each cell inside the board can store different kinds of content (FOOD, SNAKEHEAD, SNAKEBODY, EMPTY). The ownership of the Board belongs to the Game class. 
3. Snake : It represents the snake that is moving inside the Board associated. We store for the snake a vector that represents the body, and a point that represents the head. Also we are maintaining the Direction that is using the snake to move in the Board. Also we include a logic so the snake could have some lifes associated, for now is a predetermined value, but it could be configured in future versions. For this class we implemented the rule of five.
4. Food : This class represents the abstraction of the Food, the attributes are the point where the food is stored and we are using 3 kinds of food (NORMAL, PLUS, POISON), when the snake's head reach a food of type of POISON the game is over. For each kind of food , we have a different color, and also we associated differents rewards for each one . The food has a lifetime associated, so when the lifetime is expired we generate a new position and a new kind of food.
5. Controller: In this class we are implementing the keyboard handle input, and when detects a user command then delegates the processing in the SnakeCommand class, that is in charge of Change the direction in the Snake object.
6. Renderer: This class is in charge of the rendering of the game in the screen. 
7. Command : Just an abstract class that defines the command pattern.
8. SnakeCommand : This is a class that inherits from Command , and implements that abstract class for the Snake.


This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.