#ifndef BOARD_H
#define BOARD_H

#include <random>
#include <iostream>
#include <vector>
#include <mutex>

enum CellContent { Unknown = -1, Empty = 0, SnakeHead = 1, SnakeBody =2, FoodKind = 3};
class Board {
 public:
    Board(std::size_t grid_width, std::size_t grid_height);
    CellContent GetCellValue(int x, int y);
    void SetCellValue(int x, int y, CellContent value);
    bool IsCellValid(int x, int y);
    std::size_t width;
    std::size_t height;
 private:
    std::vector<std::vector<CellContent> > _cells;
    std::mutex mtx_;
   
};

#endif