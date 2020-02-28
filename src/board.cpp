#include "board.h"
#include <vector>
#include <iostream>
#include <mutex>

Board::Board(std::size_t grid_width, std::size_t grid_height):width(grid_width), height(grid_height)
{
    std::vector<std::vector<CellContent> > temp(width, std::vector<CellContent>(height, CellContent::Empty));
    this->_cells = std::move(temp);
}

CellContent Board::GetCellValue(int x, int y)
{
    if (IsCellValid(x,y))
    {
        std::unique_lock<std::mutex> lck(mtx_);
        return _cells[x][y];
    }
    return CellContent::Unknown;
}

void Board::SetCellValue(int x, int y, CellContent value)
{  
    if (IsCellValid(x,y))
    {
        std::unique_lock<std::mutex> lck(mtx_);
        _cells[x][y] = value;
    }
}

bool Board::IsCellValid(int x, int y)
{
    if (x >= this->width || y >= this->height)
        return false;
    return true;
}