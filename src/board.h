#ifndef BOARD_H
#define BOARD_H

#include <random>
#include <iostream>
#include <vector>
#include <mutex>


class Board {
 public:
    enum CellContent { Unknown = -1, Empty = 0, SnakeHead = 1, SnakeBody =2, FoodKind = 3};
    Board(std::size_t grid_width, std::size_t grid_height);
    Board::CellContent GetCellValue(int x, int y);
    void SetCellValue(int x, int y, Board::CellContent value);
    bool IsCellValid(int x, int y);
    void Clear();
    std::size_t width;
    std::size_t height;
 private:
    std::vector<std::vector<Board::CellContent> > _cells;
    std::mutex mtx_;
   
};

#endif