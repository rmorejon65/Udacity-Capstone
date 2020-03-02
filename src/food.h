#ifndef FOOD_H
#define FOOD_H

#include <random>
#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include "SDL.h"
#include "board.h"




class Food {
 public:
    enum FoodType { Normal = 1, Plus = 2 , Poison = 3};

    Food(size_t width, size_t height);
    SDL_Point GetPoint()  ;
    void SetBoard(Board *board);
    FoodType GetFoodType();
    void SetExpirationTime(int value);
    int GetExpirationTime();
    void Place();
    void CheckExpired();
 private:
    SDL_Point point;
    FoodType foodType {Food::FoodType::Normal};
    Board *board;
    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_food;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;
    std::mutex mtx_;
    std::chrono::steady_clock::time_point lastTimePlaced;
    int expirationTime;

};

#endif