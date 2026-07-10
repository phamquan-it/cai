#include "Position.hpp"
#include <iostream>

bool isPositionInRange(const Position& pos, const Position& start, const Position& end) {
    // Kiểm tra row nằm trong khoảng
    bool rowInRange = (pos.row >= start.row && pos.row <= end.row);
    // Kiểm tra col nằm trong khoảng
    bool colInRange = (pos.col >= start.col && pos.col <= end.col);
    
    return rowInRange && colInRange;
}
